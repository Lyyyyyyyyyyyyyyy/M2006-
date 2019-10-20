#include "user_code.h"

void Init(void)
{
	CAN1_QuickInit();
	Bullet_Init();
	DR16_receiverInit();  
	USART2_QuickInit(9600);
	TIM6_CounterMode(89, 9999);
	
}

void Test(void)
{
//	  ANO_Send_Data_V1(-M3508s[0].targetSpeed, -M3508s[0].realSpeed,M3508s[1].targetSpeed,M3508s[1].realSpeed);
//		ANO_Send_Data_V2(-M3508s[0].targetSpeed, -M3508s[0].realSpeed,M3508s[1].targetSpeed,M3508s[1].realSpeed);
//		ANO_Send_Data_V3(-M3508s[0].targetSpeed, -M3508s[0].realSpeed,M3508s[1].targetSpeed,M3508s[1].realSpeed);
}
