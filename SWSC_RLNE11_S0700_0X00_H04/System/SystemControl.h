#ifndef _SYSTEMCONTROL_H_
#define _SYSTEMCONTROL_H_
#include "Algorithm.h"

extern bool_t colorDimmingFlag;
typedef struct
{
	uint8_t dimStatus;
	uint8_t currentDimPriority;
	uint16_t actualIntensity;
	uint32_t x_Diff_ms;
	//uint32_t brightnessBarrier;
	uint32_t lastActualIntensity;
	uint32_t currentX;
	uint32_t y_Diff;
	tick_t lastLoopSysTick;
	uint8_t dimmingStateMachineInit;
}IntensityDimStatus;

#define X00_DIMIDLE (0u)
#define X01_DIMUP (1u)
#define X02_DIMIDOWN (2u)
#define X00_DIMPRIORITY_IDLE (0u)
#define X01_DIMPRIORITY_WAKEUP (1u)
#define X02_DIMPRIORITY_NORMAL (2u)
#define X03_DIMPRIORITY_SLEEP (3u)
#define HANDWRITE_INTERPOLATE

extern IntensityDimStatus intensityDimStatus;
/** @file
 *
*
* @defgroup SYSTEM_CONTROL system control module.
* @{
*
* @ingroup RGB_SYSTEM
*
* @brief This module controls RGB LIN module.
*
* @details This module controls RGB LIN module.
*/


/**
*@details   Controls color.
*
*@params[in&out] inputColor input and output Color signal @SColorParams
*
*@retval    None.
*/
void systemControl(SColorParams *inputColor);

/**
*@details   Control module prepare to enter sleep.
*
*@retval    None.
*/
void systemControlSleep(void);

void systemControl_reInitDimmingStateMachine(void);

void reinitColorDimming(void);
/** @} */
PWMs32_t* SystemCtrl_GetPWMAfterCtrl(void);
#endif
