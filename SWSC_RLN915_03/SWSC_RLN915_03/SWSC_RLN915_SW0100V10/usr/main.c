//#include <appConfig.h>
//#include <systemInit.h>

#include "systemMain.h"

#include <pdsTask.h>
#include <validation.h>

#include "hwcfg.h"

void main(void)
{
    /* Enable trim revise access enable*/
    HWCFG_TrimAccessUnlock();
    CRGA_SFRS->MODULERSTREQ = 0xFFU;
    /* System init for hardwre init */
//    SYS_Init();
    systemInit();
    
    PDS_Init();
    
    
    /* system main infinite loop */
    for(;;)
    {
//        TM_RunTasks();
        systemMainLoop();
    }
}
