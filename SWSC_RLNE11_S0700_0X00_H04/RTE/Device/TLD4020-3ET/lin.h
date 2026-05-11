/*
 ***********************************************************************************************************************
 *
 * Copyright (c) 2022-2023, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * This software is provided by the copyright holders and contributors "as is" and any express or implied warranties,
 * including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are
 * disclaimed. In no event shall the copyright holder or contributors be liable  for any direct, indirect, incidental,
 * special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or
 * services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability,
 * whether in contract, strict liability,or tort (including negligence or otherwise) arising in any way out  of the
 * use of this software, even if advised of the possibility of such damage.
 *
 **********************************************************************************************************************/

 
 /**
 * \file     lin.h
 *
 * \brief    LIN/UART/Timer2 low level access library
 *
 * \version  V2.0.2
 * \date     28. Sep 2023
 *
 */

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** CG           Christopher Gabriel                                           **
** SL           Stefano Lissandron                                            **
** VO           Vanessa Ongaro                                                **
** JO           Julia Ott                                                     **
*******************************************************************************/


/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V2.0.0: 2023-02-13, SL:   Added HAL and support for UART; correction of    **
**                            the init sequence; bugfix on the init error     **
**                            generation; adaption to the new LIN_UARTCLOCK   **
**                            now in Hz                                       **
** V2.0.1: 2023-08-31, VO:   [ATVCOMETLL-105] Fixed wrong function names for  **
**                            En/Dis RX Int                                   **
** V2.0.2: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
*******************************************************************************/
 
#if !defined(_LIN_H_)
#define _LIN_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "tld40xx.h"
#include "pmu.h"
#include "lin_defines.h"

sint8 LIN_init(void);
sint8 LIN_setUartBaudrate(uint32 u32_baudrate);
uint32 LIN_getUartBaudrate(void);

#if (LIN_UART_STD_EN == 1)
  sint32 stdout_putchar(sint32 character);
  sint32 stdin_getchar(void);
#endif

__STATIC_INLINE uint16 LIN_getUartRXbuffer(void);
__STATIC_INLINE void LIN_setUartTXbuffer(uint16 u16_value);
__STATIC_INLINE bool LIN_isUartByteReceived(void);
__STATIC_INLINE bool LIN_isUartByteTransmitted(void);
__STATIC_INLINE void LIN_enUartBaudrateGen(void);
__STATIC_INLINE void LIN_disUartBaudrateGen(void);
__STATIC_INLINE void LIN_startUartTX(void);
__STATIC_INLINE void LIN_enUartTXInt(void);
__STATIC_INLINE void LIN_enUartRXInt(void);
__STATIC_INLINE void LIN_enUartSyncErrInt(void);
__STATIC_INLINE void LIN_enUartEOSInt(void);
__STATIC_INLINE void LIN_disUartTXInt(void);
__STATIC_INLINE void LIN_disUartRXInt(void);
__STATIC_INLINE void LIN_disUartSyncErrInt(void);
__STATIC_INLINE void LIN_disUartEOSInt(void);
__STATIC_INLINE uint8 LIN_getUartTXIntSts(void);
__STATIC_INLINE uint8 LIN_getUartRXIntSts(void);
__STATIC_INLINE uint8 LIN_getUartSyncErrIntSts(void);
__STATIC_INLINE uint8 LIN_getUartEOFIntSts(void);
__STATIC_INLINE void LIN_clrUartTXIntSts(void);
__STATIC_INLINE void LIN_clrUartRXIntSts(void);
__STATIC_INLINE void LIN_clrUartSyncErrIntSts(void);
__STATIC_INLINE void LIN_clrUartEOFIntSts(void);


/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/

/** \brief Get the UART RX buffer value
 *
 * \return uint16 UART RX buffer
*/
__STATIC_INLINE uint16 LIN_getUartRXbuffer(void)
{
  LIN_clrUartRXIntSts();
  return (uint16)(LIN->UART_RXBUF.reg);
}

/** \brief Set the UART TX buffer value
 *
 * \param u16_value UART TX buffer
*/
__STATIC_INLINE void LIN_setUartTXbuffer(uint16 u16_value)
{
  LIN_clrUartTXIntSts();
  LIN->UART_TXBUF.reg = u16_value;
}

/** \brief Check if UART has received a byte
 *
 * \return true, a byte was received OR false, a byte was not received
*/
__STATIC_INLINE bool LIN_isUartByteReceived(void)
{
  bool result = false;

  if (LIN_getUartRXIntSts() == 1u)
  {
    result = true;
  }

  return result;
}

/** \brief Check if UART has transmitted a byte
 *
 * \return true, a byte was transmitted OR false, a byte was not transmitted
*/
__STATIC_INLINE bool LIN_isUartByteTransmitted(void)
{
  bool result = false;

  if (LIN_getUartTXIntSts() == 1u)
  {
    result = true;
  }

  return result;
}

/** \brief Enable the baudrate generator, LIN.UART_BCON.BR_R
 */
