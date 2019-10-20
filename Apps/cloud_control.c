/**
  ******************************************************************************
  * @file    cloud_control.c
  * @author   
  * @version  
  * @date    
  * @brief   云台控制函数接口
  ******************************************************************************
  */
  
#include "cloud_control.h"

Cloud_t Cloud;

/**
  * @brief  云台初始化，配置参数
  * @param  None
  * @retval None
  */
	
void Cloud_Init(void){
	
    Cloud.LpfAttFactor = Cloud_LpfAttFactor;
	
  	M6020s[0].targetAngle = 4700.0f;
	  M6020s[1].targetAngle = 2700.0f;
	
	  Cloud.targetPitchRaw = Cloud.targetPitchLPF = M6020s[0].totalAngle;
	  Cloud.targetYawRaw = Cloud.targetYawLPF = M6020s[1].totalAngle;
	
	  Cloud.IMUtargetYawRaw = Cloud.IMUtargetYawLPF = IMU_Cloud.totalYaw;
	  Cloud.IMUtargetPitchRaw = Cloud.IMUtargetPitchLPF = IMU_Cloud.Eular.Pitch;
		
	  /*-----正常使用PID-----*/
	  //云台电机PID
    PositionPID_paraReset(&M6020s[0].pid_Speed, 70.0f, 0.0f, 0.8f, 27000, 800);
	  PositionPID_paraReset(&M6020s[0].pid_Angle, 70.0f, 0.0f, 0.8f, 27000, 0);
	  
	  PositionPID_paraReset(&M6020s[1].pid_Speed, 70.0f, 0.0f, 0.8f, 27000, 100);
	  PositionPID_paraReset(&M6020s[1].pid_Angle, 70.0f, 0.0f, 0.8f, 30000, 0);
	
	  //陀螺仪双环PID
	  PositionPID_paraReset(&Cloud.YawAttitude_pid, 220.0f, 0.0f, 0.0f, 27000, 5000);
	  PositionPID_paraReset(&Cloud.YawSpeed_pid, 2.0f, 0.0f, 0.0f, 30000, 1000);      //发送给电机的最大控制值

	  PositionPID_paraReset(&Cloud.PitchAttitude_pid, 2.0f, 0.0f, 0.0f, 27000, 4000); 
	  PositionPID_paraReset(&Cloud.PitchSpeed_pid, 60.0f, 0.0f, 40.0f, 30000, 1000); //发送给电机的最大控制值
}

void Set_GM6020_TargetAngle(float angle)
{
	
	if (M6020s[1].M6020InfoUpdateFlag == 1)
	{
		if (M6020s[1].targetAngle <= Cloud_Yaw_Min)
		{
			 M6020s[1].targetAngle = Cloud_Yaw_Min;
		}
		else if (M6020s[1].targetAngle >= Cloud_Yaw_Max)
		{
			 M6020s[1].targetAngle = Cloud_Yaw_Max;
		}
	}

	for(int i = 0; i < 2; i++)
	{	
	  M6020s[i].targetAngle+=angle;
		
	  M6020s[i].outCurrent = Position_PID(&M6020s[i].pid_Angle, M6020s[i].targetAngle, M6020s[i].totalAngle);
	  M6020s[i].M6020InfoUpdateFlag = 0;
		
		
  }
	  M6020_setCurrent(M6020s[0].outCurrent, M6020s[1].outCurrent, 0, 0);
}
