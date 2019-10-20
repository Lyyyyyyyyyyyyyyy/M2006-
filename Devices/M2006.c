/**
  ******************************************************************************
  * @file    M2006.c
  * @author   
  * @version  
  * @date    
  * @brief   M2006无刷电机，配套C610电调驱动应用函数接口
  ******************************************************************************
  */

#include "M2006.h"

M2006s_t M2006s[1];

/**
  * @brief  设置M2006电机电流值（id号为7）
  * @param  iqx (x:5) 对应id号电机的电流值，范围-10000~0~10000
  * @retval None
  */
	
void M2006_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4){

	uint8_t data[8];
	
	//数据格式详见说明书P32
	data[0] = iq1 >> 8;
	data[1] = iq1;
	data[2] = iq2 >> 8;
	data[3] = iq2;
	data[4] = iq3 >> 8;
	data[5] = iq3;
	data[6] = iq4 >> 8;
	data[7] = iq4;
	
	CAN_SendData(CAN1, CAN_ID_STD, M2006_SENDID, data);
	
}	

/**
  * @brief  从CAN报文中获取M2006电机信息
  * @param  RxMessage 	CAN报文接收结构体
  * @retval None
  */

void M2006_getInfo(CanRxMsg RxMessage){
	//报文id确认
	if((RxMessage.StdId < M2006_READID_START) || (RxMessage.StdId > M2006_READID_END))
		return;
	
	uint32_t StdId;
	StdId = RxMessage.StdId - M2006_READID_START;
	
	//解包数据，数据格式详见C610电调说明书P33
	M2006s[StdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]);   //读回来的机械角度
	M2006s[StdId].realSpeed = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);    //读回来的速度
	M2006s[StdId].realCurrent = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5]);   //读回来的实际转矩
	M2006s[StdId].temperture = RxMessage.Data[6];
	
	//过零处理
	    if(M2006s[StdId].realAngle - M2006s[StdId].lastAngle < -6000){
		    M2006s[StdId].turnCount++;
			}

			if(M2006s[StdId].lastAngle - M2006s[StdId].realAngle < -6000){
				M2006s[StdId].turnCount--;
			}

			M2006s[StdId].lastAngle =  M2006s[StdId].realAngle;

			M2006s[StdId].totalAngle = M2006s[StdId].realAngle + (8192*M2006s[StdId].turnCount);

	//清零处理
//	if(	M2006s[StdId].totalAngle > 30000 || M2006s[StdId].totalAngle < -30000)
//	{
//		M2006s[StdId].totalAngle = M2006s[StdId].totalAngle-(8192*M2006s[StdId].turnCount);
//	  M2006s[StdId].targetAngle = M2006s[StdId].targetAngle -(8192*M2006s[StdId].turnCount);
//		M2006s[StdId].turnCount=0;
//	}
			
	//帧率统计，数据更新标志位
	M2006s[StdId].M2006InfoUpdateFrame++;
	M2006s[StdId].M2006InfoUpdateFlag = 1;
}	
