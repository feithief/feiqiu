/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/
#include "isr_nvic_irq2_handler.h"


/* Check if NVIC node 2 is enabled */
#if ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN2_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN2_Pos))

/*******************************************************************************
**                        Global Variable Definitions                         **
*******************************************************************************/
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
  extern uint8_t u8_interrupt_cnt_irq2;
#endif

/*******************************************************************************
**                            Function Declarations                           **
*******************************************************************************/
#if (LIN_EXF2_INT_EN == 1u)
  void LIN_EXF2_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_TF2_INT_EN == 1u)
  void LIN_TF2_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_RX_INT_EN == 1u)
  void LIN_RX_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_TX_INT_EN == 1u)
  void LIN_TX_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_OC_INT_EN == 1u)
  void LIN_OC_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_OT_INT_EN == 1u)
  void LIN_OT_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_TMOUT_INT_EN == 1u)
  void LIN_TMOUT_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_EOF_INT_EN == 1u)
  void LIN_EOF_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_ERR_INT_EN == 1u)
  void LIN_ERR_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_AAMEASRDY_INT_EN == 1u)
  void LIN_AAMEASRDY_CALLBACK(void);
#endif /* Interrupt enabled */

#if (LIN_AAERR_INT_EN == 1u)
  void LIN_AAERR_CALLBACK(void);
#endif /* Interrupt enabled */

/*******************************************************************************
**                         Global Function Definitions                        **
*******************************************************************************/

#if ((LIN_EXF2_INT_EN == 1u) || \
     (LIN_TF2_INT_EN == 1u) || \
     (LIN_RX_INT_EN == 1u) || \
     (LIN_TX_INT_EN == 1u) || \
     (LIN_OC_INT_EN == 1u) || \
     (LIN_OT_INT_EN == 1u) || \
     (LIN_TMOUT_INT_EN == 1u) || \
     (LIN_EOF_INT_EN == 1u) || \
     (LIN_ERR_INT_EN == 1u) || \
     (LIN_AAMEASRDY_INT_EN == 1u) || \
     (LIN_AAERR_INT_EN == 1u))

/** \brief ISR from LIN/UART/Timer2
 */
