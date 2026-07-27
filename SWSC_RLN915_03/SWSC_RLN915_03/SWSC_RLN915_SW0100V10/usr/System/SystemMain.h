#ifndef _SYSTEMMAIN_H_
#define _SYSTEMMAIN_H_



/** @file
*
* @defgroup SYSTEM_STATUS system status management.
* @{
*
* @ingroup RGB_SYSTEM
*
* @brief This module manage the real-time LIN signal, system error and power status.
*
* @details This module manage the real-time LIN signal, system error and power status.
*/

/**@brief defines system main loop time period, because our LIN signal time is 10ms, we set 10ms as well.*/
#define DSYSTEM_EVENT_LOOP_PERIOD   10//10ms

/**
*@details   Initialize all system modules.
*
*@retval    None.
*/
void systemInit(void);

/**
*@details   System main loop function.
*
*@retval    None.
*/
void systemMainLoop(void);

/** @} */


#endif
