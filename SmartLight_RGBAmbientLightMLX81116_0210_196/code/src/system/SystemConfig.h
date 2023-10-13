/*
 * system_config.h
 *
 *  Created on: 2019Äê5ÔÂ19ÈÕ
 *      Author: SH1705016
 */

#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

#include <stdint.h>

/*
typedef signed long int     int32_t;
typedef signed short int    int16_t;
typedef signed char         int8_t;
typedef unsigned long int   uint32_t;
typedef unsigned short int  uint16_t;
typedef unsigned char       uint8_t;
*/

/**@brief defines boolean type*/
typedef unsigned char       bool_t;
#define btrue               1/**<boolean type true.*/
#define bfalse              0/**<boolean type false.*/

/**@brief defines system tick type*/
typedef unsigned short int  tick_t;
#define DMAX_SYSTEM_TICK  (0xFFFF)/**<system tick type max value.*/

/**@brief This macro is used to comparing two ticks if a tick exceed or equal b tick.*/
#define DTickAfter(a, b) ((signed short)(b)-(signed short)(a)<=0)

/* RGB LED defines */
#define RGBLEDMAX 6u
#define RGBLEDCHANNELS 3u
#define NUMBOFSINGLELEDS (RGBLEDMAX*RGBLEDCHANNELS)

typedef enum{
  ESingleLedChannelR = 0x0,
  ESingleLedChannelG = 0x1,
  ESingleLedChannelB = 0x2,
  ESingleLedChannelMax = RGBLEDCHANNELS,
}ESingleLedChannel;

typedef struct{
  int16_t ledChannelData[ESingleLedChannelMax];
}SSingleLEDChannel;

#define DTEMP_MULTIPLIED_RATIO  10
#define DBAT_FILTER_LEVEL       3

#define PWMMAXPER 0x7E00u
#define CFL_MAXDUTY 0xFFFFu

#endif /* CODE_SRC_SYSTEM_CONFIG_H_ */
