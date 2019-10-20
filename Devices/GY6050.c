/**
  ******************************************************************************
  * @file    GY6050.c
  * @author   
  * @version V1.0
  * @date
  * @brief   实验室原 _(串口通信) 和 GY_6050(can通信)的合并
  ******************************************************************************
  */

#include "GY6050.h"
#include "string.h"

GY_IMU_t IMU_Cloud;

bno055_data_u bno055_data[2];

/**
  * @brief  从CAN报文中获取云台陀螺仪信息
  * @param  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void GY6050_getCloundInfo(CanRxMsg RxMessage) {
	
	//报文id确认
	if (RxMessage.StdId == GY6050_SENDID)
	{
		memcpy(bno055_data[0].dataBuff, RxMessage.Data, sizeof(uint8_t[8]));
		
		IMU_Cloud.Eular.Yaw = (float)bno055_data[0].yaw / 100.0f;
		IMU_Cloud.Gyro.z = bno055_data[0].gyro_z;
		IMU_Cloud.Eular.Pitch = (float)bno055_data[0].pitch / 100.0f;
		IMU_Cloud.Gyro.x = bno055_data[0].gyro_x;

		if (abs(IMU_Cloud.Gyro.z) < 2)
		{
			IMU_Cloud.Gyro.z = bno055_data[0].gyro_z = 0;
		} 
		
  //经过跳变边沿
		if (IMU_Cloud.Eular.Yaw - IMU_Cloud.lastYaw < -300) {
			IMU_Cloud.turnCount++;
		}
		if (IMU_Cloud.lastYaw - IMU_Cloud.Eular.Yaw < -300) {
			IMU_Cloud.turnCount--;
		}

		IMU_Cloud.totalYaw = IMU_Cloud.Eular.Yaw + (360 * IMU_Cloud.turnCount);

		IMU_Cloud.lastYaw = IMU_Cloud.Eular.Yaw;

		IMU_Cloud.ImuInfoUpdateFrame++;
		IMU_Cloud.ImuInfoUpdateFlag = 1;

	}
}

  /**
	* @brief  GY_IMU串口初始化
	* @param  None
	* @retval None
	*/
void GY_Usart_Init(uint32_t p_buff, uint32_t buffSize) {

	USART3_QuickInit(115200);
	USART3_RXDMA_Config(p_buff, buffSize);
}

/**
  * @brief      GY_IMU数据包校验
  * @param[in]  buff	数据包
  * @retval 	  0 校验失败，1校验通过
  */
uint8_t GY_Usart_Checksum(uint8_t* buff) {
	
	uint8_t sum = 0;
	for (uint8_t i = 0; i < GY_IMU_PACKAGE_LENGTH - 1; i++) {
		sum += buff[i];
	}
	
	if (sum == buff[GY_IMU_PACKAGE_LENGTH - 1]) {
		return 1;
	}
	else {
		return 0;
	}
}

/**
  * @brief      GY_IMU数据包校验
  * @param[in]  buff	数据包
  * @retval 	  0 校验失败，1校验通过
  */
void GY_Usart_IMU_getInfo(uint8_t* buff, uint16_t dataLength, GY_IMU_t* dataOut) {

	for (uint16_t n = 0; n < dataLength; ) {
		//匹配帧头
		if ((buff[n] == 0x5A) && (buff[n + 1] == 0x5A)) {
			if (GY_Usart_Checksum(&buff[n])) {
				int16_t DATA[6];
				DATA[0] = (buff[n + 4] << 8) | buff[n + 5];
				DATA[1] = (buff[n + 6] << 8) | buff[n + 7];
				DATA[2] = (buff[n + 8] << 8) | buff[n + 9];
				DATA[3] = (buff[n + 10] << 8) | buff[n + 11];
				DATA[4] = (buff[n + 12] << 8) | buff[n + 13];
				DATA[5] = (buff[n + 14] << 8) | buff[n + 15];

				dataOut->Gyro.x = (float)DATA[0] / 16;
				dataOut->Gyro.y = (float)DATA[1] / 16;
				dataOut->Gyro.z = (float)DATA[2] / 16;

				dataOut->Eular.Yaw = (float)((uint16_t)DATA[3]) / 100;
				dataOut->Eular.Roll = (float)DATA[4] / 100;
				dataOut->Eular.Pitch = (float)DATA[5] / 100;

				dataOut->ImuDevStatus = buff[n + 16];

				if (dataOut->Eular.Yaw - dataOut->lastYaw < -300) {
					dataOut->turnCount++;
				}

				if (dataOut->lastYaw - dataOut->Eular.Yaw < -300) {
					dataOut->turnCount--;
				}

				dataOut->lastYaw = dataOut->Eular.Yaw;

				dataOut->totalYaw = dataOut->Eular.Yaw + (360.0f * dataOut->turnCount);

				dataOut->ImuInfoUpdateFrame++;
				dataOut->ImuInfoUpdateFlag = 1;

				return;
			}
			else {
				n++;
			}
		}
		else {
			n++;
		}
	}
}
