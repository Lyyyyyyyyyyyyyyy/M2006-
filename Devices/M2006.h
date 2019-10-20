#ifndef __M2006_H
#define __M2006_H

#include "user_common.h"

#define M2006_READID_START	 0x201
#define M2006_READID_END	   0x201
#define M2006_SENDID		     0x200   

extern M2006s_t M2006s[1];

void M2006_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void M2006_getInfo(CanRxMsg RxMessage);

#endif /* __M3508_H */
