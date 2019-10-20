/**
  ******************************************************************************
  * @file    Shoot.c(M2006_control.c)
  * @author   
  * @version V1.1
  * @date    
  * @brief   M2006测试例程
  ******************************************************************************
  */
  
  
#include "Shoot.h"

/**
  * @brief  M2006初始化，配置参数
  * @param  None
  * @retval None
  */
void Bullet_Init(void){
	
	M2006s[0].LpfAttFactor = M2006_LpfAttFactor;
	
	M2006s[0].targetAngle = 6800.0f;
	
	PositionPID_paraReset(&M2006s[0].pid_Speed, 3.0f, 0.0f, 0.0f, 10000, 2000);   //3.0f
	PositionPID_paraReset(&M2006s[0].pid_Angle, 0.2f, 0.0f, 0.0f, 8000, 2000);    //0.2f
 
	IncrementalPID_paraReset(&M2006s[0].pid, 3.0f, 0.3f, 0.0f, 8000, 1000);
}	

void ResetPill(void) {
	//清零处理。
	M2006s[0].totalAngle = M2006s[0].realAngle;
	M2006s[0].targetAngle = M2006s[0].totalAngle;
	M2006s[0].lastAngle = 0;
	M2006s[0].turnCount =  0;
}

/**
  * @brief      控制处理(M2006)双环
  * @param[in]  
  * @retval     None
  */
void M2006_DL(float Bullet_Angle){

	M2006s[0].targetAngle +=  Bullet_Angle;
	
	if(M2006s[0].targetAngle > 300000)
	{
		M2006s[0].targetAngle = 300000;
	}
	else if(M2006s[0].targetAngle < 2000)
	{
		M2006s[0].targetAngle = 2000;
	}

	M2006s[0].outCurrent = Position_PID(&M2006s[0].pid_Angle, M2006s[0].targetAngle, M2006s[0].totalAngle);
	M2006s[0].inneroutCurrent = Position_PID(&M2006s[0].pid_Speed, M2006s[0].outCurrent, M2006s[0].realSpeed);
	
	M2006s[0].M2006InfoUpdateFlag = 0;
	
	M2006_setCurrent(M2006s[0].inneroutCurrent, 0, 0, 0);
	
}

//单环（M2006)
void M2006_SR(float Speed){
	
	M2006s[0].targetSpeed = Speed;
	
	//PID计算
	M2006s[0].outCurrent = Incremental_PID(&M2006s[0].pid, M2006s[0].targetSpeed, M2006s[0].realSpeed); 
	//清标志位
	M2006s[0].M2006InfoUpdateFlag = 0;
	
	M2006_setCurrent(M2006s[0].outCurrent, 0, 0, 0);
		}

//	//清零防炸
//	if(M2006s[0].turnCount>100){
//		M2006s[0].addAngle -= (8192*M2006s[0].turnCount);
//		M2006s[0].turnCount = 0;

