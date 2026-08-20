
#ifndef _SYSTEMDERATING_H_
#define _SYSTEMDERATING_H_

#include "Swc_Types.h"
#include "Algorithm.h"

void systemTempDerating(SColorParams *inputColor);

/**
*@details   get current TEMPSTATE level.
*
*@retval    TEMPSTATE level.
*/
uint8_t Swc_Derating_GetTempstate(void);


#endif
