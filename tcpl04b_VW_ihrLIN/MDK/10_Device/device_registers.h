/**
 *****************************************************************************
 * @brief   device registers header file.
 *
 * @file    device_registers.h
 * @author  AE/FAE team
 * @date    2026.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2026 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __DEVICE_REGISTERS_H__
#define __DEVICE_REGISTERS_H__

#include <stdint.h>
#include <stdbool.h>

#if defined(__TCPL01X__)

#define TCPL01x_SERIES

/* specific core definitions */
#include "tcpl01x/inc/tcpl01x.h"
#include "tcpl01x/inc/system_tcpl01x.h"

/* CPU specific feature definitions */
// #include "tcpl01x/inc/tcpl01x_features.h"

#elif defined(__TCPL03X__)

#define TCPL03x_SERIES

/* specific core definitions */
#include "tcpl03x/inc/tcpl03x.h"
#include "tcpl03x/inc/system_tcpl03x.h"

/* CPU specific feature definitions */
// #include "tcpl03x/inc/tcpl03x_features.h"

#elif defined(__TCPL04X__)

#define TCPL04x_SERIES

/* specific core definitions */
#include "tcpl04x.h"
#include "system_tcpl04x.h"
/* CPU specific feature definitions */
// #include "tcpl04x/inc/tcpl04x_features.h"

#elif (defined(__TCPL08X__) || defined(__TCPL18X__))

#define TCPL08x_SERIES

/* specific core definitions */
#include "tcpl08x/inc/tcpl08x.h"
#include "tcpl08x/inc/system_tcpl08x.h"

/* CPU specific feature definitions */
#if defined(__TCPL08X__)
#include "tcpl08x/inc/tcpl08x_features.h"
#else
#include "tcpl18x/inc/tcpl18x_features.h"
#endif

#else
#error "No valid CPU defined!"
#endif


#endif /* __DEVICE_REGISTERS_H__ */
