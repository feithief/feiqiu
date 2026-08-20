/**
 *****************************************************************************
 * @brief   lin_sci_uart_reg_def header file.
 *
 * @file    lin_sci_uart_reg_def.h
 * @author  AE/FAE team
 * @date    2024.09.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2024 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __LIN_SCI1_DEF_H__
#define __LIN_SCI1_DEF_H__

// Register Offset Macro Definition
#define LIN_SCI1_TX_DATA_OFFSET                             0x0000
#define LIN_SCI1_RX_DATA_OFFSET                             0x0004
#define LIN_SCI1_CTRL_OFFSET                                0x0008
#define LIN_SCI1_CTRL_UART_OFFSET                           0x000C
#define LIN_SCI1_BAUD_CFG_OFFSET                            0x0014
#define LIN_SCI1_BRK_SYNC_CFG_OFFSET                        0x0018
#define LIN_SCI1_TX_CFG_OFFSET                              0x001C
#define LIN_SCI1_RX_CFG_OFFSET                              0x0020
#define LIN_SCI1_RX_FILTER_CFG_OFFSET                       0x0024
#define LIN_SCI1_STATUS_OFFSET                              0x0028
#define LIN_SCI1_AUTO_BAUD_VAL_OFFSET                       0x002C
#define LIN_SCI1_RX_PID_OFFSET                              0x0030
#define LIN_SCI1_CHKSUM_VAL_OFFSET                          0x0034
#define LIN_SCI1_ICR_OFFSET                                 0x005C
#define LIN_SCI1_IMR_OFFSET                                 0x0060
#define LIN_SCI1_ISR_OFFSET                                 0x0064
#define LIN_SCI1_IRSR_OFFSET                                0x0068

// Register Address Macro Definition
#define LIN_SCI1_TX_DATA_ADDR                               ( LIN_SCI1_BASE_ADDR + LIN_SCI1_TX_DATA_OFFSET)
#define LIN_SCI1_RX_DATA_ADDR                               ( LIN_SCI1_BASE_ADDR + LIN_SCI1_RX_DATA_OFFSET)
#define LIN_SCI1_CTRL_ADDR                                  ( LIN_SCI1_BASE_ADDR + LIN_SCI1_CTRL_OFFSET)
#define LIN_SCI1_CTRL_UART_ADDR                             ( LIN_SCI1_BASE_ADDR + LIN_SCI1_CTRL_UART_OFFSET)
#define LIN_SCI1_BAUD_CFG_ADDR                              ( LIN_SCI1_BASE_ADDR + LIN_SCI1_BAUD_CFG_OFFSET)
#define LIN_SCI1_BRK_SYNC_CFG_ADDR                          ( LIN_SCI1_BASE_ADDR + LIN_SCI1_BRK_SYNC_CFG_OFFSET)
#define LIN_SCI1_TX_CFG_ADDR                                ( LIN_SCI1_BASE_ADDR + LIN_SCI1_TX_CFG_OFFSET)
#define LIN_SCI1_RX_CFG_ADDR                                ( LIN_SCI1_BASE_ADDR + LIN_SCI1_RX_CFG_OFFSET)
#define LIN_SCI1_RX_FILTER_CFG_ADDR                         ( LIN_SCI1_BASE_ADDR + LIN_SCI1_RX_FILTER_CFG_OFFSET)
#define LIN_SCI1_STATUS_ADDR                                ( LIN_SCI1_BASE_ADDR + LIN_SCI1_STATUS_OFFSET)
#define LIN_SCI1_AUTO_BAUD_VAL_ADDR                         ( LIN_SCI1_BASE_ADDR + LIN_SCI1_AUTO_BAUD_VAL_OFFSET)
#define LIN_SCI1_RX_PID_ADDR                                ( LIN_SCI1_BASE_ADDR + LIN_SCI1_RX_PID_OFFSET)
#define LIN_SCI1_CHKSUM_VAL_ADDR                            ( LIN_SCI1_BASE_ADDR + LIN_SCI1_CHKSUM_VAL_OFFSET)
#define LIN_SCI1_ICR_ADDR                                   ( LIN_SCI1_BASE_ADDR + LIN_SCI1_ICR_OFFSET)
#define LIN_SCI1_IMR_ADDR                                   ( LIN_SCI1_BASE_ADDR + LIN_SCI1_IMR_OFFSET)
#define LIN_SCI1_ISR_ADDR                                   ( LIN_SCI1_BASE_ADDR + LIN_SCI1_ISR_OFFSET)
#define LIN_SCI1_IRSR_ADDR                                  ( LIN_SCI1_BASE_ADDR + LIN_SCI1_IRSR_OFFSET)

// Register Field Macro Definition
#define LIN_SCI1_TX_DATA_TX_DATA_SHIFT                                                0
#define LIN_SCI1_TX_DATA_TX_DATA_MASK                                                 0x000000FF
#define LIN_SCI1_TX_DATA_TX_DATA_SET(n)                                               (((uint32_t)(n) << 0  ) & 0x000000FF)
#define LIN_SCI1_TX_DATA_TX_DATA_CLR                                                  0xFFFFFF00

#define LIN_SCI1_RX_DATA_RX_DATA_SHIFT                                                0
#define LIN_SCI1_RX_DATA_RX_DATA_MASK                                                 0x000000FF
#define LIN_SCI1_RX_DATA_RX_DATA_SET(n)                                               (((uint32_t)(n) << 0  ) & 0x000000FF)
#define LIN_SCI1_RX_DATA_RX_DATA_CLR                                                  0xFFFFFF00

#define LIN_SCI1_CTRL_BIT_ERR_DET_EN_SHIFT                                            30
#define LIN_SCI1_CTRL_BIT_ERR_DET_EN_MASK                                             0x40000000
#define LIN_SCI1_CTRL_BIT_ERR_DET_EN_SET(n)                                           (((uint32_t)(n) << 30 ) & 0x40000000)
#define LIN_SCI1_CTRL_BIT_ERR_DET_EN_CLR                                              0xBFFFFFFF

#define LIN_SCI1_CTRL_SHORT_GND_DET_EN_SHIFT                                          29
#define LIN_SCI1_CTRL_SHORT_GND_DET_EN_MASK                                           0x20000000
#define LIN_SCI1_CTRL_SHORT_GND_DET_EN_SET(n)                                         (((uint32_t)(n) << 29 ) & 0x20000000)
#define LIN_SCI1_CTRL_SHORT_GND_DET_EN_CLR                                            0xDFFFFFFF

#define LIN_SCI1_CTRL_LPBK_MODE_SHIFT                                                 27
#define LIN_SCI1_CTRL_LPBK_MODE_MASK                                                  0x18000000
#define LIN_SCI1_CTRL_LPBK_MODE_SET(n)                                                (((uint32_t)(n) << 27 ) & 0x18000000)
#define LIN_SCI1_CTRL_LPBK_MODE_CLR                                                   0xE7FFFFFF

#define LIN_SCI1_CTRL_RX_FIFO_CLR_SHIFT                                               26
#define LIN_SCI1_CTRL_RX_FIFO_CLR_MASK                                                0x04000000
#define LIN_SCI1_CTRL_RX_FIFO_CLR_SET(n)                                              (((uint32_t)(n) << 26 ) & 0x04000000)
#define LIN_SCI1_CTRL_RX_FIFO_CLR_CLR                                                 0xFBFFFFFF

#define LIN_SCI1_CTRL_TX_FIFO_CLR_SHIFT                                               25
#define LIN_SCI1_CTRL_TX_FIFO_CLR_MASK                                                0x02000000
#define LIN_SCI1_CTRL_TX_FIFO_CLR_SET(n)                                              (((uint32_t)(n) << 25 ) & 0x02000000)
#define LIN_SCI1_CTRL_TX_FIFO_CLR_CLR                                                 0xFDFFFFFF

#define LIN_SCI1_CTRL_RX_ABORT_SHIFT                                                  24
#define LIN_SCI1_CTRL_RX_ABORT_MASK                                                   0x01000000
#define LIN_SCI1_CTRL_RX_ABORT_SET(n)                                                 (((uint32_t)(n) << 24 ) & 0x01000000)
#define LIN_SCI1_CTRL_RX_ABORT_CLR                                                    0xFEFFFFFF

#define LIN_SCI1_CTRL_TX_ABORT_SHIFT                                                  23
#define LIN_SCI1_CTRL_TX_ABORT_MASK                                                   0x00800000
#define LIN_SCI1_CTRL_TX_ABORT_SET(n)                                                 (((uint32_t)(n) << 23 ) & 0x00800000)
#define LIN_SCI1_CTRL_TX_ABORT_CLR                                                    0xFF7FFFFF

#define LIN_SCI1_CTRL_RX_MSK_SHIFT                                                    22
#define LIN_SCI1_CTRL_RX_MSK_MASK                                                     0x00400000
#define LIN_SCI1_CTRL_RX_MSK_SET(n)                                                   (((uint32_t)(n) << 22 ) & 0x00400000)
#define LIN_SCI1_CTRL_RX_MSK_CLR                                                      0xFFBFFFFF

#define LIN_SCI1_CTRL_TX_MSK_SHIFT                                                    20
#define LIN_SCI1_CTRL_TX_MSK_MASK                                                     0x00300000
#define LIN_SCI1_CTRL_TX_MSK_SET(n)                                                   (((uint32_t)(n) << 20 ) & 0x00300000)
#define LIN_SCI1_CTRL_TX_MSK_CLR                                                      0xFFCFFFFF

#define LIN_SCI1_CTRL_TX_NUM_SHIFT                                                    16
#define LIN_SCI1_CTRL_TX_NUM_MASK                                                     0x000F0000
#define LIN_SCI1_CTRL_TX_NUM_SET(n)                                                   (((uint32_t)(n) << 16 ) & 0x000F0000)
#define LIN_SCI1_CTRL_TX_NUM_CLR                                                      0xFFF0FFFF

#define LIN_SCI1_CTRL_RX_NUM_SHIFT                                                    12
#define LIN_SCI1_CTRL_RX_NUM_MASK                                                     0x0000F000
#define LIN_SCI1_CTRL_RX_NUM_SET(n)                                                   (((uint32_t)(n) << 12 ) & 0x0000F000)
#define LIN_SCI1_CTRL_RX_NUM_CLR                                                      0xFFFF0FFF

#define LIN_SCI1_CTRL_TX_WAIT_FIFO_VLD_EN_SHIFT                                       10
#define LIN_SCI1_CTRL_TX_WAIT_FIFO_VLD_EN_MASK                                        0x00000400
#define LIN_SCI1_CTRL_TX_WAIT_FIFO_VLD_EN_SET(n)                                      (((uint32_t)(n) << 10 ) & 0x00000400)
#define LIN_SCI1_CTRL_TX_WAIT_FIFO_VLD_EN_CLR                                         0xFFFFFBFF

#define LIN_SCI1_CTRL_MASTER_EN_SHIFT                                                 9
#define LIN_SCI1_CTRL_MASTER_EN_MASK                                                  0x00000200
#define LIN_SCI1_CTRL_MASTER_EN_SET(n)                                                (((uint32_t)(n) << 9  ) & 0x00000200)
#define LIN_SCI1_CTRL_MASTER_EN_CLR                                                   0xFFFFFDFF

#define LIN_SCI1_CTRL_RX_NUM_MODE_SHIFT                                               8
#define LIN_SCI1_CTRL_RX_NUM_MODE_MASK                                                0x00000100
#define LIN_SCI1_CTRL_RX_NUM_MODE_SET(n)                                              (((uint32_t)(n) << 8  ) & 0x00000100)
#define LIN_SCI1_CTRL_RX_NUM_MODE_CLR                                                 0xFFFFFEFF

#define LIN_SCI1_CTRL_TX_NUM_MODE_SHIFT                                               7
#define LIN_SCI1_CTRL_TX_NUM_MODE_MASK                                                0x00000080
#define LIN_SCI1_CTRL_TX_NUM_MODE_SET(n)                                              (((uint32_t)(n) << 7  ) & 0x00000080)
#define LIN_SCI1_CTRL_TX_NUM_MODE_CLR                                                 0xFFFFFF7F

#define LIN_SCI1_CTRL_CHKSUM_TYPE_SHIFT                                               6
#define LIN_SCI1_CTRL_CHKSUM_TYPE_MASK                                                0x00000040
#define LIN_SCI1_CTRL_CHKSUM_TYPE_SET(n)                                              (((uint32_t)(n) << 6  ) & 0x00000040)
#define LIN_SCI1_CTRL_CHKSUM_TYPE_CLR                                                 0xFFFFFFBF

#define LIN_SCI1_CTRL_CHKSUM_EN_SHIFT                                                 5
#define LIN_SCI1_CTRL_CHKSUM_EN_MASK                                                  0x00000020
#define LIN_SCI1_CTRL_CHKSUM_EN_SET(n)                                                (((uint32_t)(n) << 5  ) & 0x00000020)
#define LIN_SCI1_CTRL_CHKSUM_EN_CLR                                                   0xFFFFFFDF

#define LIN_SCI1_CTRL_BRK_TX_TRIG_SHIFT                                               4
#define LIN_SCI1_CTRL_BRK_TX_TRIG_MASK                                                0x00000010
#define LIN_SCI1_CTRL_BRK_TX_TRIG_SET(n)                                              (((uint32_t)(n) << 4  ) & 0x00000010)
#define LIN_SCI1_CTRL_BRK_TX_TRIG_CLR                                                 0xFFFFFFEF

#define LIN_SCI1_CTRL_AUTO_BAUD_EN_SHIFT                                              3
#define LIN_SCI1_CTRL_AUTO_BAUD_EN_MASK                                               0x00000008
#define LIN_SCI1_CTRL_AUTO_BAUD_EN_SET(n)                                             (((uint32_t)(n) << 3  ) & 0x00000008)
#define LIN_SCI1_CTRL_AUTO_BAUD_EN_CLR                                                0xFFFFFFF7

#define LIN_SCI1_CTRL_RX_EN_SHIFT                                                     2
#define LIN_SCI1_CTRL_RX_EN_MASK                                                      0x00000004
#define LIN_SCI1_CTRL_RX_EN_SET(n)                                                    (((uint32_t)(n) << 2  ) & 0x00000004)
#define LIN_SCI1_CTRL_RX_EN_CLR                                                       0xFFFFFFFB

#define LIN_SCI1_CTRL_TX_EN_SHIFT                                                     1
#define LIN_SCI1_CTRL_TX_EN_MASK                                                      0x00000002
#define LIN_SCI1_CTRL_TX_EN_SET(n)                                                    (((uint32_t)(n) << 1  ) & 0x00000002)
#define LIN_SCI1_CTRL_TX_EN_CLR                                                       0xFFFFFFFD

#define LIN_SCI1_CTRL_GLB_EN_SHIFT                                                    0
#define LIN_SCI1_CTRL_GLB_EN_MASK                                                     0x00000001
#define LIN_SCI1_CTRL_GLB_EN_SET(n)                                                   (((uint32_t)(n) << 0  ) & 0x00000001)
#define LIN_SCI1_CTRL_GLB_EN_CLR                                                      0xFFFFFFFE

#define LIN_SCI1_CTRL_UART_MP_RX_ADDR_WR_EN_SHIFT                                     8
#define LIN_SCI1_CTRL_UART_MP_RX_ADDR_WR_EN_MASK                                      0x00000100
#define LIN_SCI1_CTRL_UART_MP_RX_ADDR_WR_EN_SET(n)                                    (((uint32_t)(n) << 8  ) & 0x00000100)
#define LIN_SCI1_CTRL_UART_MP_RX_ADDR_WR_EN_CLR                                       0xFFFFFEFF

#define LIN_SCI1_CTRL_UART_MP_TX_ADDR_DATA_SEL_SHIFT                                  7
#define LIN_SCI1_CTRL_UART_MP_TX_ADDR_DATA_SEL_MASK                                   0x00000080
#define LIN_SCI1_CTRL_UART_MP_TX_ADDR_DATA_SEL_SET(n)                                 (((uint32_t)(n) << 7  ) & 0x00000080)
#define LIN_SCI1_CTRL_UART_MP_TX_ADDR_DATA_SEL_CLR                                    0xFFFFFF7F

#define LIN_SCI1_CTRL_UART_MP_MODE_EN_SHIFT                                           6
#define LIN_SCI1_CTRL_UART_MP_MODE_EN_MASK                                            0x00000040
#define LIN_SCI1_CTRL_UART_MP_MODE_EN_SET(n)                                          (((uint32_t)(n) << 6  ) & 0x00000040)
#define LIN_SCI1_CTRL_UART_MP_MODE_EN_CLR                                             0xFFFFFFBF

#define LIN_SCI1_CTRL_UART_PTY_MODE_SHIFT                                             4
#define LIN_SCI1_CTRL_UART_PTY_MODE_MASK                                              0x00000030
#define LIN_SCI1_CTRL_UART_PTY_MODE_SET(n)                                            (((uint32_t)(n) << 4  ) & 0x00000030)
#define LIN_SCI1_CTRL_UART_PTY_MODE_CLR                                               0xFFFFFFCF

#define LIN_SCI1_CTRL_UART_STP_BIT_SEL_SHIFT                                          2
#define LIN_SCI1_CTRL_UART_STP_BIT_SEL_MASK                                           0x00000004
#define LIN_SCI1_CTRL_UART_STP_BIT_SEL_SET(n)                                         (((uint32_t)(n) << 2  ) & 0x00000004)
#define LIN_SCI1_CTRL_UART_STP_BIT_SEL_CLR                                            0xFFFFFFFB

#define LIN_SCI1_CTRL_UART_PTY_CHK_EN_SHIFT                                           1
#define LIN_SCI1_CTRL_UART_PTY_CHK_EN_MASK                                            0x00000002
#define LIN_SCI1_CTRL_UART_PTY_CHK_EN_SET(n)                                          (((uint32_t)(n) << 1  ) & 0x00000002)
#define LIN_SCI1_CTRL_UART_PTY_CHK_EN_CLR                                             0xFFFFFFFD

#define LIN_SCI1_CTRL_UART_MODE_SHIFT                                                 0
#define LIN_SCI1_CTRL_UART_MODE_MASK                                                  0x00000001
#define LIN_SCI1_CTRL_UART_MODE_SET(n)                                                (((uint32_t)(n) << 0  ) & 0x00000001)
#define LIN_SCI1_CTRL_UART_MODE_CLR                                                   0xFFFFFFFE

#define LIN_SCI1_BAUD_CFG_BD_FRAC_SHIFT                                               12
#define LIN_SCI1_BAUD_CFG_BD_FRAC_MASK                                                0x0000F000
#define LIN_SCI1_BAUD_CFG_BD_FRAC_SET(n)                                              (((uint32_t)(n) << 12 ) & 0x0000F000)
#define LIN_SCI1_BAUD_CFG_BD_FRAC_CLR                                                 0xFFFF0FFF

#define LIN_SCI1_BAUD_CFG_BD_INTR_SHIFT                                               0
#define LIN_SCI1_BAUD_CFG_BD_INTR_MASK                                                0x000001FF
#define LIN_SCI1_BAUD_CFG_BD_INTR_SET(n)                                              (((uint32_t)(n) << 0  ) & 0x000001FF)
#define LIN_SCI1_BAUD_CFG_BD_INTR_CLR                                                 0xFFFFFE00

#define LIN_SCI1_BRK_SYNC_CFG_SHORT_GND_THD_SHIFT                                     13
#define LIN_SCI1_BRK_SYNC_CFG_SHORT_GND_THD_MASK                                      0x00002000
#define LIN_SCI1_BRK_SYNC_CFG_SHORT_GND_THD_SET(n)                                    (((uint32_t)(n) << 13 ) & 0x00002000)
#define LIN_SCI1_BRK_SYNC_CFG_SHORT_GND_THD_CLR                                       0xFFFFDFFF

#define LIN_SCI1_BRK_SYNC_CFG_BRK_THD_SHIFT                                           12
#define LIN_SCI1_BRK_SYNC_CFG_BRK_THD_MASK                                            0x00001000
#define LIN_SCI1_BRK_SYNC_CFG_BRK_THD_SET(n)                                          (((uint32_t)(n) << 12 ) & 0x00001000)
#define LIN_SCI1_BRK_SYNC_CFG_BRK_THD_CLR                                             0xFFFFEFFF

#define LIN_SCI1_BRK_SYNC_CFG_DLT_NUM_SHIFT                                           10
#define LIN_SCI1_BRK_SYNC_CFG_DLT_NUM_MASK                                            0x00000C00
#define LIN_SCI1_BRK_SYNC_CFG_DLT_NUM_SET(n)                                          (((uint32_t)(n) << 10 ) & 0x00000C00)
#define LIN_SCI1_BRK_SYNC_CFG_DLT_NUM_CLR                                             0xFFFFF3FF

#define LIN_SCI1_BRK_SYNC_CFG_BRK_NUM_SHIFT                                           0
#define LIN_SCI1_BRK_SYNC_CFG_BRK_NUM_MASK                                            0x000003FF
#define LIN_SCI1_BRK_SYNC_CFG_BRK_NUM_SET(n)                                          (((uint32_t)(n) << 0  ) & 0x000003FF)
#define LIN_SCI1_BRK_SYNC_CFG_BRK_NUM_CLR                                             0xFFFFFC00

#define LIN_SCI1_TX_CFG_TX_PID_SHIFT                                                  8
#define LIN_SCI1_TX_CFG_TX_PID_MASK                                                   0x0000FF00
#define LIN_SCI1_TX_CFG_TX_PID_SET(n)                                                 (((uint32_t)(n) << 8  ) & 0x0000FF00)
#define LIN_SCI1_TX_CFG_TX_PID_CLR                                                    0xFFFF00FF

#define LIN_SCI1_TX_CFG_TX_RSP_SPACE_SHIFT                                            4
#define LIN_SCI1_TX_CFG_TX_RSP_SPACE_MASK                                             0x00000030
#define LIN_SCI1_TX_CFG_TX_RSP_SPACE_SET(n)                                           (((uint32_t)(n) << 4  ) & 0x00000030)
#define LIN_SCI1_TX_CFG_TX_RSP_SPACE_CLR                                              0xFFFFFFCF

#define LIN_SCI1_TX_CFG_CHK_PT_SEL_SHIFT                                              3
#define LIN_SCI1_TX_CFG_CHK_PT_SEL_MASK                                               0x00000008
#define LIN_SCI1_TX_CFG_CHK_PT_SEL_SET(n)                                             (((uint32_t)(n) << 3  ) & 0x00000008)
#define LIN_SCI1_TX_CFG_CHK_PT_SEL_CLR                                                0xFFFFFFF7

#define LIN_SCI1_TX_CFG_TX_BYTE_SPACE_SHIFT                                           1
#define LIN_SCI1_TX_CFG_TX_BYTE_SPACE_MASK                                            0x00000006
#define LIN_SCI1_TX_CFG_TX_BYTE_SPACE_SET(n)                                          (((uint32_t)(n) << 1  ) & 0x00000006)
#define LIN_SCI1_TX_CFG_TX_BYTE_SPACE_CLR                                             0xFFFFFFF9

#define LIN_SCI1_TX_CFG_SW_TXD_VAL_SHIFT                                              0
#define LIN_SCI1_TX_CFG_SW_TXD_VAL_MASK                                               0x00000001
#define LIN_SCI1_TX_CFG_SW_TXD_VAL_SET(n)                                             (((uint32_t)(n) << 0  ) & 0x00000001)
#define LIN_SCI1_TX_CFG_SW_TXD_VAL_CLR                                                0xFFFFFFFE

#define LIN_SCI1_RX_CFG_RX_FIFO_VLD_ENTRY_SHIFT                                       16
#define LIN_SCI1_RX_CFG_RX_FIFO_VLD_ENTRY_MASK                                        0x00030000
#define LIN_SCI1_RX_CFG_RX_FIFO_VLD_ENTRY_SET(n)                                      (((uint32_t)(n) << 16 ) & 0x00030000)
#define LIN_SCI1_RX_CFG_RX_FIFO_VLD_ENTRY_CLR                                         0xFFFCFFFF

#define LIN_SCI1_RX_CFG_MP_SLAVE_ADDR_MSK_SHIFT                                       8
#define LIN_SCI1_RX_CFG_MP_SLAVE_ADDR_MSK_MASK                                        0x00000100
#define LIN_SCI1_RX_CFG_MP_SLAVE_ADDR_MSK_SET(n)                                      (((uint32_t)(n) << 8  ) & 0x00000100)
#define LIN_SCI1_RX_CFG_MP_SLAVE_ADDR_MSK_CLR                                         0xFFFFFEFF

#define LIN_SCI1_RX_CFG_MP_SLAVE_ADDR_SHIFT                                           0
#define LIN_SCI1_RX_CFG_MP_SLAVE_ADDR_MASK                                            0x000000FF
#define LIN_SCI1_RX_CFG_MP_SLAVE_ADDR_SET(n)                                          (((uint32_t)(n) << 0  ) & 0x000000FF)
#define LIN_SCI1_RX_CFG_MP_SLAVE_ADDR_CLR                                             0xFFFFFF00

#define LIN_SCI1_RX_FILTER_CFG_RX_FILTER_TIM_SHIFT                                    0
#define LIN_SCI1_RX_FILTER_CFG_RX_FILTER_TIM_MASK                                     0x000001FF
#define LIN_SCI1_RX_FILTER_CFG_RX_FILTER_TIM_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x000001FF)
#define LIN_SCI1_RX_FILTER_CFG_RX_FILTER_TIM_CLR                                      0xFFFFFE00

#define LIN_SCI1_STATUS_RXD_VAL_ANA_SHIFT                                             8
#define LIN_SCI1_STATUS_RXD_VAL_ANA_MASK                                              0x00000100
#define LIN_SCI1_STATUS_RXD_VAL_ANA_SET(n)                                            (((uint32_t)(n) << 8  ) & 0x00000100)
#define LIN_SCI1_STATUS_RXD_VAL_ANA_CLR                                               0xFFFFFEFF

#define LIN_SCI1_STATUS_RX_SYNC_STATE_SHIFT                                           7
#define LIN_SCI1_STATUS_RX_SYNC_STATE_MASK                                            0x00000080
#define LIN_SCI1_STATUS_RX_SYNC_STATE_SET(n)                                          (((uint32_t)(n) << 7  ) & 0x00000080)
#define LIN_SCI1_STATUS_RX_SYNC_STATE_CLR                                             0xFFFFFF7F

#define LIN_SCI1_STATUS_RX_BRK_STATE_SHIFT                                            6
#define LIN_SCI1_STATUS_RX_BRK_STATE_MASK                                             0x00000040
#define LIN_SCI1_STATUS_RX_BRK_STATE_SET(n)                                           (((uint32_t)(n) << 6  ) & 0x00000040)
#define LIN_SCI1_STATUS_RX_BRK_STATE_CLR                                              0xFFFFFFBF

#define LIN_SCI1_STATUS_TX_STATE_SHIFT                                                5
#define LIN_SCI1_STATUS_TX_STATE_MASK                                                 0x00000020
#define LIN_SCI1_STATUS_TX_STATE_SET(n)                                               (((uint32_t)(n) << 5  ) & 0x00000020)
#define LIN_SCI1_STATUS_TX_STATE_CLR                                                  0xFFFFFFDF

#define LIN_SCI1_STATUS_RX_STATE_SHIFT                                                4
#define LIN_SCI1_STATUS_RX_STATE_MASK                                                 0x00000010
#define LIN_SCI1_STATUS_RX_STATE_SET(n)                                               (((uint32_t)(n) << 4  ) & 0x00000010)
#define LIN_SCI1_STATUS_RX_STATE_CLR                                                  0xFFFFFFEF

#define LIN_SCI1_STATUS_RX_FIFO_EMPTY_SHIFT                                           3
#define LIN_SCI1_STATUS_RX_FIFO_EMPTY_MASK                                            0x00000008
#define LIN_SCI1_STATUS_RX_FIFO_EMPTY_SET(n)                                          (((uint32_t)(n) << 3  ) & 0x00000008)
#define LIN_SCI1_STATUS_RX_FIFO_EMPTY_CLR                                             0xFFFFFFF7

#define LIN_SCI1_STATUS_TX_FIFO_FULL_SHIFT                                            2
#define LIN_SCI1_STATUS_TX_FIFO_FULL_MASK                                             0x00000004
#define LIN_SCI1_STATUS_TX_FIFO_FULL_SET(n)                                           (((uint32_t)(n) << 2  ) & 0x00000004)
#define LIN_SCI1_STATUS_TX_FIFO_FULL_CLR                                              0xFFFFFFFB

#define LIN_SCI1_STATUS_TXD_VAL_SHIFT                                                 1
#define LIN_SCI1_STATUS_TXD_VAL_MASK                                                  0x00000002
#define LIN_SCI1_STATUS_TXD_VAL_SET(n)                                                (((uint32_t)(n) << 1  ) & 0x00000002)
#define LIN_SCI1_STATUS_TXD_VAL_CLR                                                   0xFFFFFFFD

#define LIN_SCI1_STATUS_RXD_VAL_SHIFT                                                 0
#define LIN_SCI1_STATUS_RXD_VAL_MASK                                                  0x00000001
#define LIN_SCI1_STATUS_RXD_VAL_SET(n)                                                (((uint32_t)(n) << 0  ) & 0x00000001)
#define LIN_SCI1_STATUS_RXD_VAL_CLR                                                   0xFFFFFFFE

#define LIN_SCI1_AUTO_BAUD_VAL_AUTO_BD_FRAC_SHIFT                                     12
#define LIN_SCI1_AUTO_BAUD_VAL_AUTO_BD_FRAC_MASK                                      0x0000F000
#define LIN_SCI1_AUTO_BAUD_VAL_AUTO_BD_FRAC_SET(n)                                    (((uint32_t)(n) << 12 ) & 0x0000F000)
#define LIN_SCI1_AUTO_BAUD_VAL_AUTO_BD_FRAC_CLR                                       0xFFFF0FFF

#define LIN_SCI1_AUTO_BAUD_VAL_AUTO_BD_INTR_SHIFT                                     0
#define LIN_SCI1_AUTO_BAUD_VAL_AUTO_BD_INTR_MASK                                      0x000001FF
#define LIN_SCI1_AUTO_BAUD_VAL_AUTO_BD_INTR_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x000001FF)
#define LIN_SCI1_AUTO_BAUD_VAL_AUTO_BD_INTR_CLR                                       0xFFFFFE00

#define LIN_SCI1_RX_PID_RX_PID_SHIFT                                                  0
#define LIN_SCI1_RX_PID_RX_PID_MASK                                                   0x000000FF
#define LIN_SCI1_RX_PID_RX_PID_SET(n)                                                 (((uint32_t)(n) << 0  ) & 0x000000FF)
#define LIN_SCI1_RX_PID_RX_PID_CLR                                                    0xFFFFFF00

#define LIN_SCI1_CHKSUM_VAL_CHKSUM_VAL_SHIFT                                          0
#define LIN_SCI1_CHKSUM_VAL_CHKSUM_VAL_MASK                                           0x000000FF
#define LIN_SCI1_CHKSUM_VAL_CHKSUM_VAL_SET(n)                                         (((uint32_t)(n) << 0  ) & 0x000000FF)
#define LIN_SCI1_CHKSUM_VAL_CHKSUM_VAL_CLR                                            0xFFFFFF00

#define LIN_SCI1_ICR_RX_FIFO_VLD_INT_CLR_SHIFT                                        22
#define LIN_SCI1_ICR_RX_FIFO_VLD_INT_CLR_MASK                                         0x00400000
#define LIN_SCI1_ICR_RX_FIFO_VLD_INT_CLR_SET(n)                                       (((uint32_t)(n) << 22 ) & 0x00400000)
#define LIN_SCI1_ICR_RX_FIFO_VLD_INT_CLR_CLR                                          0xFFBFFFFF

#define LIN_SCI1_ICR_SHORT_GND_DET_INT_CLR_SHIFT                                      21
#define LIN_SCI1_ICR_SHORT_GND_DET_INT_CLR_MASK                                       0x00200000
#define LIN_SCI1_ICR_SHORT_GND_DET_INT_CLR_SET(n)                                     (((uint32_t)(n) << 21 ) & 0x00200000)
#define LIN_SCI1_ICR_SHORT_GND_DET_INT_CLR_CLR                                        0xFFDFFFFF

#define LIN_SCI1_ICR_RX_DATA_DONE_INT_CLR_SHIFT                                       20
#define LIN_SCI1_ICR_RX_DATA_DONE_INT_CLR_MASK                                        0x00100000
#define LIN_SCI1_ICR_RX_DATA_DONE_INT_CLR_SET(n)                                      (((uint32_t)(n) << 20 ) & 0x00100000)
#define LIN_SCI1_ICR_RX_DATA_DONE_INT_CLR_CLR                                         0xFFEFFFFF

#define LIN_SCI1_ICR_SLV_TX_BRK_DONE_INT_CLR_SHIFT                                    19
#define LIN_SCI1_ICR_SLV_TX_BRK_DONE_INT_CLR_MASK                                     0x00080000
#define LIN_SCI1_ICR_SLV_TX_BRK_DONE_INT_CLR_SET(n)                                   (((uint32_t)(n) << 19 ) & 0x00080000)
#define LIN_SCI1_ICR_SLV_TX_BRK_DONE_INT_CLR_CLR                                      0xFFF7FFFF

#define LIN_SCI1_ICR_TX_1BYTE_DONE_INT_CLR_SHIFT                                      18
#define LIN_SCI1_ICR_TX_1BYTE_DONE_INT_CLR_MASK                                       0x00040000
#define LIN_SCI1_ICR_TX_1BYTE_DONE_INT_CLR_SET(n)                                     (((uint32_t)(n) << 18 ) & 0x00040000)
#define LIN_SCI1_ICR_TX_1BYTE_DONE_INT_CLR_CLR                                        0xFFFBFFFF

#define LIN_SCI1_ICR_MP_MODE_ADDR_INT_CLR_SHIFT                                       15
#define LIN_SCI1_ICR_MP_MODE_ADDR_INT_CLR_MASK                                        0x00008000
#define LIN_SCI1_ICR_MP_MODE_ADDR_INT_CLR_SET(n)                                      (((uint32_t)(n) << 15 ) & 0x00008000)
#define LIN_SCI1_ICR_MP_MODE_ADDR_INT_CLR_CLR                                         0xFFFF7FFF

#define LIN_SCI1_ICR_TX_RX_CONF_INT_CLR_SHIFT                                         14
#define LIN_SCI1_ICR_TX_RX_CONF_INT_CLR_MASK                                          0x00004000
#define LIN_SCI1_ICR_TX_RX_CONF_INT_CLR_SET(n)                                        (((uint32_t)(n) << 14 ) & 0x00004000)
#define LIN_SCI1_ICR_TX_RX_CONF_INT_CLR_CLR                                           0xFFFFBFFF

#define LIN_SCI1_ICR_TX_PID_DONE_INT_CLR_SHIFT                                        13
#define LIN_SCI1_ICR_TX_PID_DONE_INT_CLR_MASK                                         0x00002000
#define LIN_SCI1_ICR_TX_PID_DONE_INT_CLR_SET(n)                                       (((uint32_t)(n) << 13 ) & 0x00002000)
#define LIN_SCI1_ICR_TX_PID_DONE_INT_CLR_CLR                                          0xFFFFDFFF

#define LIN_SCI1_ICR_SYNC_VAL_ERR_INT_CLR_SHIFT                                       12
#define LIN_SCI1_ICR_SYNC_VAL_ERR_INT_CLR_MASK                                        0x00001000
#define LIN_SCI1_ICR_SYNC_VAL_ERR_INT_CLR_SET(n)                                      (((uint32_t)(n) << 12 ) & 0x00001000)
#define LIN_SCI1_ICR_SYNC_VAL_ERR_INT_CLR_CLR                                         0xFFFFEFFF

#define LIN_SCI1_ICR_SYNC_DET_INT_CLR_SHIFT                                           11
#define LIN_SCI1_ICR_SYNC_DET_INT_CLR_MASK                                            0x00000800
#define LIN_SCI1_ICR_SYNC_DET_INT_CLR_SET(n)                                          (((uint32_t)(n) << 11 ) & 0x00000800)
#define LIN_SCI1_ICR_SYNC_DET_INT_CLR_CLR                                             0xFFFFF7FF

#define LIN_SCI1_ICR_BRK_DET_INT_CLR_SHIFT                                            10
#define LIN_SCI1_ICR_BRK_DET_INT_CLR_MASK                                             0x00000400
#define LIN_SCI1_ICR_BRK_DET_INT_CLR_SET(n)                                           (((uint32_t)(n) << 10 ) & 0x00000400)
#define LIN_SCI1_ICR_BRK_DET_INT_CLR_CLR                                              0xFFFFFBFF

#define LIN_SCI1_ICR_TX_FIFO_EMPTY_INT_CLR_SHIFT                                      9
#define LIN_SCI1_ICR_TX_FIFO_EMPTY_INT_CLR_MASK                                       0x00000200
#define LIN_SCI1_ICR_TX_FIFO_EMPTY_INT_CLR_SET(n)                                     (((uint32_t)(n) << 9  ) & 0x00000200)
#define LIN_SCI1_ICR_TX_FIFO_EMPTY_INT_CLR_CLR                                        0xFFFFFDFF

#define LIN_SCI1_ICR_TX_DONE_INT_CLR_SHIFT                                            8
#define LIN_SCI1_ICR_TX_DONE_INT_CLR_MASK                                             0x00000100
#define LIN_SCI1_ICR_TX_DONE_INT_CLR_SET(n)                                           (((uint32_t)(n) << 8  ) & 0x00000100)
#define LIN_SCI1_ICR_TX_DONE_INT_CLR_CLR                                              0xFFFFFEFF

#define LIN_SCI1_ICR_RX_FIFO_OVF_ERR_INT_CLR_SHIFT                                    7
#define LIN_SCI1_ICR_RX_FIFO_OVF_ERR_INT_CLR_MASK                                     0x00000080
#define LIN_SCI1_ICR_RX_FIFO_OVF_ERR_INT_CLR_SET(n)                                   (((uint32_t)(n) << 7  ) & 0x00000080)
#define LIN_SCI1_ICR_RX_FIFO_OVF_ERR_INT_CLR_CLR                                      0xFFFFFF7F

#define LIN_SCI1_ICR_RX_FIFO_FULL_INT_CLR_SHIFT                                       6
#define LIN_SCI1_ICR_RX_FIFO_FULL_INT_CLR_MASK                                        0x00000040
#define LIN_SCI1_ICR_RX_FIFO_FULL_INT_CLR_SET(n)                                      (((uint32_t)(n) << 6  ) & 0x00000040)
#define LIN_SCI1_ICR_RX_FIFO_FULL_INT_CLR_CLR                                         0xFFFFFFBF

#define LIN_SCI1_ICR_RX_STP_ERR_INT_CLR_SHIFT                                         5
#define LIN_SCI1_ICR_RX_STP_ERR_INT_CLR_MASK                                          0x00000020
#define LIN_SCI1_ICR_RX_STP_ERR_INT_CLR_SET(n)                                        (((uint32_t)(n) << 5  ) & 0x00000020)
#define LIN_SCI1_ICR_RX_STP_ERR_INT_CLR_CLR                                           0xFFFFFFDF

#define LIN_SCI1_ICR_RX_CHKSUM_ERR_INT_CLR_SHIFT                                      4
#define LIN_SCI1_ICR_RX_CHKSUM_ERR_INT_CLR_MASK                                       0x00000010
#define LIN_SCI1_ICR_RX_CHKSUM_ERR_INT_CLR_SET(n)                                     (((uint32_t)(n) << 4  ) & 0x00000010)
#define LIN_SCI1_ICR_RX_CHKSUM_ERR_INT_CLR_CLR                                        0xFFFFFFEF

#define LIN_SCI1_ICR_RX_PTY_CHK_ERR_INT_CLR_SHIFT                                     3
#define LIN_SCI1_ICR_RX_PTY_CHK_ERR_INT_CLR_MASK                                      0x00000008
#define LIN_SCI1_ICR_RX_PTY_CHK_ERR_INT_CLR_SET(n)                                    (((uint32_t)(n) << 3  ) & 0x00000008)
#define LIN_SCI1_ICR_RX_PTY_CHK_ERR_INT_CLR_CLR                                       0xFFFFFFF7

#define LIN_SCI1_ICR_RX_PID_DONE_INT_CLR_SHIFT                                        2
#define LIN_SCI1_ICR_RX_PID_DONE_INT_CLR_MASK                                         0x00000004
#define LIN_SCI1_ICR_RX_PID_DONE_INT_CLR_SET(n)                                       (((uint32_t)(n) << 2  ) & 0x00000004)
#define LIN_SCI1_ICR_RX_PID_DONE_INT_CLR_CLR                                          0xFFFFFFFB

#define LIN_SCI1_ICR_RX_DONE_INT_CLR_SHIFT                                            1
#define LIN_SCI1_ICR_RX_DONE_INT_CLR_MASK                                             0x00000002
#define LIN_SCI1_ICR_RX_DONE_INT_CLR_SET(n)                                           (((uint32_t)(n) << 1  ) & 0x00000002)
#define LIN_SCI1_ICR_RX_DONE_INT_CLR_CLR                                              0xFFFFFFFD

#define LIN_SCI1_ICR_RX_1BYTE_DONE_INT_CLR_SHIFT                                      0
#define LIN_SCI1_ICR_RX_1BYTE_DONE_INT_CLR_MASK                                       0x00000001
#define LIN_SCI1_ICR_RX_1BYTE_DONE_INT_CLR_SET(n)                                     (((uint32_t)(n) << 0  ) & 0x00000001)
#define LIN_SCI1_ICR_RX_1BYTE_DONE_INT_CLR_CLR                                        0xFFFFFFFE

#define LIN_SCI1_IMR_RX_FIFO_VLD_INT_MSK_SHIFT                                        22
#define LIN_SCI1_IMR_RX_FIFO_VLD_INT_MSK_MASK                                         0x00400000
#define LIN_SCI1_IMR_RX_FIFO_VLD_INT_MSK_SET(n)                                       (((uint32_t)(n) << 22 ) & 0x00400000)
#define LIN_SCI1_IMR_RX_FIFO_VLD_INT_MSK_CLR                                          0xFFBFFFFF

#define LIN_SCI1_IMR_SHORT_GND_DET_INT_MSK_SHIFT                                      21
#define LIN_SCI1_IMR_SHORT_GND_DET_INT_MSK_MASK                                       0x00200000
#define LIN_SCI1_IMR_SHORT_GND_DET_INT_MSK_SET(n)                                     (((uint32_t)(n) << 21 ) & 0x00200000)
#define LIN_SCI1_IMR_SHORT_GND_DET_INT_MSK_CLR                                        0xFFDFFFFF

#define LIN_SCI1_IMR_RX_DATA_DONE_INT_MSK_SHIFT                                       20
#define LIN_SCI1_IMR_RX_DATA_DONE_INT_MSK_MASK                                        0x00100000
#define LIN_SCI1_IMR_RX_DATA_DONE_INT_MSK_SET(n)                                      (((uint32_t)(n) << 20 ) & 0x00100000)
#define LIN_SCI1_IMR_RX_DATA_DONE_INT_MSK_CLR                                         0xFFEFFFFF

#define LIN_SCI1_IMR_SLV_TX_BRK_DONE_INT_MSK_SHIFT                                    19
#define LIN_SCI1_IMR_SLV_TX_BRK_DONE_INT_MSK_MASK                                     0x00080000
#define LIN_SCI1_IMR_SLV_TX_BRK_DONE_INT_MSK_SET(n)                                   (((uint32_t)(n) << 19 ) & 0x00080000)
#define LIN_SCI1_IMR_SLV_TX_BRK_DONE_INT_MSK_CLR                                      0xFFF7FFFF

#define LIN_SCI1_IMR_TX_1BYTE_DONE_INT_MSK_SHIFT                                      18
#define LIN_SCI1_IMR_TX_1BYTE_DONE_INT_MSK_MASK                                       0x00040000
#define LIN_SCI1_IMR_TX_1BYTE_DONE_INT_MSK_SET(n)                                     (((uint32_t)(n) << 18 ) & 0x00040000)
#define LIN_SCI1_IMR_TX_1BYTE_DONE_INT_MSK_CLR                                        0xFFFBFFFF

#define LIN_SCI1_IMR_MP_MODE_ADDR_INT_MSK_SHIFT                                       15
#define LIN_SCI1_IMR_MP_MODE_ADDR_INT_MSK_MASK                                        0x00008000
#define LIN_SCI1_IMR_MP_MODE_ADDR_INT_MSK_SET(n)                                      (((uint32_t)(n) << 15 ) & 0x00008000)
#define LIN_SCI1_IMR_MP_MODE_ADDR_INT_MSK_CLR                                         0xFFFF7FFF

#define LIN_SCI1_IMR_TX_RX_CONF_INT_MSK_SHIFT                                         14
#define LIN_SCI1_IMR_TX_RX_CONF_INT_MSK_MASK                                          0x00004000
#define LIN_SCI1_IMR_TX_RX_CONF_INT_MSK_SET(n)                                        (((uint32_t)(n) << 14 ) & 0x00004000)
#define LIN_SCI1_IMR_TX_RX_CONF_INT_MSK_CLR                                           0xFFFFBFFF

#define LIN_SCI1_IMR_TX_PID_DONE_INT_MSK_SHIFT                                        13
#define LIN_SCI1_IMR_TX_PID_DONE_INT_MSK_MASK                                         0x00002000
#define LIN_SCI1_IMR_TX_PID_DONE_INT_MSK_SET(n)                                       (((uint32_t)(n) << 13 ) & 0x00002000)
#define LIN_SCI1_IMR_TX_PID_DONE_INT_MSK_CLR                                          0xFFFFDFFF

#define LIN_SCI1_IMR_SYNC_VAL_ERR_INT_MSK_SHIFT                                       12
#define LIN_SCI1_IMR_SYNC_VAL_ERR_INT_MSK_MASK                                        0x00001000
#define LIN_SCI1_IMR_SYNC_VAL_ERR_INT_MSK_SET(n)                                      (((uint32_t)(n) << 12 ) & 0x00001000)
#define LIN_SCI1_IMR_SYNC_VAL_ERR_INT_MSK_CLR                                         0xFFFFEFFF

#define LIN_SCI1_IMR_SYNC_DET_INT_MSK_SHIFT                                           11
#define LIN_SCI1_IMR_SYNC_DET_INT_MSK_MASK                                            0x00000800
#define LIN_SCI1_IMR_SYNC_DET_INT_MSK_SET(n)                                          (((uint32_t)(n) << 11 ) & 0x00000800)
#define LIN_SCI1_IMR_SYNC_DET_INT_MSK_CLR                                             0xFFFFF7FF

#define LIN_SCI1_IMR_BRK_DET_INT_MSK_SHIFT                                            10
#define LIN_SCI1_IMR_BRK_DET_INT_MSK_MASK                                             0x00000400
#define LIN_SCI1_IMR_BRK_DET_INT_MSK_SET(n)                                           (((uint32_t)(n) << 10 ) & 0x00000400)
#define LIN_SCI1_IMR_BRK_DET_INT_MSK_CLR                                              0xFFFFFBFF

#define LIN_SCI1_IMR_TX_FIFO_EMPTY_INT_MSK_SHIFT                                      9
#define LIN_SCI1_IMR_TX_FIFO_EMPTY_INT_MSK_MASK                                       0x00000200
#define LIN_SCI1_IMR_TX_FIFO_EMPTY_INT_MSK_SET(n)                                     (((uint32_t)(n) << 9  ) & 0x00000200)
#define LIN_SCI1_IMR_TX_FIFO_EMPTY_INT_MSK_CLR                                        0xFFFFFDFF

#define LIN_SCI1_IMR_TX_DONE_INT_MSK_SHIFT                                            8
#define LIN_SCI1_IMR_TX_DONE_INT_MSK_MASK                                             0x00000100
#define LIN_SCI1_IMR_TX_DONE_INT_MSK_SET(n)                                           (((uint32_t)(n) << 8  ) & 0x00000100)
#define LIN_SCI1_IMR_TX_DONE_INT_MSK_CLR                                              0xFFFFFEFF

#define LIN_SCI1_IMR_RX_FIFO_OVF_ERR_INT_MSK_SHIFT                                    7
#define LIN_SCI1_IMR_RX_FIFO_OVF_ERR_INT_MSK_MASK                                     0x00000080
#define LIN_SCI1_IMR_RX_FIFO_OVF_ERR_INT_MSK_SET(n)                                   (((uint32_t)(n) << 7  ) & 0x00000080)
#define LIN_SCI1_IMR_RX_FIFO_OVF_ERR_INT_MSK_CLR                                      0xFFFFFF7F

#define LIN_SCI1_IMR_RX_FIFO_FULL_INT_MSK_SHIFT                                       6
#define LIN_SCI1_IMR_RX_FIFO_FULL_INT_MSK_MASK                                        0x00000040
#define LIN_SCI1_IMR_RX_FIFO_FULL_INT_MSK_SET(n)                                      (((uint32_t)(n) << 6  ) & 0x00000040)
#define LIN_SCI1_IMR_RX_FIFO_FULL_INT_MSK_CLR                                         0xFFFFFFBF

#define LIN_SCI1_IMR_RX_STP_ERR_INT_MSK_SHIFT                                         5
#define LIN_SCI1_IMR_RX_STP_ERR_INT_MSK_MASK                                          0x00000020
#define LIN_SCI1_IMR_RX_STP_ERR_INT_MSK_SET(n)                                        (((uint32_t)(n) << 5  ) & 0x00000020)
#define LIN_SCI1_IMR_RX_STP_ERR_INT_MSK_CLR                                           0xFFFFFFDF

#define LIN_SCI1_IMR_RX_CHKSUM_ERR_INT_MSK_SHIFT                                      4
#define LIN_SCI1_IMR_RX_CHKSUM_ERR_INT_MSK_MASK                                       0x00000010
#define LIN_SCI1_IMR_RX_CHKSUM_ERR_INT_MSK_SET(n)                                     (((uint32_t)(n) << 4  ) & 0x00000010)
#define LIN_SCI1_IMR_RX_CHKSUM_ERR_INT_MSK_CLR                                        0xFFFFFFEF

#define LIN_SCI1_IMR_RX_PTY_CHK_ERR_INT_MSK_SHIFT                                     3
#define LIN_SCI1_IMR_RX_PTY_CHK_ERR_INT_MSK_MASK                                      0x00000008
#define LIN_SCI1_IMR_RX_PTY_CHK_ERR_INT_MSK_SET(n)                                    (((uint32_t)(n) << 3  ) & 0x00000008)
#define LIN_SCI1_IMR_RX_PTY_CHK_ERR_INT_MSK_CLR                                       0xFFFFFFF7

#define LIN_SCI1_IMR_RX_PID_DONE_INT_MSK_SHIFT                                        2
#define LIN_SCI1_IMR_RX_PID_DONE_INT_MSK_MASK                                         0x00000004
#define LIN_SCI1_IMR_RX_PID_DONE_INT_MSK_SET(n)                                       (((uint32_t)(n) << 2  ) & 0x00000004)
#define LIN_SCI1_IMR_RX_PID_DONE_INT_MSK_CLR                                          0xFFFFFFFB

#define LIN_SCI1_IMR_RX_DONE_INT_MSK_SHIFT                                            1
#define LIN_SCI1_IMR_RX_DONE_INT_MSK_MASK                                             0x00000002
#define LIN_SCI1_IMR_RX_DONE_INT_MSK_SET(n)                                           (((uint32_t)(n) << 1  ) & 0x00000002)
#define LIN_SCI1_IMR_RX_DONE_INT_MSK_CLR                                              0xFFFFFFFD

#define LIN_SCI1_IMR_RX_1BYTE_DONE_INT_MSK_SHIFT                                      0
#define LIN_SCI1_IMR_RX_1BYTE_DONE_INT_MSK_MASK                                       0x00000001
#define LIN_SCI1_IMR_RX_1BYTE_DONE_INT_MSK_SET(n)                                     (((uint32_t)(n) << 0  ) & 0x00000001)
#define LIN_SCI1_IMR_RX_1BYTE_DONE_INT_MSK_CLR                                        0xFFFFFFFE

#define LIN_SCI1_ISR_RX_FIFO_VLD_INT_STATUS_SHIFT                                     22
#define LIN_SCI1_ISR_RX_FIFO_VLD_INT_STATUS_MASK                                      0x00400000
#define LIN_SCI1_ISR_RX_FIFO_VLD_INT_STATUS_SET(n)                                    (((uint32_t)(n) << 22 ) & 0x00400000)
#define LIN_SCI1_ISR_RX_FIFO_VLD_INT_STATUS_CLR                                       0xFFBFFFFF

#define LIN_SCI1_ISR_SHORT_GND_DET_INT_STATUS_SHIFT                                   21
#define LIN_SCI1_ISR_SHORT_GND_DET_INT_STATUS_MASK                                    0x00200000
#define LIN_SCI1_ISR_SHORT_GND_DET_INT_STATUS_SET(n)                                  (((uint32_t)(n) << 21 ) & 0x00200000)
#define LIN_SCI1_ISR_SHORT_GND_DET_INT_STATUS_CLR                                     0xFFDFFFFF

#define LIN_SCI1_ISR_RX_DATA_DONE_INT_STATUS_SHIFT                                    20
#define LIN_SCI1_ISR_RX_DATA_DONE_INT_STATUS_MASK                                     0x00100000
#define LIN_SCI1_ISR_RX_DATA_DONE_INT_STATUS_SET(n)                                   (((uint32_t)(n) << 20 ) & 0x00100000)
#define LIN_SCI1_ISR_RX_DATA_DONE_INT_STATUS_CLR                                      0xFFEFFFFF

#define LIN_SCI1_ISR_SLV_TX_BRK_DONE_INT_STATUS_SHIFT                                 19
#define LIN_SCI1_ISR_SLV_TX_BRK_DONE_INT_STATUS_MASK                                  0x00080000
#define LIN_SCI1_ISR_SLV_TX_BRK_DONE_INT_STATUS_SET(n)                                (((uint32_t)(n) << 19 ) & 0x00080000)
#define LIN_SCI1_ISR_SLV_TX_BRK_DONE_INT_STATUS_CLR                                   0xFFF7FFFF

#define LIN_SCI1_ISR_TX_1BYTE_DONE_INT_STATUS_SHIFT                                   18
#define LIN_SCI1_ISR_TX_1BYTE_DONE_INT_STATUS_MASK                                    0x00040000
#define LIN_SCI1_ISR_TX_1BYTE_DONE_INT_STATUS_SET(n)                                  (((uint32_t)(n) << 18 ) & 0x00040000)
#define LIN_SCI1_ISR_TX_1BYTE_DONE_INT_STATUS_CLR                                     0xFFFBFFFF

#define LIN_SCI1_ISR_MP_MODE_ADDR_INT_STATUS_SHIFT                                    15
#define LIN_SCI1_ISR_MP_MODE_ADDR_INT_STATUS_MASK                                     0x00008000
#define LIN_SCI1_ISR_MP_MODE_ADDR_INT_STATUS_SET(n)                                   (((uint32_t)(n) << 15 ) & 0x00008000)
#define LIN_SCI1_ISR_MP_MODE_ADDR_INT_STATUS_CLR                                      0xFFFF7FFF

#define LIN_SCI1_ISR_TX_RX_CONF_INT_STATUS_SHIFT                                      14
#define LIN_SCI1_ISR_TX_RX_CONF_INT_STATUS_MASK                                       0x00004000
#define LIN_SCI1_ISR_TX_RX_CONF_INT_STATUS_SET(n)                                     (((uint32_t)(n) << 14 ) & 0x00004000)
#define LIN_SCI1_ISR_TX_RX_CONF_INT_STATUS_CLR                                        0xFFFFBFFF

#define LIN_SCI1_ISR_TX_PID_DONE_INT_STATUS_SHIFT                                     13
#define LIN_SCI1_ISR_TX_PID_DONE_INT_STATUS_MASK                                      0x00002000
#define LIN_SCI1_ISR_TX_PID_DONE_INT_STATUS_SET(n)                                    (((uint32_t)(n) << 13 ) & 0x00002000)
#define LIN_SCI1_ISR_TX_PID_DONE_INT_STATUS_CLR                                       0xFFFFDFFF

#define LIN_SCI1_ISR_SYNC_VAL_ERR_INT_STATUS_SHIFT                                    12
#define LIN_SCI1_ISR_SYNC_VAL_ERR_INT_STATUS_MASK                                     0x00001000
#define LIN_SCI1_ISR_SYNC_VAL_ERR_INT_STATUS_SET(n)                                   (((uint32_t)(n) << 12 ) & 0x00001000)
#define LIN_SCI1_ISR_SYNC_VAL_ERR_INT_STATUS_CLR                                      0xFFFFEFFF

#define LIN_SCI1_ISR_SYNC_DET_INT_STATUS_SHIFT                                        11
#define LIN_SCI1_ISR_SYNC_DET_INT_STATUS_MASK                                         0x00000800
#define LIN_SCI1_ISR_SYNC_DET_INT_STATUS_SET(n)                                       (((uint32_t)(n) << 11 ) & 0x00000800)
#define LIN_SCI1_ISR_SYNC_DET_INT_STATUS_CLR                                          0xFFFFF7FF

#define LIN_SCI1_ISR_BRK_DET_INT_STATUS_SHIFT                                         10
#define LIN_SCI1_ISR_BRK_DET_INT_STATUS_MASK                                          0x00000400
#define LIN_SCI1_ISR_BRK_DET_INT_STATUS_SET(n)                                        (((uint32_t)(n) << 10 ) & 0x00000400)
#define LIN_SCI1_ISR_BRK_DET_INT_STATUS_CLR                                           0xFFFFFBFF

#define LIN_SCI1_ISR_TX_FIFO_EMPTY_INT_STATUS_SHIFT                                   9
#define LIN_SCI1_ISR_TX_FIFO_EMPTY_INT_STATUS_MASK                                    0x00000200
#define LIN_SCI1_ISR_TX_FIFO_EMPTY_INT_STATUS_SET(n)                                  (((uint32_t)(n) << 9  ) & 0x00000200)
#define LIN_SCI1_ISR_TX_FIFO_EMPTY_INT_STATUS_CLR                                     0xFFFFFDFF

#define LIN_SCI1_ISR_TX_DONE_INT_STATUS_SHIFT                                         8
#define LIN_SCI1_ISR_TX_DONE_INT_STATUS_MASK                                          0x00000100
#define LIN_SCI1_ISR_TX_DONE_INT_STATUS_SET(n)                                        (((uint32_t)(n) << 8  ) & 0x00000100)
#define LIN_SCI1_ISR_TX_DONE_INT_STATUS_CLR                                           0xFFFFFEFF

#define LIN_SCI1_ISR_RX_FIFO_OVF_ERR_INT_STATUS_SHIFT                                 7
#define LIN_SCI1_ISR_RX_FIFO_OVF_ERR_INT_STATUS_MASK                                  0x00000080
#define LIN_SCI1_ISR_RX_FIFO_OVF_ERR_INT_STATUS_SET(n)                                (((uint32_t)(n) << 7  ) & 0x00000080)
#define LIN_SCI1_ISR_RX_FIFO_OVF_ERR_INT_STATUS_CLR                                   0xFFFFFF7F

#define LIN_SCI1_ISR_RX_FIFO_FULL_INT_STATUS_SHIFT                                    6
#define LIN_SCI1_ISR_RX_FIFO_FULL_INT_STATUS_MASK                                     0x00000040
#define LIN_SCI1_ISR_RX_FIFO_FULL_INT_STATUS_SET(n)                                   (((uint32_t)(n) << 6  ) & 0x00000040)
#define LIN_SCI1_ISR_RX_FIFO_FULL_INT_STATUS_CLR                                      0xFFFFFFBF

#define LIN_SCI1_ISR_RX_STP_ERR_INT_STATUS_SHIFT                                      5
#define LIN_SCI1_ISR_RX_STP_ERR_INT_STATUS_MASK                                       0x00000020
#define LIN_SCI1_ISR_RX_STP_ERR_INT_STATUS_SET(n)                                     (((uint32_t)(n) << 5  ) & 0x00000020)
#define LIN_SCI1_ISR_RX_STP_ERR_INT_STATUS_CLR                                        0xFFFFFFDF

#define LIN_SCI1_ISR_RX_CHKSUM_ERR_INT_STATUS_SHIFT                                   4
#define LIN_SCI1_ISR_RX_CHKSUM_ERR_INT_STATUS_MASK                                    0x00000010
#define LIN_SCI1_ISR_RX_CHKSUM_ERR_INT_STATUS_SET(n)                                  (((uint32_t)(n) << 4  ) & 0x00000010)
#define LIN_SCI1_ISR_RX_CHKSUM_ERR_INT_STATUS_CLR                                     0xFFFFFFEF

#define LIN_SCI1_ISR_RX_PTY_CHK_ERR_INT_STATUS_SHIFT                                  3
#define LIN_SCI1_ISR_RX_PTY_CHK_ERR_INT_STATUS_MASK                                   0x00000008
#define LIN_SCI1_ISR_RX_PTY_CHK_ERR_INT_STATUS_SET(n)                                 (((uint32_t)(n) << 3  ) & 0x00000008)
#define LIN_SCI1_ISR_RX_PTY_CHK_ERR_INT_STATUS_CLR                                    0xFFFFFFF7

#define LIN_SCI1_ISR_RX_PID_DONE_INT_STATUS_SHIFT                                     2
#define LIN_SCI1_ISR_RX_PID_DONE_INT_STATUS_MASK                                      0x00000004
#define LIN_SCI1_ISR_RX_PID_DONE_INT_STATUS_SET(n)                                    (((uint32_t)(n) << 2  ) & 0x00000004)
#define LIN_SCI1_ISR_RX_PID_DONE_INT_STATUS_CLR                                       0xFFFFFFFB

#define LIN_SCI1_ISR_RX_DONE_INT_STATUS_SHIFT                                         1
#define LIN_SCI1_ISR_RX_DONE_INT_STATUS_MASK                                          0x00000002
#define LIN_SCI1_ISR_RX_DONE_INT_STATUS_SET(n)                                        (((uint32_t)(n) << 1  ) & 0x00000002)
#define LIN_SCI1_ISR_RX_DONE_INT_STATUS_CLR                                           0xFFFFFFFD

#define LIN_SCI1_ISR_RX_1BYTE_DONE_INT_STATUS_SHIFT                                   0
#define LIN_SCI1_ISR_RX_1BYTE_DONE_INT_STATUS_MASK                                    0x00000001
#define LIN_SCI1_ISR_RX_1BYTE_DONE_INT_STATUS_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x00000001)
#define LIN_SCI1_ISR_RX_1BYTE_DONE_INT_STATUS_CLR                                     0xFFFFFFFE

#define LIN_SCI1_IRSR_RX_FIFO_VLD_INT_RAW_STATUS_SHIFT                                22
#define LIN_SCI1_IRSR_RX_FIFO_VLD_INT_RAW_STATUS_MASK                                 0x00400000
#define LIN_SCI1_IRSR_RX_FIFO_VLD_INT_RAW_STATUS_SET(n)                               (((uint32_t)(n) << 22 ) & 0x00400000)
#define LIN_SCI1_IRSR_RX_FIFO_VLD_INT_RAW_STATUS_CLR                                  0xFFBFFFFF

#define LIN_SCI1_IRSR_SHORT_GND_DET_INT_RAW_STATUS_SHIFT                              21
#define LIN_SCI1_IRSR_SHORT_GND_DET_INT_RAW_STATUS_MASK                               0x00200000
#define LIN_SCI1_IRSR_SHORT_GND_DET_INT_RAW_STATUS_SET(n)                             (((uint32_t)(n) << 21 ) & 0x00200000)
#define LIN_SCI1_IRSR_SHORT_GND_DET_INT_RAW_STATUS_CLR                                0xFFDFFFFF

#define LIN_SCI1_IRSR_RX_DATA_DONE_INT_RAW_STATUS_SHIFT                               20
#define LIN_SCI1_IRSR_RX_DATA_DONE_INT_RAW_STATUS_MASK                                0x00100000
#define LIN_SCI1_IRSR_RX_DATA_DONE_INT_RAW_STATUS_SET(n)                              (((uint32_t)(n) << 20 ) & 0x00100000)
#define LIN_SCI1_IRSR_RX_DATA_DONE_INT_RAW_STATUS_CLR                                 0xFFEFFFFF

#define LIN_SCI1_IRSR_SLV_TX_BRK_DONE_INT_RAW_STATUS_SHIFT                            19
#define LIN_SCI1_IRSR_SLV_TX_BRK_DONE_INT_RAW_STATUS_MASK                             0x00080000
#define LIN_SCI1_IRSR_SLV_TX_BRK_DONE_INT_RAW_STATUS_SET(n)                           (((uint32_t)(n) << 19 ) & 0x00080000)
#define LIN_SCI1_IRSR_SLV_TX_BRK_DONE_INT_RAW_STATUS_CLR                              0xFFF7FFFF

#define LIN_SCI1_IRSR_TX_1BYTE_DONE_INT_RAW_STATUS_SHIFT                              18
#define LIN_SCI1_IRSR_TX_1BYTE_DONE_INT_RAW_STATUS_MASK                               0x00040000
#define LIN_SCI1_IRSR_TX_1BYTE_DONE_INT_RAW_STATUS_SET(n)                             (((uint32_t)(n) << 18 ) & 0x00040000)
#define LIN_SCI1_IRSR_TX_1BYTE_DONE_INT_RAW_STATUS_CLR                                0xFFFBFFFF

#define LIN_SCI1_IRSR_MP_MODE_ADDR_INT_RAW_STATUS_SHIFT                               15
#define LIN_SCI1_IRSR_MP_MODE_ADDR_INT_RAW_STATUS_MASK                                0x00008000
#define LIN_SCI1_IRSR_MP_MODE_ADDR_INT_RAW_STATUS_SET(n)                              (((uint32_t)(n) << 15 ) & 0x00008000)
#define LIN_SCI1_IRSR_MP_MODE_ADDR_INT_RAW_STATUS_CLR                                 0xFFFF7FFF

#define LIN_SCI1_IRSR_TX_RX_CONF_INT_RAW_STATUS_SHIFT                                 14
#define LIN_SCI1_IRSR_TX_RX_CONF_INT_RAW_STATUS_MASK                                  0x00004000
#define LIN_SCI1_IRSR_TX_RX_CONF_INT_RAW_STATUS_SET(n)                                (((uint32_t)(n) << 14 ) & 0x00004000)
#define LIN_SCI1_IRSR_TX_RX_CONF_INT_RAW_STATUS_CLR                                   0xFFFFBFFF

#define LIN_SCI1_IRSR_TX_PID_DONE_INT_RAW_STATUS_SHIFT                                13
#define LIN_SCI1_IRSR_TX_PID_DONE_INT_RAW_STATUS_MASK                                 0x00002000
#define LIN_SCI1_IRSR_TX_PID_DONE_INT_RAW_STATUS_SET(n)                               (((uint32_t)(n) << 13 ) & 0x00002000)
#define LIN_SCI1_IRSR_TX_PID_DONE_INT_RAW_STATUS_CLR                                  0xFFFFDFFF

#define LIN_SCI1_IRSR_SYNC_VAL_ERR_INT_RAW_STATUS_SHIFT                               12
#define LIN_SCI1_IRSR_SYNC_VAL_ERR_INT_RAW_STATUS_MASK                                0x00001000
#define LIN_SCI1_IRSR_SYNC_VAL_ERR_INT_RAW_STATUS_SET(n)                              (((uint32_t)(n) << 12 ) & 0x00001000)
#define LIN_SCI1_IRSR_SYNC_VAL_ERR_INT_RAW_STATUS_CLR                                 0xFFFFEFFF

#define LIN_SCI1_IRSR_SYNC_DET_INT_RAW_STATUS_SHIFT                                   11
#define LIN_SCI1_IRSR_SYNC_DET_INT_RAW_STATUS_MASK                                    0x00000800
#define LIN_SCI1_IRSR_SYNC_DET_INT_RAW_STATUS_SET(n)                                  (((uint32_t)(n) << 11 ) & 0x00000800)
#define LIN_SCI1_IRSR_SYNC_DET_INT_RAW_STATUS_CLR                                     0xFFFFF7FF

#define LIN_SCI1_IRSR_BRK_DET_INT_RAW_STATUS_SHIFT                                    10
#define LIN_SCI1_IRSR_BRK_DET_INT_RAW_STATUS_MASK                                     0x00000400
#define LIN_SCI1_IRSR_BRK_DET_INT_RAW_STATUS_SET(n)                                   (((uint32_t)(n) << 10 ) & 0x00000400)
#define LIN_SCI1_IRSR_BRK_DET_INT_RAW_STATUS_CLR                                      0xFFFFFBFF

#define LIN_SCI1_IRSR_TX_FIFO_EMPTY_INT_RAW_STATUS_SHIFT                              9
#define LIN_SCI1_IRSR_TX_FIFO_EMPTY_INT_RAW_STATUS_MASK                               0x00000200
#define LIN_SCI1_IRSR_TX_FIFO_EMPTY_INT_RAW_STATUS_SET(n)                             (((uint32_t)(n) << 9  ) & 0x00000200)
#define LIN_SCI1_IRSR_TX_FIFO_EMPTY_INT_RAW_STATUS_CLR                                0xFFFFFDFF

#define LIN_SCI1_IRSR_TX_DONE_INT_RAW_STATUS_SHIFT                                    8
#define LIN_SCI1_IRSR_TX_DONE_INT_RAW_STATUS_MASK                                     0x00000100
#define LIN_SCI1_IRSR_TX_DONE_INT_RAW_STATUS_SET(n)                                   (((uint32_t)(n) << 8  ) & 0x00000100)
#define LIN_SCI1_IRSR_TX_DONE_INT_RAW_STATUS_CLR                                      0xFFFFFEFF

#define LIN_SCI1_IRSR_RX_FIFO_OVF_ERR_INT_RAW_STATUS_SHIFT                            7
#define LIN_SCI1_IRSR_RX_FIFO_OVF_ERR_INT_RAW_STATUS_MASK                             0x00000080
#define LIN_SCI1_IRSR_RX_FIFO_OVF_ERR_INT_RAW_STATUS_SET(n)                           (((uint32_t)(n) << 7  ) & 0x00000080)
#define LIN_SCI1_IRSR_RX_FIFO_OVF_ERR_INT_RAW_STATUS_CLR                              0xFFFFFF7F

#define LIN_SCI1_IRSR_RX_FIFO_FULL_INT_RAW_STATUS_SHIFT                               6
#define LIN_SCI1_IRSR_RX_FIFO_FULL_INT_RAW_STATUS_MASK                                0x00000040
#define LIN_SCI1_IRSR_RX_FIFO_FULL_INT_RAW_STATUS_SET(n)                              (((uint32_t)(n) << 6  ) & 0x00000040)
#define LIN_SCI1_IRSR_RX_FIFO_FULL_INT_RAW_STATUS_CLR                                 0xFFFFFFBF

#define LIN_SCI1_IRSR_RX_STP_ERR_INT_RAW_STATUS_SHIFT                                 5
#define LIN_SCI1_IRSR_RX_STP_ERR_INT_RAW_STATUS_MASK                                  0x00000020
#define LIN_SCI1_IRSR_RX_STP_ERR_INT_RAW_STATUS_SET(n)                                (((uint32_t)(n) << 5  ) & 0x00000020)
#define LIN_SCI1_IRSR_RX_STP_ERR_INT_RAW_STATUS_CLR                                   0xFFFFFFDF

#define LIN_SCI1_IRSR_RX_CHKSUM_ERR_INT_RAW_STATUS_SHIFT                              4
#define LIN_SCI1_IRSR_RX_CHKSUM_ERR_INT_RAW_STATUS_MASK                               0x00000010
#define LIN_SCI1_IRSR_RX_CHKSUM_ERR_INT_RAW_STATUS_SET(n)                             (((uint32_t)(n) << 4  ) & 0x00000010)
#define LIN_SCI1_IRSR_RX_CHKSUM_ERR_INT_RAW_STATUS_CLR                                0xFFFFFFEF

#define LIN_SCI1_IRSR_RX_PTY_CHK_ERR_INT_RAW_STATUS_SHIFT                             3
#define LIN_SCI1_IRSR_RX_PTY_CHK_ERR_INT_RAW_STATUS_MASK                              0x00000008
#define LIN_SCI1_IRSR_RX_PTY_CHK_ERR_INT_RAW_STATUS_SET(n)                            (((uint32_t)(n) << 3  ) & 0x00000008)
#define LIN_SCI1_IRSR_RX_PTY_CHK_ERR_INT_RAW_STATUS_CLR                               0xFFFFFFF7

#define LIN_SCI1_IRSR_RX_PID_DONE_INT_RAW_STATUS_SHIFT                                2
#define LIN_SCI1_IRSR_RX_PID_DONE_INT_RAW_STATUS_MASK                                 0x00000004
#define LIN_SCI1_IRSR_RX_PID_DONE_INT_RAW_STATUS_SET(n)                               (((uint32_t)(n) << 2  ) & 0x00000004)
#define LIN_SCI1_IRSR_RX_PID_DONE_INT_RAW_STATUS_CLR                                  0xFFFFFFFB

#define LIN_SCI1_IRSR_RX_DONE_INT_RAW_STATUS_SHIFT                                    1
#define LIN_SCI1_IRSR_RX_DONE_INT_RAW_STATUS_MASK                                     0x00000002
#define LIN_SCI1_IRSR_RX_DONE_INT_RAW_STATUS_SET(n)                                   (((uint32_t)(n) << 1  ) & 0x00000002)
#define LIN_SCI1_IRSR_RX_DONE_INT_RAW_STATUS_CLR                                      0xFFFFFFFD

#define LIN_SCI1_IRSR_RX_1BYTE_DONE_INT_RAW_STATUS_SHIFT                              0
#define LIN_SCI1_IRSR_RX_1BYTE_DONE_INT_RAW_STATUS_MASK                               0x00000001
#define LIN_SCI1_IRSR_RX_1BYTE_DONE_INT_RAW_STATUS_SET(n)                             (((uint32_t)(n) << 0  ) & 0x00000001)
#define LIN_SCI1_IRSR_RX_1BYTE_DONE_INT_RAW_STATUS_CLR                                0xFFFFFFFE

// Register Structure Definition
typedef struct
{
    __IO uint32_t TX_DATA                        : 8  ; // 7  : 0
    __IO uint32_t RESERVED0                      : 24 ; // 31 : 8
} LIN_SCI1_TX_DATA_FIELD_T;

typedef struct
{
    __IO uint32_t RX_DATA                        : 8  ; // 7  : 0
    __IO uint32_t RESERVED0                      : 24 ; // 31 : 8
} LIN_SCI1_RX_DATA_FIELD_T;

typedef struct
{
    __IO uint32_t GLB_EN                         : 1  ; // 0  : 0
    __IO uint32_t TX_EN                          : 1  ; // 1  : 1
    __IO uint32_t RX_EN                          : 1  ; // 2  : 2
    __IO uint32_t AUTO_BAUD_EN                   : 1  ; // 3  : 3
    __IO uint32_t BRK_TX_TRIG                    : 1  ; // 4  : 4
    __IO uint32_t CHKSUM_EN                      : 1  ; // 5  : 5
    __IO uint32_t CHKSUM_TYPE                    : 1  ; // 6  : 6
    __IO uint32_t TX_NUM_MODE                    : 1  ; // 7  : 7
    __IO uint32_t RX_NUM_MODE                    : 1  ; // 8  : 8
    __IO uint32_t MASTER_EN                      : 1  ; // 9  : 9
    __IO uint32_t TX_WAIT_FIFO_VLD_EN            : 1  ; // 10 : 10
    __IO uint32_t RESERVED1                      : 1  ; // 11 : 11
    __IO uint32_t RX_NUM                         : 4  ; // 15 : 12
    __IO uint32_t TX_NUM                         : 4  ; // 19 : 16
    __IO uint32_t TX_MSK                         : 2  ; // 21 : 20
    __IO uint32_t RX_MSK                         : 1  ; // 22 : 22
    __IO uint32_t TX_ABORT                       : 1  ; // 23 : 23
    __IO uint32_t RX_ABORT                       : 1  ; // 24 : 24
    __IO uint32_t TX_FIFO_CLR                    : 1  ; // 25 : 25
    __IO uint32_t RX_FIFO_CLR                    : 1  ; // 26 : 26
    __IO uint32_t LPBK_MODE                      : 2  ; // 28 : 27
    __IO uint32_t SHORT_GND_DET_EN               : 1  ; // 29 : 29
    __IO uint32_t BIT_ERR_DET_EN                 : 1  ; // 30 : 30
    __IO uint32_t RESERVED0                      : 1  ; // 31 : 31
} LIN_SCI1_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t MODE                           : 1  ; // 0  : 0
    __IO uint32_t PTY_CHK_EN                     : 1  ; // 1  : 1
    __IO uint32_t STP_BIT_SEL                    : 1  ; // 2  : 2
    __IO uint32_t RESERVED1                      : 1  ; // 3  : 3
    __IO uint32_t PTY_MODE                       : 2  ; // 5  : 4
    __IO uint32_t MP_MODE_EN                     : 1  ; // 6  : 6
    __IO uint32_t MP_TX_ADDR_DATA_SEL            : 1  ; // 7  : 7
    __IO uint32_t MP_RX_ADDR_WR_EN               : 1  ; // 8  : 8
    __IO uint32_t RESERVED0                      : 23 ; // 31 : 9
} LIN_SCI1_CTRL_UART_FIELD_T;

typedef struct
{
    __IO uint32_t BD_INTR                        : 9  ; // 8  : 0
    __IO uint32_t RESERVED1                      : 3  ; // 11 : 9
    __IO uint32_t BD_FRAC                        : 4  ; // 15 : 12
    __IO uint32_t RESERVED0                      : 16 ; // 31 : 16
} LIN_SCI1_BAUD_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t BRK_NUM                        : 10 ; // 9  : 0
    __IO uint32_t DLT_NUM                        : 2  ; // 11 : 10
    __IO uint32_t BRK_THD                        : 1  ; // 12 : 12
    __IO uint32_t SHORT_GND_THD                  : 1  ; // 13 : 13
    __IO uint32_t RESERVED0                      : 18 ; // 31 : 14
} LIN_SCI1_BRK_SYNC_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t SW_TXD_VAL                     : 1  ; // 0  : 0
    __IO uint32_t TX_BYTE_SPACE                  : 2  ; // 2  : 1
    __IO uint32_t CHK_PT_SEL                     : 1  ; // 3  : 3
    __IO uint32_t TX_RSP_SPACE                   : 2  ; // 5  : 4
    __IO uint32_t RESERVED1                      : 2  ; // 7  : 6
    __IO uint32_t TX_PID                         : 8  ; // 15 : 8
    __IO uint32_t RESERVED0                      : 16 ; // 31 : 16
} LIN_SCI1_TX_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t MP_SLAVE_ADDR                  : 8  ; // 7  : 0
    __IO uint32_t MP_SLAVE_ADDR_MSK              : 1  ; // 8  : 8
    __IO uint32_t RESERVED1                      : 7  ; // 15 : 9
    __IO uint32_t RX_FIFO_VLD_ENTRY              : 2  ; // 17 : 16
    __IO uint32_t RESERVED0                      : 14 ; // 31 : 18
} LIN_SCI1_RX_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t RX_FILTER_TIM                  : 9  ; // 8  : 0
    __IO uint32_t RESERVED0                      : 23 ; // 31 : 9
} LIN_SCI1_RX_FILTER_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t RXD_VAL                        : 1  ; // 0  : 0
    __IO uint32_t TXD_VAL                        : 1  ; // 1  : 1
    __IO uint32_t TX_FIFO_FULL                   : 1  ; // 2  : 2
    __IO uint32_t RX_FIFO_EMPTY                  : 1  ; // 3  : 3
    __IO uint32_t RX_STATE                       : 1  ; // 4  : 4
    __IO uint32_t TX_STATE                       : 1  ; // 5  : 5
    __IO uint32_t RX_BRK_STATE                   : 1  ; // 6  : 6
    __IO uint32_t RX_SYNC_STATE                  : 1  ; // 7  : 7
    __IO uint32_t RXD_VAL_ANA                    : 1  ; // 8  : 8
    __IO uint32_t RESERVED0                      : 23 ; // 31 : 9
} LIN_SCI1_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t AUTO_BD_INTR                   : 9  ; // 8  : 0
    __IO uint32_t RESERVED1                      : 3  ; // 11 : 9
    __IO uint32_t AUTO_BD_FRAC                   : 4  ; // 15 : 12
    __IO uint32_t RESERVED0                      : 16 ; // 31 : 16
} LIN_SCI1_AUTO_BAUD_VAL_FIELD_T;

typedef struct
{
    __IO uint32_t RX_PID                         : 8  ; // 7  : 0
    __IO uint32_t RESERVED0                      : 24 ; // 31 : 8
} LIN_SCI1_RX_PID_FIELD_T;

typedef struct
{
    __IO uint32_t CHKSUM_VAL                     : 8  ; // 7  : 0
    __IO uint32_t RESERVED0                      : 24 ; // 31 : 8
} LIN_SCI1_CHKSUM_VAL_FIELD_T;

typedef struct
{
    __IO uint32_t RX_1BYTE_DONE_INT_CLR          : 1  ; // 0  : 0
    __IO uint32_t RX_DONE_INT_CLR                : 1  ; // 1  : 1
    __IO uint32_t RX_PID_DONE_INT_CLR            : 1  ; // 2  : 2
    __IO uint32_t RX_PTY_CHK_ERR_INT_CLR         : 1  ; // 3  : 3
    __IO uint32_t RX_CHKSUM_ERR_INT_CLR          : 1  ; // 4  : 4
    __IO uint32_t RX_STP_ERR_INT_CLR             : 1  ; // 5  : 5
    __IO uint32_t RX_FIFO_FULL_INT_CLR           : 1  ; // 6  : 6
    __IO uint32_t RX_FIFO_OVF_ERR_INT_CLR        : 1  ; // 7  : 7
    __IO uint32_t TX_DONE_INT_CLR                : 1  ; // 8  : 8
    __IO uint32_t TX_FIFO_EMPTY_INT_CLR          : 1  ; // 9  : 9
    __IO uint32_t BRK_DET_INT_CLR                : 1  ; // 10 : 10
    __IO uint32_t SYNC_DET_INT_CLR               : 1  ; // 11 : 11
    __IO uint32_t SYNC_VAL_ERR_INT_CLR           : 1  ; // 12 : 12
    __IO uint32_t TX_PID_DONE_INT_CLR            : 1  ; // 13 : 13
    __IO uint32_t TX_RX_CONF_INT_CLR             : 1  ; // 14 : 14
    __IO uint32_t MP_MODE_ADDR_INT_CLR           : 1  ; // 15 : 15
    __IO uint32_t RESERVED2                      : 1  ; // 16 : 16
    __IO uint32_t RESERVED1                      : 1  ; // 17 : 17
    __IO uint32_t TX_1BYTE_DONE_INT_CLR          : 1  ; // 18 : 18
    __IO uint32_t SLV_TX_BRK_DONE_INT_CLR        : 1  ; // 19 : 19
    __IO uint32_t RX_DATA_DONE_INT_CLR           : 1  ; // 20 : 20
    __IO uint32_t SHORT_GND_DET_INT_CLR          : 1  ; // 21 : 21
    __IO uint32_t RX_FIFO_VLD_INT_CLR            : 1  ; // 22 : 22
    __IO uint32_t RESERVED0                      : 9  ; // 31 : 23
} LIN_SCI1_ICR_FIELD_T;

typedef struct
{
    __IO uint32_t RX_1BYTE_DONE_INT_MSK          : 1  ; // 0  : 0
    __IO uint32_t RX_DONE_INT_MSK                : 1  ; // 1  : 1
    __IO uint32_t RX_PID_DONE_INT_MSK            : 1  ; // 2  : 2
    __IO uint32_t RX_PTY_CHK_ERR_INT_MSK         : 1  ; // 3  : 3
    __IO uint32_t RX_CHKSUM_ERR_INT_MSK          : 1  ; // 4  : 4
    __IO uint32_t RX_STP_ERR_INT_MSK             : 1  ; // 5  : 5
    __IO uint32_t RX_FIFO_FULL_INT_MSK           : 1  ; // 6  : 6
    __IO uint32_t RX_FIFO_OVF_ERR_INT_MSK        : 1  ; // 7  : 7
    __IO uint32_t TX_DONE_INT_MSK                : 1  ; // 8  : 8
    __IO uint32_t TX_FIFO_EMPTY_INT_MSK          : 1  ; // 9  : 9
    __IO uint32_t BRK_DET_INT_MSK                : 1  ; // 10 : 10
    __IO uint32_t SYNC_DET_INT_MSK               : 1  ; // 11 : 11
    __IO uint32_t SYNC_VAL_ERR_INT_MSK           : 1  ; // 12 : 12
    __IO uint32_t TX_PID_DONE_INT_MSK            : 1  ; // 13 : 13
    __IO uint32_t TX_RX_CONF_INT_MSK             : 1  ; // 14 : 14
    __IO uint32_t MP_MODE_ADDR_INT_MSK           : 1  ; // 15 : 15
    __IO uint32_t RESERVED2                      : 1  ; // 16 : 16
    __IO uint32_t RESERVED1                      : 1  ; // 17 : 17
    __IO uint32_t TX_1BYTE_DONE_INT_MSK          : 1  ; // 18 : 18
    __IO uint32_t SLV_TX_BRK_DONE_INT_MSK        : 1  ; // 19 : 19
    __IO uint32_t RX_DATA_DONE_INT_MSK           : 1  ; // 20 : 20
    __IO uint32_t SHORT_GND_DET_INT_MSK          : 1  ; // 21 : 21
    __IO uint32_t RX_FIFO_VLD_INT_MSK            : 1  ; // 22 : 22
    __IO uint32_t RESERVED0                      : 9  ; // 31 : 23
} LIN_SCI1_IMR_FIELD_T;

typedef struct
{
    __IO uint32_t RX_1BYTE_DONE_INT_STATUS       : 1  ; // 0  : 0
    __IO uint32_t RX_DONE_INT_STATUS             : 1  ; // 1  : 1
    __IO uint32_t RX_PID_DONE_INT_STATUS         : 1  ; // 2  : 2
    __IO uint32_t RX_PTY_CHK_ERR_INT_STATUS      : 1  ; // 3  : 3
    __IO uint32_t RX_CHKSUM_ERR_INT_STATUS       : 1  ; // 4  : 4
    __IO uint32_t RX_STP_ERR_INT_STATUS          : 1  ; // 5  : 5
    __IO uint32_t RX_FIFO_FULL_INT_STATUS        : 1  ; // 6  : 6
    __IO uint32_t RX_FIFO_OVF_ERR_INT_STATUS     : 1  ; // 7  : 7
    __IO uint32_t TX_DONE_INT_STATUS             : 1  ; // 8  : 8
    __IO uint32_t TX_FIFO_EMPTY_INT_STATUS       : 1  ; // 9  : 9
    __IO uint32_t BRK_DET_INT_STATUS             : 1  ; // 10 : 10
    __IO uint32_t SYNC_DET_INT_STATUS            : 1  ; // 11 : 11
    __IO uint32_t SYNC_VAL_ERR_INT_STATUS        : 1  ; // 12 : 12
    __IO uint32_t TX_PID_DONE_INT_STATUS         : 1  ; // 13 : 13
    __IO uint32_t TX_RX_CONF_INT_STATUS          : 1  ; // 14 : 14
    __IO uint32_t MP_MODE_ADDR_INT_STATUS        : 1  ; // 15 : 15
    __IO uint32_t RESERVED2                      : 1  ; // 16 : 16
    __IO uint32_t RESERVED1                      : 1  ; // 17 : 17
    __IO uint32_t TX_1BYTE_DONE_INT_STATUS       : 1  ; // 18 : 18
    __IO uint32_t SLV_TX_BRK_DONE_INT_STATUS     : 1  ; // 19 : 19
    __IO uint32_t RX_DATA_DONE_INT_STATUS        : 1  ; // 20 : 20
    __IO uint32_t SHORT_GND_DET_INT_STATUS       : 1  ; // 21 : 21
    __IO uint32_t RX_FIFO_VLD_INT_STATUS         : 1  ; // 22 : 22
    __IO uint32_t RESERVED0                      : 9  ; // 31 : 23
} LIN_SCI1_ISR_FIELD_T;

typedef struct
{
    __IO uint32_t RX_1BYTE_DONE_INT_RAW_STATUS   : 1  ; // 0  : 0
    __IO uint32_t RX_DONE_INT_RAW_STATUS         : 1  ; // 1  : 1
    __IO uint32_t RX_PID_DONE_INT_RAW_STATUS     : 1  ; // 2  : 2
    __IO uint32_t RX_PTY_CHK_ERR_INT_RAW_STATUS  : 1  ; // 3  : 3
    __IO uint32_t RX_CHKSUM_ERR_INT_RAW_STATUS   : 1  ; // 4  : 4
    __IO uint32_t RX_STP_ERR_INT_RAW_STATUS      : 1  ; // 5  : 5
    __IO uint32_t RX_FIFO_FULL_INT_RAW_STATUS    : 1  ; // 6  : 6
    __IO uint32_t RX_FIFO_OVF_ERR_INT_RAW_STATUS : 1  ; // 7  : 7
    __IO uint32_t TX_DONE_INT_RAW_STATUS         : 1  ; // 8  : 8
    __IO uint32_t TX_FIFO_EMPTY_INT_RAW_STATUS   : 1  ; // 9  : 9
    __IO uint32_t BRK_DET_INT_RAW_STATUS         : 1  ; // 10 : 10
    __IO uint32_t SYNC_DET_INT_RAW_STATUS        : 1  ; // 11 : 11
    __IO uint32_t SYNC_VAL_ERR_INT_RAW_STATUS    : 1  ; // 12 : 12
    __IO uint32_t TX_PID_DONE_INT_RAW_STATUS     : 1  ; // 13 : 13
    __IO uint32_t TX_RX_CONF_INT_RAW_STATUS      : 1  ; // 14 : 14
    __IO uint32_t MP_MODE_ADDR_INT_RAW_STATUS    : 1  ; // 15 : 15
    __IO uint32_t RESERVED2                      : 1  ; // 16 : 16
    __IO uint32_t RESERVED1                      : 1  ; // 17 : 17
    __IO uint32_t TX_1BYTE_DONE_INT_RAW_STATUS   : 1  ; // 18 : 18
    __IO uint32_t SLV_TX_BRK_DONE_INT_RAW_STATUS : 1  ; // 19 : 19
    __IO uint32_t RX_DATA_DONE_INT_RAW_STATUS    : 1  ; // 20 : 20
    __IO uint32_t SHORT_GND_DET_INT_RAW_STATUS   : 1  ; // 21 : 21
    __IO uint32_t RX_FIFO_VLD_INT_RAW_STATUS     : 1  ; // 22 : 22
    __IO uint32_t RESERVED0                      : 9  ; // 31 : 23
} LIN_SCI1_IRSR_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t TX_DATA                          ; // 0x0000
        __IO  LIN_SCI1_TX_DATA_FIELD_T TX_DATA_F        ;
    };

    union
    {
        __IO  uint32_t RX_DATA                          ; // 0x0004
        __IO  LIN_SCI1_RX_DATA_FIELD_T RX_DATA_F        ;
    };

    union
    {
        __IO  uint32_t CTRL                             ; // 0x0008
        __IO  LIN_SCI1_CTRL_FIELD_T CTRL_F              ;
    };

    union
    {
        __IO  uint32_t CTRL_UART                        ; // 0x000C
        __IO  LIN_SCI1_CTRL_UART_FIELD_T CTRL_UART_F    ;
    };

    uint32_t RESERVED0                                  ;

    union
    {
        __IO  uint32_t BAUD_CFG                         ; // 0x0014
        __IO  LIN_SCI1_BAUD_CFG_FIELD_T BAUD_CFG_F      ;
    };

    union
    {
        __IO  uint32_t BRK_SYNC_CFG                     ; // 0x0018
        __IO  LIN_SCI1_BRK_SYNC_CFG_FIELD_T BRK_SYNC_CFG_F;
    };

    union
    {
        __IO  uint32_t TX_CFG                           ; // 0x001C
        __IO  LIN_SCI1_TX_CFG_FIELD_T TX_CFG_F          ;
    };

    union
    {
        __IO  uint32_t RX_CFG                           ; // 0x0020
        __IO  LIN_SCI1_RX_CFG_FIELD_T RX_CFG_F          ;
    };

    union
    {
        __IO  uint32_t RX_FILTER_CFG                    ; // 0x0024
        __IO  LIN_SCI1_RX_FILTER_CFG_FIELD_T RX_FILTER_CFG_F;
    };

    union
    {
        __IO  uint32_t STATUS                           ; // 0x0028
        __IO  LIN_SCI1_STATUS_FIELD_T STATUS_F          ;
    };

    union
    {
        __IO  uint32_t AUTO_BAUD_VAL                    ; // 0x002C
        __IO  LIN_SCI1_AUTO_BAUD_VAL_FIELD_T AUTO_BAUD_VAL_F;
    };

    union
    {
        __IO  uint32_t RX_PID                           ; // 0x0030
        __IO  LIN_SCI1_RX_PID_FIELD_T RX_PID_F          ;
    };

    union
    {
        __IO  uint32_t CHKSUM_VAL                       ; // 0x0034
        __IO  LIN_SCI1_CHKSUM_VAL_FIELD_T CHKSUM_VAL_F  ;
    };

    uint32_t RESERVED1[9]                               ;

    union
    {
        __IO  uint32_t ICR                              ; // 0x005C
        __IO  LIN_SCI1_ICR_FIELD_T ICR_F                ;
    };

    union
    {
        __IO  uint32_t IMR                              ; // 0x0060
        __IO  LIN_SCI1_IMR_FIELD_T IMR_F                ;
    };

    union
    {
        __IO  uint32_t ISR                              ; // 0x0064
        __IO  LIN_SCI1_ISR_FIELD_T ISR_F                ;
    };

    union
    {
        __IO  uint32_t IRSR                             ; // 0x0068
        __IO  LIN_SCI1_IRSR_FIELD_T IRSR_F              ;
    };

} LIN_SCI1_REG_TypeDef;

#endif /* __LIN_SCI1_DEF_H__ */
