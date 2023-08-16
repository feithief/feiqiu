/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModuleWatchdog.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.24
  *Description: This file contains basic functions of watchdog.
  *History:
     1.Date:         2017.10.24
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "ModuleWatchdog.h"
#include "SystemType.h"

#include "wdt_device.h"
/* ============================================================================ */
/* --------------------------- Application Functions--------------------------- */
/* ============================================================================ */
/********************************************************************************
*@details   Initialize watch dog hardware. Timeout time is 100ms for now.
*
*@warning   This function must be invoked before @moduleWatchdogFeed.
*
*@retval    None.
********************************************************************************/
void moduleWatchdogInit(void)
{
#if 1
    WDTA_Enable(WDTA_INTERVAL_1S); /* 1s */
#endif
}

/*******************************************************************************
*@details   Feed dog.
*
*@retval    None.
*******************************************************************************/
void moduleWatchdogFeed(void)
{
#if 1
    WDTA_Clear();
#endif
}
