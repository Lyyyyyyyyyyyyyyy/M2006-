/**
  ******************************************************************************
  * @file    Chassis_control.c
  * @author   
  * @version  
  * @date    
  * @brief   ���̿��ƺ����ӿ�
  ******************************************************************************
  */
  
#include "chassis_control.h"

Chassis_t Chassis;

/**
  * @brief  ���̳�ʼ�������ò���
  * @param  None
  * @retval None
  */
void Chassis_Init(void){
	
  Chassis.LpfAttFactor = Chassis_LpfAttFactor;
	Chassis.speedLimit = Chassis_SpeedNormal;
	Chassis.mode = Chassis_Mode_Normal;
	Chassis.PowerOverflowFlag = 1;
	Chassis.swingSpeed = Chassis_SwingSpeed_Normal;
	
	/*-----����ʹ��PID-----*/
	//���̵��PID
	for(int i = 0; i < 4; i++){
	IncrementalPID_paraReset(&M3508s[i].pid, 3.0f, 0.3f, 0.0f, 8000, 1000);
	}
}	


/**
  * @brief  �����ķ���ٶȽ���
  * @param[in]  Vx	  	x���ٶ�
  *				      Vy		  y���ٶ�
  *			      	VOmega	��ת�ٶ�
  * @param[out]	Speed	�ٶ�
  * @retval None
  */
void MecanumCalculate(float Vx, float Vy, float VOmega, int16_t *Speed)
{
  float tempSpeed[4];
	float MaxSpeed = 0.0f;
	float Param = 1.0f;
	
	//�ٶ�����
	VAL_LIMIT(Vx, -Chassis_MaxSpeed_X, Chassis_MaxSpeed_X);  
	VAL_LIMIT(Vy, -Chassis_MaxSpeed_Y, Chassis_MaxSpeed_Y);  
	VAL_LIMIT(VOmega, -Chassis_MaxSpeed_W, Chassis_MaxSpeed_W);  
	
	//�����ٶȷֽ�
	tempSpeed[0] = Vx - Vy + VOmega;
	tempSpeed[1] = Vx + Vy + VOmega;
	tempSpeed[2] = -Vx + Vy + VOmega;
	tempSpeed[3] = -Vx - Vy + VOmega;
	
    //Ѱ������ٶ�
    for(uint8_t i = 0; i < 4; i++)
    {
        if(abs(tempSpeed[i]) > MaxSpeed)
        {
            MaxSpeed = abs(tempSpeed[i]);
        }
    }
	
	
	//�ٶȷ���
    if(MaxSpeed > WheelMaxSpeed)
    {
        Param = (float)WheelMaxSpeed / MaxSpeed;
    }
	
	Speed[0] = tempSpeed[0] * Param;
	Speed[1] = tempSpeed[1] * Param;
	Speed[2] = tempSpeed[2] * Param;
	Speed[3] = tempSpeed[3] * Param;
	
}


/**
  * @brief  ���̿��ƴ���(4��)
  * @param[in]  Vx		x���ٶ�
  *				      Vy		y���ٶ�
  *				      Omega	ƫ���
  * 		        mode	ģʽ - ��Status_ControlOFF�⣬������������
  * @retval None
  */
void Chassis_processing(float Vx, float Vy, float VOmega, uint8_t mode){
	
	if(mode == ControlMode_OFF){
		M3508_setCurrent(0, 0, 0, 0);
		return;
	}

	if(dr16_data.DR16OffLineFlag){
		Vx = Vy = VOmega = 0.0f;
	}
	
	//ƽ������
	Filter_IIRLPF(&Vx, &Chassis.targetXLPF, Chassis.LpfAttFactor);
	Filter_IIRLPF(&Vy, &Chassis.targetYLPF, Chassis.LpfAttFactor);
	Filter_IIRLPF(&VOmega, &Chassis.targetZLPF, Chassis.LpfAttFactor);
	
	int16_t speed[4];
	
	//���ֽ���
	MecanumCalculate(Chassis.targetXLPF, Chassis.targetYLPF, Chassis.targetZLPF, speed);
	
	for(int i = 0; i<4; i++){
    if(M3508s[i].M3508InfoUpdateFlag == 1){
			
			M3508s[i].targetSpeed = speed[i];
			//PID����
			M3508s[i].outCurrent = Incremental_PID(&M3508s[i].pid, M3508s[i].targetSpeed, M3508s[i].realSpeed); 
			//���־λ
			M3508s[i].M3508InfoUpdateFlag = 0;
		}
	}

	//�趨�������ֵ
	M3508_setCurrent(M3508s[0].outCurrent, M3508s[1].outCurrent, M3508s[2].outCurrent, M3508s[3].outCurrent);
	
}
