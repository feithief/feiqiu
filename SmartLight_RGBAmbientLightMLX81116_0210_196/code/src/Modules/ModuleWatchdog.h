/*
 * watchdog.h
 *
 *  Created on: 2019Äê5ÔÂ19ÈÕ
 *      Author: SH1705016
 */

#ifndef _MODULE_WATCHDOG_H_
#define _MODULE_WATCHDOG_H_

#include <stdint.h>

void moduleWDTInit(void);
void moduleWDTStart(void);
void moduleWDTFeed(void);

#endif /* CODE_SRC_DRIVERS_WATCHDOG_H_ */
