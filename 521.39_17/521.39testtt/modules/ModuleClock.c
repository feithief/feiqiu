/***************************************************************************//**
 * @file      mod_swtimer.c
 *
 * @creator   poe
 * @created   2021.05.04 
 *
 * @brief     Example for SW Timer usage.
 *
 * @purpose   Simple implementation example for usage of the swtimer module.
 *            
 * 
 *
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the 
 * integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 * $Id: mod_swtimer.c 8848 2021-05-10 14:11:24Z poe $
 *
 * $Revision: 8848 $
 *
 ******************************************************************************/

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_helper.h"
#include "ModuleClock.h"
#include "swtimer_InterruptHandler.h"
#include "LinDrvImp_Config.h"
#include "CommonInterruptHandler.h"
/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/
static const unsigned char SWT_CLKDIVS[] = {239, 119, 79, 39};
static swtimer_sInterruptEnvironmentData_t swtimer_envData;
/* ****************************************************************************/
/* ************************** FORWARD DECLARATIONS ****************************/
/* ****************************************************************************/
vic_cpInterfaceFunctions_t vic_IfFuns; /* VIC Interface ptr. */
/*brief This variant contains current system ticks.*/
tick_t currentTick = 0;

/*IRQ Handler for SwTimer0 Cnt_Zero event. */
static void swtimer_tickHandler(swtimer_eInterruptVectorNum_t irqvecnum, swtimer_pInterruptContextData_t contextdata)
{   
    moduleClockISR(0);
}


/* ****************************************************************************
 * @implementation
 * Initialize global system settings
 *
 * @param   none
 * @return  FALSE,  if initialization fails
 *          TRUE ,  else
 * *************************************************************************** */
bool_t __moduleClockSysInit(void)
{
  /* Set system clock to 24MHz SysClock & 24MHz PWM clock. */
  SYS_STATE_CONTROL = 0x0000u; 
  
  /* Get VIC interface form SysROM. */
  if(romIf_MainInterface->Interface_Get(ROMIF_API_VIC, (romIf_cpGenericInterface_t *) &vic_IfFuns, NULL) != TRUE)
  {
    /* goto safe state */
    while(1);
  }
  /* Initialize VIC */
  InterruptHandlerInitialize(vic_IfFuns);  
    
  return TRUE;
}


/********************************************************************************
*@details   system Tick interrupt service routing.
*
*@retval    None.
********************************************************************************/
void moduleClockISR(int sno)
{
  currentTick+=2;
//  SWTIMER_IRQ_VNO = 2; //clear interrupt
}


/* SwTimer initialization. */
bool_t moduleClockInit1(vic_cpInterfaceFunctions_t vicIf)
{
  /* Set default return value. */
  bool_t retVal = FALSE;


  /* VIC IF ptr plausible? */
  if (vicIf != NULL)
  {
    //set swtimer clock to 100kHz
    SWTIMER_DIV0_RELOAD = SWT_CLKDIVS[(SYS_STATE_CONTROL & 0x03u)]; 
    SWTIMER_CNT0_RELOAD = 199u; //=2ms
    
  
    /* Initialise swtimer interrupt handling. */
    swtimer_InterruptInitialisation(vicIf, &swtimer_envData, (void*)NULL);
    /* Register Swtimer irq handler. */
    swtimer_InterruptRegisterCallback(swtimer_IRQ_EVT_CNT_ZERO_0, &swtimer_tickHandler);
    
    /* Enable Cnt_Zero_event for Timer0. */
    SWTIMER_IRQ_MASK |= (1L << (0x0Fu & (uint16_t)(swtimer_IRQ_EVT_CNT_ZERO_0)));
    
    SWTIMER_CONFIG |= 0x05;
    
    retVal = TRUE;
  }
  else { /* Error: invalid vic if ptr! */ }
    
  return retVal;
}


bool_t moduleClockInit(void)
{
    bool_t retVal = FALSE;
    __moduleClockSysInit();
    retVal = moduleClockInit1(vic_IfFuns);
    return retVal;
}

/********************************************************************************
*@details   Get current clock tick(Basicly, we set 1 tick equals 1ms).
*
*@retval    Current tick.
********************************************************************************/
tick_t moduleClockGet(void)
{
  tick_t temp;
  temp = currentTick;
  return temp;
}