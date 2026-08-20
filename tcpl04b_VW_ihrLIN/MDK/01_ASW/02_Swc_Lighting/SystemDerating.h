
#ifndef _SYSTEMDERATING_H_
#define _SYSTEMDERATING_H_

#include "SystemType.h"
#include "Algorithm.h"

void systemTempDerating(SColorParams *inputColor);

/**
*@details   get current TEMPSTATE level.
*
*@retval    TEMPSTATE level.
*/
uint8_t systemDeratingGetTempstate(void);


#endif