void NVIC_IRQ2_Handler(void)
{
  /*{C8F8BC9D-2D0F-46c2-9E8D-00EAC1B0E384}*/
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
  u8_interrupt_cnt_irq2 = 0;
#endif
#if (LIN_EXF2_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.EXF2EN == LIN_LIN_IEN_EXF2EN_Enable)
  {
    if (LIN->LIN_IS.bit.EXF2 == LIN_LIN_IS_EXF2_Occurred)
    {
      LIN_EXF2_CALLBACK();
      LIN->LIN_ISC.bit.EXF2CLR = LIN_LIN_ISC_EXF2CLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_TF2_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.TF2EN == LIN_LIN_IEN_TF2EN_Enable)
  {
    if (LIN->LIN_IS.bit.TF2 == LIN_LIN_IS_TF2_Occurred)
    {
      LIN_TF2_CALLBACK();
      LIN->LIN_ISC.bit.TF2CLR = LIN_LIN_ISC_TF2CLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_RX_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.RIEN == LIN_LIN_IEN_RIEN_Enable)
  {
    if (LIN->LIN_IS.bit.RI == LIN_LIN_IS_RI_Occurred)
    {
      LIN_RX_CALLBACK();
      LIN->LIN_ISC.bit.RICLR = LIN_LIN_ISC_RICLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_TX_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.TIEN == LIN_LIN_IEN_TIEN_Enable)
  {
    if (LIN->LIN_IS.bit.TI == LIN_LIN_IS_TI_Occurred)
    {
      LIN_TX_CALLBACK();
      LIN->LIN_ISC.bit.TICLR = LIN_LIN_ISC_TICLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_OC_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.OC_IEN == LIN_LIN_IEN_OC_IEN_Enable)
  {
    if (LIN->LIN_IS.bit.OC_IS == LIN_LIN_IS_OC_IS_Occurred)
    {
      LIN_OC_CALLBACK();
      LIN->LIN_ISC.bit.OC_IS_CLR = LIN_LIN_ISC_OC_IS_CLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_OT_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.OT_IEN == LIN_LIN_IEN_OT_IEN_Enable)
  {
    if (LIN->LIN_IS.bit.OT_IS == LIN_LIN_IS_OT_IS_Occurred)
    {
      LIN_OT_CALLBACK();
      LIN->LIN_ISC.bit.OT_IS_CLR = LIN_LIN_ISC_OT_IS_CLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_TMOUT_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.TXD_TMOUT_IEN == LIN_LIN_IEN_TXD_TMOUT_IEN_Enable)
  {
    if (LIN->LIN_IS.bit.TXD_TMOUT_IS == LIN_LIN_IS_TXD_TMOUT_IS_Occurred)
    {
      LIN_TMOUT_CALLBACK();
      LIN->LIN_ISC.bit.TXD_TMOUT_IS_CLR = LIN_LIN_ISC_TXD_TMOUT_IS_CLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_EOF_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.EOFSYNEN == LIN_LIN_IEN_EOFSYNEN_Enable)
  {
    if (LIN->LIN_IS.bit.EOFSYN == LIN_LIN_IS_EOFSYN_Occurred)
    {
      LIN_EOF_CALLBACK();
      LIN->LIN_ISC.bit.EOFSYNCLR = LIN_LIN_ISC_EOFSYNCLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_ERR_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.ERRSYNEN == LIN_LIN_IEN_ERRSYNEN_Enable)
  {
    if (LIN->LIN_IS.bit.ERRSYN == LIN_LIN_IS_ERRSYN_Occurred)
    {
      LIN_ERR_CALLBACK();
      LIN->LIN_ISC.bit.ERRSYNCLR =  LIN_LIN_ISC_ERRSYNCLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_AAMEASRDY_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.AA_MEAS_RDY_IEN == LIN_LIN_IEN_AA_MEAS_RDY_IEN_Enable)
  {
    if (LIN->LIN_IS.bit.AA_MEAS_RDY == LIN_LIN_IS_AA_MEAS_RDY_Occurred)
    {
      LIN_AAMEASRDY_CALLBACK();
      LIN->LIN_ISC.bit.AA_MEAS_RDY_CLR =  LIN_LIN_ISC_AA_MEAS_RDY_CLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
#if (LIN_AAERR_INT_EN == 1u)

  if (LIN->LIN_IEN.bit.AA_ERR_IEN == LIN_LIN_IEN_AA_ERR_IEN_Enable)
  {
    if (LIN->LIN_IS.bit.AA_ERR == LIN_LIN_IS_AA_ERR_Occurred)
    {
      LIN_AAERR_CALLBACK();
      LIN->LIN_ISC.bit.AA_ERR_CLR =  LIN_LIN_ISC_AA_ERR_CLR_Cleared;
#if ((defined NVIC_IRQ2_HANDLER_INT_CHECK) && (NVIC_IRQ2_HANDLER_INT_CHECK == 1))
      u8_interrupt_cnt_irq2 += 1u;
#endif
    }
  }

#endif /* Interrupt enabled */
}

#endif /* ((LIN_EXF2_INT_EN == 1u) || 
          (LIN_TF2_INT_EN == 1u) || \
          (LIN_RX_INT_EN == 1u) || \
          (LIN_TX_INT_EN == 1u) || \
          (LIN_OC_INT_EN == 1u) || \
          (LIN_OT_INT_EN == 1u) || \
          (LIN_TMOUT_INT_EN == 1u) || \
          (LIN_EOF_INT_EN == 1u) || \
          (LIN_ERR_INT_EN == 1u) || \
          (LIN_AAMEASRDY_INT_EN == 1u) || \
          (LIN_AAERR_INT_EN == 1u)) */

#endif /* ((CPU_NVIC_ISER0 & CPU_NVIC_ISER0_SETENA_IRQEN2_Msk) == (1u << CPU_NVIC_ISER0_SETENA_IRQEN2_Pos)) */



