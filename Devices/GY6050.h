#ifndef __GY_6050_H
#define __GY_6050_H

#include "user_common.h"

#define GY_IMU_PACKAGE_LENGTH 	18
#define GY6050_SENDID		        0x414
#define Conversion ratio

typedef union{
	struct{
		uint16_t yaw;
		int16_t gyro_z;
		int16_t pitch;
		int16_t gyro_x;
	};
	uint8_t dataBuff[8];
}bno055_data_u;

extern GY_IMU_t IMU_Cloud;
extern GY_IMU_t GY_6050[2];

void GY6050_getCloundInfo(CanRxMsg RxMessage);
void GY_Usart_Init(uint32_t p_buff, uint32_t buffSize);
void GY_Usart_IMU_getInfo(uint8_t* buff, uint16_t dataLength, GY_IMU_t* dataOut);

#endif 

