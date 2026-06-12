/* -------------------------------------------------------------------
* @file main.h
* @brief main funtion (header)
*
* Purpose:
*    
*
* @section history_sec Revision History
* $Id: main.c, v 1.0 2018/09/13 $
*
* @verbatim
* -------------------------------------------------------------------
* Date     B/F Who Comment
* ------------------------------------------------------------------- 
* 2018/09/13 (F) JEFFERY.WU initial version
* -------------------------------------------------------------------
* 
*
* @creator      JEFFERY.WU
* @created      2018/09/13
* 
* @project      521.36
* @requested    -
* @inspector    - 
* @verification -
* @version      $Revision: V1.0 $
* 
* ------------------------------------------------------------------- 
*/
#ifndef MAIN_H
#define MAIN_H

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */


/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */

#include "SystemType.h"

/* ============================================================================ */
/* -------------------- FORWARD DECLARATIONS / PROTOTYPES --------------------- */
/* ============================================================================ */



/**
 * @brief Main entry point for the program, initializes the system and enters the main loop.
 * 
 * This function serves as the primary entry point for the system. It starts by disabling 
 * interrupts with `CMSIS_Irq_Dis`, followed by calling `SystemMain_Init` to initialize the system. 
 * After initialization, it enables interrupts again with `CMSIS_Irq_En` and then enters an 
 * infinite loop where `SystemMain_Loop` is called continuously to maintain system operations.
 * 
 * @return int32_t Return status code, currently unused.
 */
int32_t main(void);



#define LED_0	(LDRV->PWM_DC_CFG0.reg)		/*!< Define the PWM reg of RED Led >*/
#define LED_1	(LDRV->PWM_DC_CFG1.reg)		/*!< Define the PWM reg of BLUE Led >*/
#define LED_2	(LDRV->PWM_DC_CFG2.reg)		/*!< Define the PWM reg of GREEN Led >*/



#endif 
