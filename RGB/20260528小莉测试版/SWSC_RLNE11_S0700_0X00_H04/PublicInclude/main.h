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
#ifndef _MAIN_H_
#define _MAIN_H_

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
//#include "lin_app.h"
//#include "vic_InterruptHandler.h" 

/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */

//#define eint() asm("eint") // Macros for H430 global IRQ enbale/disable
//#define dint() asm("dint")		//delete  	为了编译通过

/* ============================================================================ */
/* -------------------- FORWARD DECLARATIONS / PROTOTYPES --------------------- */
/* ============================================================================ */

int main(void);

#define LED_MAX_BRIGHTNESS 0xFFFF
#define LED_MIN_BRIGHTNESS 0x0000

#define LED_0	(LDRV->PWM_DC_CFG0.reg)
#define LED_1	(LDRV->PWM_DC_CFG1.reg)
#define LED_2	(LDRV->PWM_DC_CFG2.reg)

#ifndef LINT
/* LED0 is on if there is activity on the bus */
#define LED0_setHigh()          do{LED_0 = LED_MAX_BRIGHTNESS;}while(0)
#define LED0_setLow()           do{LED_0 = LED_MIN_BRIGHTNESS;}while(0)
#define LED0_toggle()           do{LED_0 ^= LED_MAX_BRIGHTNESS;}while(0)

/* LED1 toggles if a LIN message is sent sucessfully */
#define LED1_setHigh()          do{LED_1 = LED_MAX_BRIGHTNESS;}while(0)
#define LED1_setLow()           do{LED_1 = LED_MIN_BRIGHTNESS;}while(0)
#define LED1_toggle()           do{LED_1 ^= LED_MAX_BRIGHTNESS;}while(0)

/* LED2 is controlled by the corresponding LIN signal */
#define LED2_setHigh()          do{LED_2 = LED_MAX_BRIGHTNESS;}while(0)
#define LED2_setLow()           do{LED_2 = LED_MIN_BRIGHTNESS;}while(0)
#define LED2_toggle()           do{LED_2 ^= LED_MAX_BRIGHTNESS;}while(0)

#else /* else #ifndef LINT */

#define LED0_setHigh()
#define LED0_setLow()
#define LED0_toggle()

/* LED1 toggles if a LIN message is sent sucessfully */
#define LED1_setHigh()
#define LED1_setLow()
#define LED1_toggle()

/* LED2 is controlled by the corresponding LIN signal */
#define LED2_setHigh()
#define LED2_setLow()
#define LED2_toggle()

/*  DEBUG PIN 1 LED4 */
#define DEBUG1_setHigh()
#define DEBUG1_setLow()
#define DEBUG1_toggle()

/*  DEBUG PIN 1 LED5 */
#define DEBUG2_setHigh()
#define DEBUG2_setLow()
#define DEBUG2_toggle()

#endif /* end #ifndef LINT */


/* Function prototypes */
static void InitHw(void);
extern void LIN_COM_Handler(void);
extern void LIN_Task_1ms(void);


#endif 


