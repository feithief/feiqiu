/**
 * @file
 * @brief Provides a set of definitions to link the Interrupt names and Amalthea IO_ports
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git flash 137e5489
 *
 * Melexis N.V. is supplying this code for use with Melexis N.V. processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.  MELEXIS N.V. SHALL NOT IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @endinternal
 *
 * @ingroup CAMCU_library
 *
 * @details
 */

#ifndef ITC_HELPER_H_
#define ITC_HELPER_H_

#include <io.h>

/** Set priority for an interrupt
 *
 * @param iname Name of the interrupt
 * @param val New priority for the interrupt
 */
#define  Itc_SetPrio(iname, val) IO_SET(MLX16, iname ## _PRIO, (val-3U) & 3U)

/** Enable interrupt
 *
 * @param iname Name of the interrupt
 */
#define  Itc_Enable(iname) IO_SET(MLX16, iname ## _ITC, 1)

/** Disable interrupt
 *
 * @param iname Name of the interrupt
 */
#define  Itc_Disable(iname) IO_SET(MLX16, iname ## _ITC, 0)

/** Clear the pending flag for an interrupt
 *
 * @param iname Name of the interrupt
 */
#define  Itc_Clear(iname) IO_SET(MLX16, iname ## _PEND, 1)

/** Check the pending flag of an interrupt
 *
 * @param iname Name of the interrupt
 * @retval True if the interrupt has pending flag set
 * @retval False otherwise
 */
#define  Itc_IsPending(iname) (IO_GET(MLX16, iname ## _PEND)!=0u)

#endif /* ITC_HELPER_H_ */