__STATIC_INLINE void LIN_enUartBaudrateGen(void)
{
  LIN->UART_BCON.bit.BR_R = 1u;
}

/** \brief Disable the baudrate generator, LIN.UART_BCON.BR_R
 */
__STATIC_INLINE void LIN_disUartBaudrateGen(void)
{
  LIN->UART_BCON.bit.BR_R = 0u;
}

/** \brief Start the transmission, LIN.UART_TSTART.TXSTART
 */
__STATIC_INLINE void LIN_startUartTX(void)
{
  LIN->UART_TSTART.bit.TXSTART = 1u;
}

/** \brief Enable UART Transmit Interrupt
 */
__STATIC_INLINE void LIN_enUartTXInt(void)
{
  LIN->LIN_IEN.bit.TIEN = 1u;
}

/** \brief Enable UART Receive Interrupt
 */
__STATIC_INLINE void LIN_enUartRXInt(void)
{
  LIN->LIN_IEN.bit.RIEN = 1u;
}

/** \brief Enable UART Sync Error Interrupt
 */
__STATIC_INLINE void LIN_enUartSyncErrInt(void)
{
  LIN->LIN_IEN.bit.ERRSYNEN = 1u;
}

/** \brief Enable UART End of Sync Interrupt
 */
__STATIC_INLINE void LIN_enUartEOSInt(void)
{
  LIN->LIN_IEN.bit.EOFSYNEN = 1u;
}

/** \brief Disable UART Transmit Interrupt
 */
__STATIC_INLINE void LIN_disUartTXInt(void)
{
  LIN->LIN_IEN.bit.TIEN = 0u;
}

/** \brief Disable UART Receive Interrupt
 */
__STATIC_INLINE void LIN_disUartRXInt(void)
{
  LIN->LIN_IEN.bit.RIEN = 0u;
}

/** \brief Disable UART Sync Error Interrupt
 */
__STATIC_INLINE void LIN_disUartSyncErrInt(void)
{
  LIN->LIN_IEN.bit.ERRSYNEN = 0u;
}

/** \brief Disable UART End of Sync Interrupt
 */
__STATIC_INLINE void LIN_disUartEOSInt(void)
{
  LIN->LIN_IEN.bit.EOFSYNEN = 0u;
}

/** \brief Get the UART Empty Transmit Buffer Interrupt Status, LIN.LIN_IS.TI
 *
 * \return uint8 Empty Transmit Buffer Interrupt Status, LIN.LIN_IS.TI
 */
__STATIC_INLINE uint8 LIN_getUartTXIntSts(void)
{
  return (uint8)LIN->LIN_IS.bit.TI;
}

/** \brief Get the UART Empty Receive Buffer Interrupt Status, LIN.LIN_IS.RI
 *
 * \return uint8 Empty Receive Buffer Interrupt Status, LIN.LIN_IS.RI
 */
__STATIC_INLINE uint8 LIN_getUartRXIntSts(void)
{
  return (uint8)LIN->LIN_IS.bit.RI;
}

/** \brief Get the UART Sync Error Interrupt Status, LIN.LIN_IS.ERRSYN
 *
 * \return uint8 Sync Error Interrupt Status, LIN.LIN_IS.ERRSYN
 */
__STATIC_INLINE uint8 LIN_getUartSyncErrIntSts(void)
{
  return (uint8)LIN->LIN_IS.bit.ERRSYN;
}

/** \brief Get the UART End of Sync Interrupt Status, LIN.LIN_IS.EOFSYN
 *
 * \return uint8 End of Sync Interrupt Status,  LIN.LIN_IS.EOFSYN
 */
__STATIC_INLINE uint8 LIN_getUartEOFIntSts(void)
{
  return (uint8)LIN->LIN_IS.bit.EOFSYN;
}

/** \brief Clear the UART Empty Transmit Buffer Interrupt Status, LIN.LIN_ISC.TICLR
 */
__STATIC_INLINE void LIN_clrUartTXIntSts(void)
{
  LIN->LIN_ISC.bit.TICLR = 1u;
}

/** \brief Clear the UART Empty Receive Buffer Interrupt Status, LIN.LIN_ISC.RICLR
 */
__STATIC_INLINE void LIN_clrUartRXIntSts(void)
{
  LIN->LIN_ISC.bit.RICLR = 1u;
}

/** \brief Clear the UART Sync Error Interrupt Status, LIN.LIN_ISC.ERRSYNCLR
 */
__STATIC_INLINE void LIN_clrUartSyncErrIntSts(void)
{
  LIN->LIN_ISC.bit.ERRSYNCLR = 1u;
}

/** \brief Clear the UART End of Sync Interrupt Status, LIN.LIN_ISC.EOFSYNCLR
 */
__STATIC_INLINE void LIN_clrUartEOFIntSts(void)
{
  LIN->LIN_ISC.bit.EOFSYNCLR = 1u;
}

#ifdef __cplusplus
}
#endif


#endif /*!defined(_LIN_H_)*/
