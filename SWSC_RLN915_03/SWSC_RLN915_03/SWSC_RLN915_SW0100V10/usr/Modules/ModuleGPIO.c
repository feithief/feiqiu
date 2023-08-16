/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModuleClock.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains functions that maintains system tick.
  *Others:      None.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */

#include "ModuleGPIO.h"

/* ============================================================================ */
/* ------------------------ Application Definition ---------------------------- */
/* ============================================================================ */

void moduleGPIOInit(void)
{
  GPIO_Init(GPIO_INIT_PORT_1,GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
  GPIO_Init(GPIO_INIT_PORT_2,GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
  GPIO_Init(GPIO_INIT_PORT_3,GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
  
}

