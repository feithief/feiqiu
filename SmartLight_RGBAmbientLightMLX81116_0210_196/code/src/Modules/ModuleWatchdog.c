#include "../Modules/ModuleWatchdog.h"

/*
 * watchdog.c
 *
 *  Created on: 2019��5��19��
 *      Author: SH1705016
 */

#include <lib_wdg.h>

void moduleWDTInit(void)
{
  //use absolute watch-dog, it will be activated at POR.
}

void moduleWDTStart(void)
{
  //use absolute watch-dog, it will be activated at POR.
}

void moduleWDTFeed(void)
{
  WDG_conditionalAwdRefresh();
}
