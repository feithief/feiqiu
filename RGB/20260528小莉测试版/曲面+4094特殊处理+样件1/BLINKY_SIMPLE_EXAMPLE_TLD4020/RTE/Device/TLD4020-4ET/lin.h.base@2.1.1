/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/**
* \file     lin.h
*
* \brief    LIN/UART/Timer2 low level access library
*
* \version  V2.1.1
* \date     06. Oct 2025
*
*/

/** \addtogroup LIN_api
*  @{
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
** V2.0.3: 2024-07-11, VO:   [ATVCOMETLL-118] Fixed MISRA violations          **
** V2.0.4: 2024-11-28, VO:   [ATVCOMETLL-167] Added support for TLD4020-4ET   **
** V2.0.5: 2025-02-24, VO:   [ATVCOMETLL-165] Update error return code        **
** V2.0.6: 2025-04-02, VO:   [ATVCOMETLL-145] Added comp func and getter for  **
**                            shunt current                                   **
** V2.0.7: 2025-04-29, VO:   [ATVCOMETLL-190] Fix MISRA violations            **
** V2.0.8: 2025-06-17, VO:   [ATVCOMETLL-198] Refactored c99 int types        **
** V2.0.9: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V2.1.0: 2025-09-01, VO:   [ATVCOMETLL-205] Fix further MISRA violations    **
** V2.1.1: 2025-10-06, VO:   [ATVCOMETLL-226] Fix inaccurate rounding         **
*******************************************************************************/

#if !defined(_LIN_H_)
#define _LIN_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "variants.h"
#include "pmu.h"
#include "lin_defines.h"

uint8_t LIN_init(void);
uint8_t LIN_setUartBaudrate(uint32_t u32_baudrate);
uint32_t LIN_getUartBaudrate(void);
sint32 LIN_compensateAutoAddrResultDiff(sint32 s32_adcDiffValue);
uint32_t LIN_getAutoAddrIShunt_uA(void);

#if (LIN_UART_STD_EN == 1)
sint32 stdout_putchar(sint32 character);
sint32 stdin_getchar(void);
#endif

__STATIC_INLINE uint16_t LIN_getUartRXbuffer(void);
__STATIC_INLINE void LIN_setUartTXbuffer(uint16_t u16_value);
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
__STATIC_INLINE uint8_t LIN_getUartTXIntSts(void);
__STATIC_INLINE uint8_t LIN_getUartRXIntSts(void);
__STATIC_INLINE uint8_t LIN_getUartSyncErrIntSts(void);
__STATIC_INLINE uint8_t LIN_getUartEOFIntSts(void);
__STATIC_INLINE void LIN_clrUartTXIntSts(void);
__STATIC_INLINE void LIN_clrUartRXIntSts(void);
__STATIC_INLINE void LIN_clrUartSyncErrIntSts(void);
__STATIC_INLINE void LIN_clrUartEOFIntSts(void);


/*******************************************************************************
**                     Global Inline Function Definitions                     **
*******************************************************************************/

/** \brief Get the UART RX buffer value
 *
 * \return uint16_t UART RX buffer
*/
__STATIC_INLINE uint16_t LIN_getUartRXbuffer(void)
{
  LIN_clrUartRXIntSts();
  return (uint16_t)(LIN->UART_RXBUF.reg);
}

/** \brief Set the UART TX buffer value
 *
 * \param u16_value UART TX buffer
*/
__STATIC_INLINE void LIN_setUartTXbuffer(uint16_t u16_value)
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
  //lint --e{9034}
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
  //lint --e{9034}
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
 * \return uint8_t Empty Transmit Buffer Interrupt Status, LIN.LIN_IS.TI
 */
__STATIC_INLINE uint8_t LIN_getUartTXIntSts(void)
{
  return (uint8_t)LIN->LIN_IS.bit.TI;
}

/** \brief Get the UART Empty Receive Buffer Interrupt Status, LIN.LIN_IS.RI
 *
 * \return uint8_t Empty Receive Buffer Interrupt Status, LIN.LIN_IS.RI
 */
__STATIC_INLINE uint8_t LIN_getUartRXIntSts(void)
{
  return (uint8_t)LIN->LIN_IS.bit.RI;
}

/** \brief Get the UART Sync Error Interrupt Status, LIN.LIN_IS.ERRSYN
 *
 * \return uint8_t Sync Error Interrupt Status, LIN.LIN_IS.ERRSYN
 */
__STATIC_INLINE uint8_t LIN_getUartSyncErrIntSts(void)
{
  return (uint8_t)LIN->LIN_IS.bit.ERRSYN;
}

/** \brief Get the UART End of Sync Interrupt Status, LIN.LIN_IS.EOFSYN
 *
 * \return uint8_t End of Sync Interrupt Status,  LIN.LIN_IS.EOFSYN
 */
__STATIC_INLINE uint8_t LIN_getUartEOFIntSts(void)
{
  return (uint8_t)LIN->LIN_IS.bit.EOFSYN;
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

/** @}*/

#endif /*!defined(_LIN_H_)*/
