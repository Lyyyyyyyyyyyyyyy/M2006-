#ifndef __CLOUD_CONTROL_H
#define __CLOUD_CONTROL_H

#include "user_common.h"

extern Cloud_t Cloud;

#define Cloud_LpfAttFactor    0.05f

#define Cloud_Yaw_Min			    4100.0f
#define Cloud_Yaw_Max			    5200.0f

void Cloud_Init(void);
void Set_GM6020_TargetAngle(float angle); 
 
#endif /* __CLOUD_CONTROL_H */
