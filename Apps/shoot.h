#ifndef __SHOOT_H
#define __SHOOT_H

#include "user_common.h"

#define M2006_LpfAttFactor     0.05f

void Bullet_Init(void);
void M2006_SR(float Speed);
void M2006_DL(float Bullet_Angle);
void ResetPill(void);
	
void Bullet_processing(float Bullet_Angle);

#endif /* __SHOOT_H */
