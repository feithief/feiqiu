/**
 *****************************************************************************
 * @brief   eflash_reg_def header file.
 *
 * @file    eflash_reg_def.h
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

#ifndef __EFLASH_DEF_H__
#define __EFLASH_DEF_H__

// Register Offset Macro Definition
#define EFLASH_REVISION_ID_OFFSET                               0x0000
#define EFLASH_RD_TIME_CFG_OFFSET                               0x0004
#define EFLASH_RCV_TIME_CFG_OFFSET                              0x0008
#define EFLASH_NVS_TIME_CFG_OFFSET                              0x000C
#define EFLASH_PROG_TIME_CFG_OFFSET                             0x0010
#define EFLASH_ERASE_TIME_CFG_OFFSET                            0x0014
#define EFLASH_LATENCY_TIME_CFG_OFFSET                          0x0018
#define EFLASH_ERASE_CFG_OFFSET                                 0x001C
#define EFLASH_ERASE_TRIG_OFFSET                                0x0020
#define EFLASH_MISC_CTRL_OFFSET                                 0x0024
#define EFLASH_OP_CTRL_OFFSET                                   0x0028
#define EFLASH_PROG_ADDR_PROT0_OFFSET                           0x002C
#define EFLASH_PROG_ADDR_PROT1_OFFSET                           0x0030
#define EFLASH_ICR_OFFSET                                       0x0034
#define EFLASH_IMR_OFFSET                                       0x0038
#define EFLASH_ISR_OFFSET                                       0x003C
#define EFLASH_IRSR_OFFSET                                      0x0040
#define EFLASH_ERR_ADDR_OFFSET                                  0x0044
#define EFLASH_WR_BUF_STATUS_OFFSET                             0x0048
#define EFLASH_STATUS_OFFSET                                    0x004C
#define EFLASH_CELL_INIT_CTRL_OFFSET                            0x0050
#define EFLASH_WR_LOCK_OFFSET                                   0x00F8
#define EFLASH_NVR_PROT_OFFSET                                  0x00FC

// Register Address Macro Definition
#define EFLASH_REVISION_ID_ADDR                                 ( EFLASH_BASE_ADDR + EFLASH_REVISION_ID_OFFSET)
#define EFLASH_RD_TIME_CFG_ADDR                                 ( EFLASH_BASE_ADDR + EFLASH_RD_TIME_CFG_OFFSET)
#define EFLASH_RCV_TIME_CFG_ADDR                                ( EFLASH_BASE_ADDR + EFLASH_RCV_TIME_CFG_OFFSET)
#define EFLASH_NVS_TIME_CFG_ADDR                                ( EFLASH_BASE_ADDR + EFLASH_NVS_TIME_CFG_OFFSET)
#define EFLASH_PROG_TIME_CFG_ADDR                               ( EFLASH_BASE_ADDR + EFLASH_PROG_TIME_CFG_OFFSET)
#define EFLASH_ERASE_TIME_CFG_ADDR                              ( EFLASH_BASE_ADDR + EFLASH_ERASE_TIME_CFG_OFFSET)
#define EFLASH_LATENCY_TIME_CFG_ADDR                            ( EFLASH_BASE_ADDR + EFLASH_LATENCY_TIME_CFG_OFFSET)
#define EFLASH_ERASE_CFG_ADDR                                   ( EFLASH_BASE_ADDR + EFLASH_ERASE_CFG_OFFSET)
#define EFLASH_ERASE_TRIG_ADDR                                  ( EFLASH_BASE_ADDR + EFLASH_ERASE_TRIG_OFFSET)
#define EFLASH_MISC_CTRL_ADDR                                   ( EFLASH_BASE_ADDR + EFLASH_MISC_CTRL_OFFSET)
#define EFLASH_OP_CTRL_ADDR                                     ( EFLASH_BASE_ADDR + EFLASH_OP_CTRL_OFFSET)
#define EFLASH_PROG_ADDR_PROT0_ADDR                             ( EFLASH_BASE_ADDR + EFLASH_PROG_ADDR_PROT0_OFFSET)
#define EFLASH_PROG_ADDR_PROT1_ADDR                             ( EFLASH_BASE_ADDR + EFLASH_PROG_ADDR_PROT1_OFFSET)
#define EFLASH_ICR_ADDR                                         ( EFLASH_BASE_ADDR + EFLASH_ICR_OFFSET)
#define EFLASH_IMR_ADDR                                         ( EFLASH_BASE_ADDR + EFLASH_IMR_OFFSET)
#define EFLASH_ISR_ADDR                                         ( EFLASH_BASE_ADDR + EFLASH_ISR_OFFSET)
#define EFLASH_IRSR_ADDR                                        ( EFLASH_BASE_ADDR + EFLASH_IRSR_OFFSET)
#define EFLASH_ERR_ADDR_ADDR                                    ( EFLASH_BASE_ADDR + EFLASH_ERR_ADDR_OFFSET)
#define EFLASH_WR_BUF_STATUS_ADDR                               ( EFLASH_BASE_ADDR + EFLASH_WR_BUF_STATUS_OFFSET)
#define EFLASH_STATUS_ADDR                                      ( EFLASH_BASE_ADDR + EFLASH_STATUS_OFFSET)
#define EFLASH_CELL_INIT_CTRL_ADDR                              ( EFLASH_BASE_ADDR + EFLASH_CELL_INIT_CTRL_OFFSET)
#define EFLASH_WR_LOCK_ADDR                                     ( EFLASH_BASE_ADDR + EFLASH_WR_LOCK_OFFSET)
#define EFLASH_NVR_PROT_ADDR                                    ( EFLASH_BASE_ADDR + EFLASH_NVR_PROT_OFFSET)

// Register Field Macro Definition
#define EFLASH_REVISION_ID_REVISION_ID_SHIFT                                                 0
#define EFLASH_REVISION_ID_REVISION_ID_MASK                                                  0x000000FF
#define EFLASH_REVISION_ID_REVISION_ID_SET(n)                                                (((uint32_t)(n) << 0  ) & 0x000000FF)
#define EFLASH_REVISION_ID_REVISION_ID_CLR                                                   0xFFFFFF00

#define EFLASH_RD_TIME_CFG_CONFEN_CYC_SHIFT                                                  12
#define EFLASH_RD_TIME_CFG_CONFEN_CYC_MASK                                                   0x00003000
#define EFLASH_RD_TIME_CFG_CONFEN_CYC_SET(n)                                                 (((uint32_t)(n) << 12 ) & 0x00003000)
#define EFLASH_RD_TIME_CFG_CONFEN_CYC_CLR                                                    0xFFFFCFFF

#define EFLASH_RD_TIME_CFG_CFS_CFH_CYC_SHIFT                                                 4
#define EFLASH_RD_TIME_CFG_CFS_CFH_CYC_MASK                                                  0x00000030
#define EFLASH_RD_TIME_CFG_CFS_CFH_CYC_SET(n)                                                (((uint32_t)(n) << 4  ) & 0x00000030)
#define EFLASH_RD_TIME_CFG_CFS_CFH_CYC_CLR                                                   0xFFFFFFCF

#define EFLASH_RD_TIME_CFG_RC_CYC_SHIFT                                                      0
#define EFLASH_RD_TIME_CFG_RC_CYC_MASK                                                       0x00000003
#define EFLASH_RD_TIME_CFG_RC_CYC_SET(n)                                                     (((uint32_t)(n) << 0  ) & 0x00000003)
#define EFLASH_RD_TIME_CFG_RC_CYC_CLR                                                        0xFFFFFFFC

#define EFLASH_RCV_TIME_CFG_ECHIP_RCV_CYC_SHIFT                                              8
#define EFLASH_RCV_TIME_CFG_ECHIP_RCV_CYC_MASK                                               0x0003FF00
#define EFLASH_RCV_TIME_CFG_ECHIP_RCV_CYC_SET(n)                                             (((uint32_t)(n) << 8  ) & 0x0003FF00)
#define EFLASH_RCV_TIME_CFG_ECHIP_RCV_CYC_CLR                                                0xFFFC00FF

#define EFLASH_RCV_TIME_CFG_PROG_ESECT_RCV_CYC_SHIFT                                         0
#define EFLASH_RCV_TIME_CFG_PROG_ESECT_RCV_CYC_MASK                                          0x000000FF
#define EFLASH_RCV_TIME_CFG_PROG_ESECT_RCV_CYC_SET(n)                                        (((uint32_t)(n) << 0  ) & 0x000000FF)
#define EFLASH_RCV_TIME_CFG_PROG_ESECT_RCV_CYC_CLR                                           0xFFFFFF00

#define EFLASH_NVS_TIME_CFG_ECHIP_NVS_CYC_SHIFT                                              8
#define EFLASH_NVS_TIME_CFG_ECHIP_NVS_CYC_MASK                                               0x0001FF00
#define EFLASH_NVS_TIME_CFG_ECHIP_NVS_CYC_SET(n)                                             (((uint32_t)(n) << 8  ) & 0x0001FF00)
#define EFLASH_NVS_TIME_CFG_ECHIP_NVS_CYC_CLR                                                0xFFFE00FF

#define EFLASH_NVS_TIME_CFG_PROG_ESECT_NVS_CYC_SHIFT                                         0
#define EFLASH_NVS_TIME_CFG_PROG_ESECT_NVS_CYC_MASK                                          0x0000007F
#define EFLASH_NVS_TIME_CFG_PROG_ESECT_NVS_CYC_SET(n)                                        (((uint32_t)(n) << 0  ) & 0x0000007F)
#define EFLASH_NVS_TIME_CFG_PROG_ESECT_NVS_CYC_CLR                                           0xFFFFFF80

#define EFLASH_PROG_TIME_CFG_PROG_CYC_SHIFT                                                  16
#define EFLASH_PROG_TIME_CFG_PROG_CYC_MASK                                                   0x001F0000
#define EFLASH_PROG_TIME_CFG_PROG_CYC_SET(n)                                                 (((uint32_t)(n) << 16 ) & 0x001F0000)
#define EFLASH_PROG_TIME_CFG_PROG_CYC_CLR                                                    0xFFE0FFFF

#define EFLASH_PROG_TIME_CFG_ADH_ADS_PGH_CYC_SHIFT                                           8
#define EFLASH_PROG_TIME_CFG_ADH_ADS_PGH_CYC_MASK                                            0x00001F00
#define EFLASH_PROG_TIME_CFG_ADH_ADS_PGH_CYC_SET(n)                                          (((uint32_t)(n) << 8  ) & 0x00001F00)
#define EFLASH_PROG_TIME_CFG_ADH_ADS_PGH_CYC_CLR                                             0xFFFFE0FF

#define EFLASH_PROG_TIME_CFG_PGS_CYC_SHIFT                                                   0
#define EFLASH_PROG_TIME_CFG_PGS_CYC_MASK                                                    0x000000FF
#define EFLASH_PROG_TIME_CFG_PGS_CYC_SET(n)                                                  (((uint32_t)(n) << 0  ) & 0x000000FF)
#define EFLASH_PROG_TIME_CFG_PGS_CYC_CLR                                                     0xFFFFFF00

#define EFLASH_ERASE_TIME_CFG_ERASE_CHIP_CYC_SHIFT                                           4
#define EFLASH_ERASE_TIME_CFG_ERASE_CHIP_CYC_MASK                                            0x00000FF0
#define EFLASH_ERASE_TIME_CFG_ERASE_CHIP_CYC_SET(n)                                          (((uint32_t)(n) << 4  ) & 0x00000FF0)
#define EFLASH_ERASE_TIME_CFG_ERASE_CHIP_CYC_CLR                                             0xFFFFF00F

#define EFLASH_ERASE_TIME_CFG_ERASE_SECT_CYC_SHIFT                                           0
#define EFLASH_ERASE_TIME_CFG_ERASE_SECT_CYC_MASK                                            0x0000000F
#define EFLASH_ERASE_TIME_CFG_ERASE_SECT_CYC_SET(n)                                          (((uint32_t)(n) << 0  ) & 0x0000000F)
#define EFLASH_ERASE_TIME_CFG_ERASE_SECT_CYC_CLR                                             0xFFFFFFF0

#define EFLASH_LATENCY_TIME_CFG_LATENCY_ECHIP_CYC_SHIFT                                      8
#define EFLASH_LATENCY_TIME_CFG_LATENCY_ECHIP_CYC_MASK                                       0x00003F00
#define EFLASH_LATENCY_TIME_CFG_LATENCY_ECHIP_CYC_SET(n)                                     (((uint32_t)(n) << 8  ) & 0x00003F00)
#define EFLASH_LATENCY_TIME_CFG_LATENCY_ECHIP_CYC_CLR                                        0xFFFFC0FF

#define EFLASH_LATENCY_TIME_CFG_LATENCY_ES_PROG_CYC_SHIFT                                    0
#define EFLASH_LATENCY_TIME_CFG_LATENCY_ES_PROG_CYC_MASK                                     0x0000001F
#define EFLASH_LATENCY_TIME_CFG_LATENCY_ES_PROG_CYC_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x0000001F)
#define EFLASH_LATENCY_TIME_CFG_LATENCY_ES_PROG_CYC_CLR                                      0xFFFFFFE0

#define EFLASH_ERASE_CFG_NVR_SECTOR_SEL_SHIFT                                                24
#define EFLASH_ERASE_CFG_NVR_SECTOR_SEL_MASK                                                 0x01000000
#define EFLASH_ERASE_CFG_NVR_SECTOR_SEL_SET(n)                                               (((uint32_t)(n) << 24 ) & 0x01000000)
#define EFLASH_ERASE_CFG_NVR_SECTOR_SEL_CLR                                                  0xFEFFFFFF

#define EFLASH_ERASE_CFG_SECTOR_INDEX_SHIFT                                                  0
#define EFLASH_ERASE_CFG_SECTOR_INDEX_MASK                                                   0x0000007F
#define EFLASH_ERASE_CFG_SECTOR_INDEX_SET(n)                                                 (((uint32_t)(n) << 0  ) & 0x0000007F)
#define EFLASH_ERASE_CFG_SECTOR_INDEX_CLR                                                    0xFFFFFF80

#define EFLASH_ERASE_TRIG_CHIP_ERASE_TRIG_SHIFT                                              16
#define EFLASH_ERASE_TRIG_CHIP_ERASE_TRIG_MASK                                               0x00010000
#define EFLASH_ERASE_TRIG_CHIP_ERASE_TRIG_SET(n)                                             (((uint32_t)(n) << 16 ) & 0x00010000)
#define EFLASH_ERASE_TRIG_CHIP_ERASE_TRIG_CLR                                                0xFFFEFFFF

#define EFLASH_ERASE_TRIG_SECTOR_ERASE_TRIG_SHIFT                                            0
#define EFLASH_ERASE_TRIG_SECTOR_ERASE_TRIG_MASK                                             0x00000001
#define EFLASH_ERASE_TRIG_SECTOR_ERASE_TRIG_SET(n)                                           (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_ERASE_TRIG_SECTOR_ERASE_TRIG_CLR                                              0xFFFFFFFE

#define EFLASH_MISC_CTRL_WR_BUF_CLR_SHIFT                                                    4
#define EFLASH_MISC_CTRL_WR_BUF_CLR_MASK                                                     0x00000010
#define EFLASH_MISC_CTRL_WR_BUF_CLR_SET(n)                                                   (((uint32_t)(n) << 4  ) & 0x00000010)
#define EFLASH_MISC_CTRL_WR_BUF_CLR_CLR                                                      0xFFFFFFEF

#define EFLASH_MISC_CTRL_DEEP_STANDBY_SHIFT                                                  0
#define EFLASH_MISC_CTRL_DEEP_STANDBY_MASK                                                   0x00000001
#define EFLASH_MISC_CTRL_DEEP_STANDBY_SET(n)                                                 (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_MISC_CTRL_DEEP_STANDBY_CLR                                                    0xFFFFFFFE

#define EFLASH_OP_CTRL_CFG_WR_EN_SHIFT                                                       5
#define EFLASH_OP_CTRL_CFG_WR_EN_MASK                                                        0x00000020
#define EFLASH_OP_CTRL_CFG_WR_EN_SET(n)                                                      (((uint32_t)(n) << 5  ) & 0x00000020)
#define EFLASH_OP_CTRL_CFG_WR_EN_CLR                                                         0xFFFFFFDF

#define EFLASH_OP_CTRL_RDONLY_EN_SHIFT                                                       4
#define EFLASH_OP_CTRL_RDONLY_EN_MASK                                                        0x00000010
#define EFLASH_OP_CTRL_RDONLY_EN_SET(n)                                                      (((uint32_t)(n) << 4  ) & 0x00000010)
#define EFLASH_OP_CTRL_RDONLY_EN_CLR                                                         0xFFFFFFEF

#define EFLASH_OP_CTRL_PROG_RD_CHK_EN_SHIFT                                                  2
#define EFLASH_OP_CTRL_PROG_RD_CHK_EN_MASK                                                   0x00000004
#define EFLASH_OP_CTRL_PROG_RD_CHK_EN_SET(n)                                                 (((uint32_t)(n) << 2  ) & 0x00000004)
#define EFLASH_OP_CTRL_PROG_RD_CHK_EN_CLR                                                    0xFFFFFFFB

#define EFLASH_OP_CTRL_PROG_ADDR_CHK_EN1_SHIFT                                               1
#define EFLASH_OP_CTRL_PROG_ADDR_CHK_EN1_MASK                                                0x00000002
#define EFLASH_OP_CTRL_PROG_ADDR_CHK_EN1_SET(n)                                              (((uint32_t)(n) << 1  ) & 0x00000002)
#define EFLASH_OP_CTRL_PROG_ADDR_CHK_EN1_CLR                                                 0xFFFFFFFD

#define EFLASH_OP_CTRL_PROG_ADDR_CHK_EN0_SHIFT                                               0
#define EFLASH_OP_CTRL_PROG_ADDR_CHK_EN0_MASK                                                0x00000001
#define EFLASH_OP_CTRL_PROG_ADDR_CHK_EN0_SET(n)                                              (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_OP_CTRL_PROG_ADDR_CHK_EN0_CLR                                                 0xFFFFFFFE

#define EFLASH_PROG_ADDR_PROT0_PROG_ADDR_PROT_END0_SHIFT                                     8
#define EFLASH_PROG_ADDR_PROT0_PROG_ADDR_PROT_END0_MASK                                      0x00007F00
#define EFLASH_PROG_ADDR_PROT0_PROG_ADDR_PROT_END0_SET(n)                                    (((uint32_t)(n) << 8  ) & 0x00007F00)
#define EFLASH_PROG_ADDR_PROT0_PROG_ADDR_PROT_END0_CLR                                       0xFFFF80FF

#define EFLASH_PROG_ADDR_PROT0_PROG_ADDR_PROT_START0_SHIFT                                   0
#define EFLASH_PROG_ADDR_PROT0_PROG_ADDR_PROT_START0_MASK                                    0x0000007F
#define EFLASH_PROG_ADDR_PROT0_PROG_ADDR_PROT_START0_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x0000007F)
#define EFLASH_PROG_ADDR_PROT0_PROG_ADDR_PROT_START0_CLR                                     0xFFFFFF80

#define EFLASH_PROG_ADDR_PROT1_PROG_ADDR_PROT_END1_SHIFT                                     8
#define EFLASH_PROG_ADDR_PROT1_PROG_ADDR_PROT_END1_MASK                                      0x00007F00
#define EFLASH_PROG_ADDR_PROT1_PROG_ADDR_PROT_END1_SET(n)                                    (((uint32_t)(n) << 8  ) & 0x00007F00)
#define EFLASH_PROG_ADDR_PROT1_PROG_ADDR_PROT_END1_CLR                                       0xFFFF80FF

#define EFLASH_PROG_ADDR_PROT1_PROG_ADDR_PROT_START1_SHIFT                                   0
#define EFLASH_PROG_ADDR_PROT1_PROG_ADDR_PROT_START1_MASK                                    0x0000007F
#define EFLASH_PROG_ADDR_PROT1_PROG_ADDR_PROT_START1_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x0000007F)
#define EFLASH_PROG_ADDR_PROT1_PROG_ADDR_PROT_START1_CLR                                     0xFFFFFF80

#define EFLASH_ICR_NVM_NVR_WR_SIZE_ERR_INT_CLR_SHIFT                                         8
#define EFLASH_ICR_NVM_NVR_WR_SIZE_ERR_INT_CLR_MASK                                          0x00000100
#define EFLASH_ICR_NVM_NVR_WR_SIZE_ERR_INT_CLR_SET(n)                                        (((uint32_t)(n) << 8  ) & 0x00000100)
#define EFLASH_ICR_NVM_NVR_WR_SIZE_ERR_INT_CLR_CLR                                           0xFFFFFEFF

#define EFLASH_ICR_NVM_NVR_WR_ERR_INT_CLR_SHIFT                                              7
#define EFLASH_ICR_NVM_NVR_WR_ERR_INT_CLR_MASK                                               0x00000080
#define EFLASH_ICR_NVM_NVR_WR_ERR_INT_CLR_SET(n)                                             (((uint32_t)(n) << 7  ) & 0x00000080)
#define EFLASH_ICR_NVM_NVR_WR_ERR_INT_CLR_CLR                                                0xFFFFFF7F

#define EFLASH_ICR_ECC_2BIT_ERR_INT_CLR_SHIFT                                                6
#define EFLASH_ICR_ECC_2BIT_ERR_INT_CLR_MASK                                                 0x00000040
#define EFLASH_ICR_ECC_2BIT_ERR_INT_CLR_SET(n)                                               (((uint32_t)(n) << 6  ) & 0x00000040)
#define EFLASH_ICR_ECC_2BIT_ERR_INT_CLR_CLR                                                  0xFFFFFFBF

#define EFLASH_ICR_ECC_1BIT_ERR_INT_CLR_SHIFT                                                5
#define EFLASH_ICR_ECC_1BIT_ERR_INT_CLR_MASK                                                 0x00000020
#define EFLASH_ICR_ECC_1BIT_ERR_INT_CLR_SET(n)                                               (((uint32_t)(n) << 5  ) & 0x00000020)
#define EFLASH_ICR_ECC_1BIT_ERR_INT_CLR_CLR                                                  0xFFFFFFDF

#define EFLASH_ICR_ERASE_DONE_INT_CLR_SHIFT                                                  4
#define EFLASH_ICR_ERASE_DONE_INT_CLR_MASK                                                   0x00000010
#define EFLASH_ICR_ERASE_DONE_INT_CLR_SET(n)                                                 (((uint32_t)(n) << 4  ) & 0x00000010)
#define EFLASH_ICR_ERASE_DONE_INT_CLR_CLR                                                    0xFFFFFFEF

#define EFLASH_ICR_PROG_DONE_INT_CLR_SHIFT                                                   3
#define EFLASH_ICR_PROG_DONE_INT_CLR_MASK                                                    0x00000008
#define EFLASH_ICR_PROG_DONE_INT_CLR_SET(n)                                                  (((uint32_t)(n) << 3  ) & 0x00000008)
#define EFLASH_ICR_PROG_DONE_INT_CLR_CLR                                                     0xFFFFFFF7

#define EFLASH_ICR_PROG_RD_CHK_ERR_INT_CLR_SHIFT                                             2
#define EFLASH_ICR_PROG_RD_CHK_ERR_INT_CLR_MASK                                              0x00000004
#define EFLASH_ICR_PROG_RD_CHK_ERR_INT_CLR_SET(n)                                            (((uint32_t)(n) << 2  ) & 0x00000004)
#define EFLASH_ICR_PROG_RD_CHK_ERR_INT_CLR_CLR                                               0xFFFFFFFB

#define EFLASH_ICR_NVR_ACCESS_ERR_INT_CLR_SHIFT                                              1
#define EFLASH_ICR_NVR_ACCESS_ERR_INT_CLR_MASK                                               0x00000002
#define EFLASH_ICR_NVR_ACCESS_ERR_INT_CLR_SET(n)                                             (((uint32_t)(n) << 1  ) & 0x00000002)
#define EFLASH_ICR_NVR_ACCESS_ERR_INT_CLR_CLR                                                0xFFFFFFFD

#define EFLASH_ICR_NVM_PROG_ADDR_CHK_ERR_INT_CLR_SHIFT                                       0
#define EFLASH_ICR_NVM_PROG_ADDR_CHK_ERR_INT_CLR_MASK                                        0x00000001
#define EFLASH_ICR_NVM_PROG_ADDR_CHK_ERR_INT_CLR_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_ICR_NVM_PROG_ADDR_CHK_ERR_INT_CLR_CLR                                         0xFFFFFFFE

#define EFLASH_IMR_NVM_NVR_WR_SIZE_ERR_INT_MSK_SHIFT                                         8
#define EFLASH_IMR_NVM_NVR_WR_SIZE_ERR_INT_MSK_MASK                                          0x00000100
#define EFLASH_IMR_NVM_NVR_WR_SIZE_ERR_INT_MSK_SET(n)                                        (((uint32_t)(n) << 8  ) & 0x00000100)
#define EFLASH_IMR_NVM_NVR_WR_SIZE_ERR_INT_MSK_CLR                                           0xFFFFFEFF

#define EFLASH_IMR_NVM_NVR_WR_ERR_INT_MSK_SHIFT                                              7
#define EFLASH_IMR_NVM_NVR_WR_ERR_INT_MSK_MASK                                               0x00000080
#define EFLASH_IMR_NVM_NVR_WR_ERR_INT_MSK_SET(n)                                             (((uint32_t)(n) << 7  ) & 0x00000080)
#define EFLASH_IMR_NVM_NVR_WR_ERR_INT_MSK_CLR                                                0xFFFFFF7F

#define EFLASH_IMR_ECC_2BIT_ERR_INT_MSK_SHIFT                                                6
#define EFLASH_IMR_ECC_2BIT_ERR_INT_MSK_MASK                                                 0x00000040
#define EFLASH_IMR_ECC_2BIT_ERR_INT_MSK_SET(n)                                               (((uint32_t)(n) << 6  ) & 0x00000040)
#define EFLASH_IMR_ECC_2BIT_ERR_INT_MSK_CLR                                                  0xFFFFFFBF

#define EFLASH_IMR_ECC_1BIT_ERR_INT_MSK_SHIFT                                                5
#define EFLASH_IMR_ECC_1BIT_ERR_INT_MSK_MASK                                                 0x00000020
#define EFLASH_IMR_ECC_1BIT_ERR_INT_MSK_SET(n)                                               (((uint32_t)(n) << 5  ) & 0x00000020)
#define EFLASH_IMR_ECC_1BIT_ERR_INT_MSK_CLR                                                  0xFFFFFFDF

#define EFLASH_IMR_ERASE_DONE_INT_MSK_SHIFT                                                  4
#define EFLASH_IMR_ERASE_DONE_INT_MSK_MASK                                                   0x00000010
#define EFLASH_IMR_ERASE_DONE_INT_MSK_SET(n)                                                 (((uint32_t)(n) << 4  ) & 0x00000010)
#define EFLASH_IMR_ERASE_DONE_INT_MSK_CLR                                                    0xFFFFFFEF

#define EFLASH_IMR_PROG_DONE_INT_MSK_SHIFT                                                   3
#define EFLASH_IMR_PROG_DONE_INT_MSK_MASK                                                    0x00000008
#define EFLASH_IMR_PROG_DONE_INT_MSK_SET(n)                                                  (((uint32_t)(n) << 3  ) & 0x00000008)
#define EFLASH_IMR_PROG_DONE_INT_MSK_CLR                                                     0xFFFFFFF7

#define EFLASH_IMR_PROG_RD_CHK_ERR_INT_MSK_SHIFT                                             2
#define EFLASH_IMR_PROG_RD_CHK_ERR_INT_MSK_MASK                                              0x00000004
#define EFLASH_IMR_PROG_RD_CHK_ERR_INT_MSK_SET(n)                                            (((uint32_t)(n) << 2  ) & 0x00000004)
#define EFLASH_IMR_PROG_RD_CHK_ERR_INT_MSK_CLR                                               0xFFFFFFFB

#define EFLASH_IMR_NVR_ACCESS_ERR_INT_MSK_SHIFT                                              1
#define EFLASH_IMR_NVR_ACCESS_ERR_INT_MSK_MASK                                               0x00000002
#define EFLASH_IMR_NVR_ACCESS_ERR_INT_MSK_SET(n)                                             (((uint32_t)(n) << 1  ) & 0x00000002)
#define EFLASH_IMR_NVR_ACCESS_ERR_INT_MSK_CLR                                                0xFFFFFFFD

#define EFLASH_IMR_NVM_PROG_ADDR_CHK_ERR_INT_MSK_SHIFT                                       0
#define EFLASH_IMR_NVM_PROG_ADDR_CHK_ERR_INT_MSK_MASK                                        0x00000001
#define EFLASH_IMR_NVM_PROG_ADDR_CHK_ERR_INT_MSK_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_IMR_NVM_PROG_ADDR_CHK_ERR_INT_MSK_CLR                                         0xFFFFFFFE

#define EFLASH_ISR_NVM_NVR_WR_SIZE_ERR_INT_STATUS_SHIFT                                      8
#define EFLASH_ISR_NVM_NVR_WR_SIZE_ERR_INT_STATUS_MASK                                       0x00000100
#define EFLASH_ISR_NVM_NVR_WR_SIZE_ERR_INT_STATUS_SET(n)                                     (((uint32_t)(n) << 8  ) & 0x00000100)
#define EFLASH_ISR_NVM_NVR_WR_SIZE_ERR_INT_STATUS_CLR                                        0xFFFFFEFF

#define EFLASH_ISR_NVM_NVR_WR_ERR_INT_STATUS_SHIFT                                           7
#define EFLASH_ISR_NVM_NVR_WR_ERR_INT_STATUS_MASK                                            0x00000080
#define EFLASH_ISR_NVM_NVR_WR_ERR_INT_STATUS_SET(n)                                          (((uint32_t)(n) << 7  ) & 0x00000080)
#define EFLASH_ISR_NVM_NVR_WR_ERR_INT_STATUS_CLR                                             0xFFFFFF7F

#define EFLASH_ISR_ECC_2BIT_ERR_INT_STATUS_SHIFT                                             6
#define EFLASH_ISR_ECC_2BIT_ERR_INT_STATUS_MASK                                              0x00000040
#define EFLASH_ISR_ECC_2BIT_ERR_INT_STATUS_SET(n)                                            (((uint32_t)(n) << 6  ) & 0x00000040)
#define EFLASH_ISR_ECC_2BIT_ERR_INT_STATUS_CLR                                               0xFFFFFFBF

#define EFLASH_ISR_ECC_1BIT_ERR_INT_STATUS_SHIFT                                             5
#define EFLASH_ISR_ECC_1BIT_ERR_INT_STATUS_MASK                                              0x00000020
#define EFLASH_ISR_ECC_1BIT_ERR_INT_STATUS_SET(n)                                            (((uint32_t)(n) << 5  ) & 0x00000020)
#define EFLASH_ISR_ECC_1BIT_ERR_INT_STATUS_CLR                                               0xFFFFFFDF

#define EFLASH_ISR_ERASE_DONE_INT_STATUS_SHIFT                                               4
#define EFLASH_ISR_ERASE_DONE_INT_STATUS_MASK                                                0x00000010
#define EFLASH_ISR_ERASE_DONE_INT_STATUS_SET(n)                                              (((uint32_t)(n) << 4  ) & 0x00000010)
#define EFLASH_ISR_ERASE_DONE_INT_STATUS_CLR                                                 0xFFFFFFEF

#define EFLASH_ISR_PROG_DONE_INT_STATUS_SHIFT                                                3
#define EFLASH_ISR_PROG_DONE_INT_STATUS_MASK                                                 0x00000008
#define EFLASH_ISR_PROG_DONE_INT_STATUS_SET(n)                                               (((uint32_t)(n) << 3  ) & 0x00000008)
#define EFLASH_ISR_PROG_DONE_INT_STATUS_CLR                                                  0xFFFFFFF7

#define EFLASH_ISR_PROG_RD_CHK_ERR_INT_STATUS_SHIFT                                          2
#define EFLASH_ISR_PROG_RD_CHK_ERR_INT_STATUS_MASK                                           0x00000004
#define EFLASH_ISR_PROG_RD_CHK_ERR_INT_STATUS_SET(n)                                         (((uint32_t)(n) << 2  ) & 0x00000004)
#define EFLASH_ISR_PROG_RD_CHK_ERR_INT_STATUS_CLR                                            0xFFFFFFFB

#define EFLASH_ISR_NVR_ACCESS_ERR_INT_STATUS_SHIFT                                           1
#define EFLASH_ISR_NVR_ACCESS_ERR_INT_STATUS_MASK                                            0x00000002
#define EFLASH_ISR_NVR_ACCESS_ERR_INT_STATUS_SET(n)                                          (((uint32_t)(n) << 1  ) & 0x00000002)
#define EFLASH_ISR_NVR_ACCESS_ERR_INT_STATUS_CLR                                             0xFFFFFFFD

#define EFLASH_ISR_NVM_PROG_ADDR_CHK_ERR_INT_STATUS_SHIFT                                    0
#define EFLASH_ISR_NVM_PROG_ADDR_CHK_ERR_INT_STATUS_MASK                                     0x00000001
#define EFLASH_ISR_NVM_PROG_ADDR_CHK_ERR_INT_STATUS_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_ISR_NVM_PROG_ADDR_CHK_ERR_INT_STATUS_CLR                                      0xFFFFFFFE

#define EFLASH_IRSR_NVM_NVR_WR_SIZE_ERR_INT_RAW_STATUS_SHIFT                                 8
#define EFLASH_IRSR_NVM_NVR_WR_SIZE_ERR_INT_RAW_STATUS_MASK                                  0x00000100
#define EFLASH_IRSR_NVM_NVR_WR_SIZE_ERR_INT_RAW_STATUS_SET(n)                                (((uint32_t)(n) << 8  ) & 0x00000100)
#define EFLASH_IRSR_NVM_NVR_WR_SIZE_ERR_INT_RAW_STATUS_CLR                                   0xFFFFFEFF

#define EFLASH_IRSR_NVM_NVR_WR_ERR_INT_RAW_STATUS_SHIFT                                      7
#define EFLASH_IRSR_NVM_NVR_WR_ERR_INT_RAW_STATUS_MASK                                       0x00000080
#define EFLASH_IRSR_NVM_NVR_WR_ERR_INT_RAW_STATUS_SET(n)                                     (((uint32_t)(n) << 7  ) & 0x00000080)
#define EFLASH_IRSR_NVM_NVR_WR_ERR_INT_RAW_STATUS_CLR                                        0xFFFFFF7F

#define EFLASH_IRSR_ECC_2BIT_ERR_INT_RAW_STATUS_SHIFT                                        6
#define EFLASH_IRSR_ECC_2BIT_ERR_INT_RAW_STATUS_MASK                                         0x00000040
#define EFLASH_IRSR_ECC_2BIT_ERR_INT_RAW_STATUS_SET(n)                                       (((uint32_t)(n) << 6  ) & 0x00000040)
#define EFLASH_IRSR_ECC_2BIT_ERR_INT_RAW_STATUS_CLR                                          0xFFFFFFBF

#define EFLASH_IRSR_ECC_1BIT_ERR_INT_RAW_STATUS_SHIFT                                        5
#define EFLASH_IRSR_ECC_1BIT_ERR_INT_RAW_STATUS_MASK                                         0x00000020
#define EFLASH_IRSR_ECC_1BIT_ERR_INT_RAW_STATUS_SET(n)                                       (((uint32_t)(n) << 5  ) & 0x00000020)
#define EFLASH_IRSR_ECC_1BIT_ERR_INT_RAW_STATUS_CLR                                          0xFFFFFFDF

#define EFLASH_IRSR_ERASE_DONE_INT_RAW_STATUS_SHIFT                                          4
#define EFLASH_IRSR_ERASE_DONE_INT_RAW_STATUS_MASK                                           0x00000010
#define EFLASH_IRSR_ERASE_DONE_INT_RAW_STATUS_SET(n)                                         (((uint32_t)(n) << 4  ) & 0x00000010)
#define EFLASH_IRSR_ERASE_DONE_INT_RAW_STATUS_CLR                                            0xFFFFFFEF

#define EFLASH_IRSR_PROG_DONE_INT_RAW_STATUS_SHIFT                                           3
#define EFLASH_IRSR_PROG_DONE_INT_RAW_STATUS_MASK                                            0x00000008
#define EFLASH_IRSR_PROG_DONE_INT_RAW_STATUS_SET(n)                                          (((uint32_t)(n) << 3  ) & 0x00000008)
#define EFLASH_IRSR_PROG_DONE_INT_RAW_STATUS_CLR                                             0xFFFFFFF7

#define EFLASH_IRSR_PROG_RD_CHK_ERR_INT_RAW_STATUS_SHIFT                                     2
#define EFLASH_IRSR_PROG_RD_CHK_ERR_INT_RAW_STATUS_MASK                                      0x00000004
#define EFLASH_IRSR_PROG_RD_CHK_ERR_INT_RAW_STATUS_SET(n)                                    (((uint32_t)(n) << 2  ) & 0x00000004)
#define EFLASH_IRSR_PROG_RD_CHK_ERR_INT_RAW_STATUS_CLR                                       0xFFFFFFFB

#define EFLASH_IRSR_NVR_ACCESS_ERR_INT_RAW_STATUS_SHIFT                                      1
#define EFLASH_IRSR_NVR_ACCESS_ERR_INT_RAW_STATUS_MASK                                       0x00000002
#define EFLASH_IRSR_NVR_ACCESS_ERR_INT_RAW_STATUS_SET(n)                                     (((uint32_t)(n) << 1  ) & 0x00000002)
#define EFLASH_IRSR_NVR_ACCESS_ERR_INT_RAW_STATUS_CLR                                        0xFFFFFFFD

#define EFLASH_IRSR_NVM_PROG_ADDR_CHK_ERR_INT_RAW_STATUS_SHIFT                               0
#define EFLASH_IRSR_NVM_PROG_ADDR_CHK_ERR_INT_RAW_STATUS_MASK                                0x00000001
#define EFLASH_IRSR_NVM_PROG_ADDR_CHK_ERR_INT_RAW_STATUS_SET(n)                              (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_IRSR_NVM_PROG_ADDR_CHK_ERR_INT_RAW_STATUS_CLR                                 0xFFFFFFFE

#define EFLASH_ERR_ADDR_ERR_ADDR_SHIFT                                                       0
#define EFLASH_ERR_ADDR_ERR_ADDR_MASK                                                        0x00003FFF
#define EFLASH_ERR_ADDR_ERR_ADDR_SET(n)                                                      (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define EFLASH_ERR_ADDR_ERR_ADDR_CLR                                                         0xFFFFC000

#define EFLASH_WR_BUF_STATUS_WR_CNT_VAL_SHIFT                                                16
#define EFLASH_WR_BUF_STATUS_WR_CNT_VAL_MASK                                                 0x00030000
#define EFLASH_WR_BUF_STATUS_WR_CNT_VAL_SET(n)                                               (((uint32_t)(n) << 16 ) & 0x00030000)
#define EFLASH_WR_BUF_STATUS_WR_CNT_VAL_CLR                                                  0xFFFCFFFF

#define EFLASH_WR_BUF_STATUS_BUF_WADDR_SHIFT                                                 0
#define EFLASH_WR_BUF_STATUS_BUF_WADDR_MASK                                                  0x00007FFF
#define EFLASH_WR_BUF_STATUS_BUF_WADDR_SET(n)                                                (((uint32_t)(n) << 0  ) & 0x00007FFF)
#define EFLASH_WR_BUF_STATUS_BUF_WADDR_CLR                                                   0xFFFF8000

#define EFLASH_STATUS_TM_PAT_ECC_2BIT_ERR_STATUS_SHIFT                                       4
#define EFLASH_STATUS_TM_PAT_ECC_2BIT_ERR_STATUS_MASK                                        0x00000010
#define EFLASH_STATUS_TM_PAT_ECC_2BIT_ERR_STATUS_SET(n)                                      (((uint32_t)(n) << 4  ) & 0x00000010)
#define EFLASH_STATUS_TM_PAT_ECC_2BIT_ERR_STATUS_CLR                                         0xFFFFFFEF

#define EFLASH_STATUS_SWD_PAT_ECC_2BIT_ERR_STATUS_SHIFT                                      3
#define EFLASH_STATUS_SWD_PAT_ECC_2BIT_ERR_STATUS_MASK                                       0x00000008
#define EFLASH_STATUS_SWD_PAT_ECC_2BIT_ERR_STATUS_SET(n)                                     (((uint32_t)(n) << 3  ) & 0x00000008)
#define EFLASH_STATUS_SWD_PAT_ECC_2BIT_ERR_STATUS_CLR                                        0xFFFFFFF7

#define EFLASH_STATUS_PROG_BUSY_STATUS_SHIFT                                                 2
#define EFLASH_STATUS_PROG_BUSY_STATUS_MASK                                                  0x00000004
#define EFLASH_STATUS_PROG_BUSY_STATUS_SET(n)                                                (((uint32_t)(n) << 2  ) & 0x00000004)
#define EFLASH_STATUS_PROG_BUSY_STATUS_CLR                                                   0xFFFFFFFB

#define EFLASH_STATUS_ERASE_BUSY_STATUS_SHIFT                                                1
#define EFLASH_STATUS_ERASE_BUSY_STATUS_MASK                                                 0x00000002
#define EFLASH_STATUS_ERASE_BUSY_STATUS_SET(n)                                               (((uint32_t)(n) << 1  ) & 0x00000002)
#define EFLASH_STATUS_ERASE_BUSY_STATUS_CLR                                                  0xFFFFFFFD

#define EFLASH_STATUS_TRIM_TAG_RD_ERR_STATUS_SHIFT                                           0
#define EFLASH_STATUS_TRIM_TAG_RD_ERR_STATUS_MASK                                            0x00000001
#define EFLASH_STATUS_TRIM_TAG_RD_ERR_STATUS_SET(n)                                          (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_STATUS_TRIM_TAG_RD_ERR_STATUS_CLR                                             0xFFFFFFFE

#define EFLASH_CELL_INIT_CTRL_RCELL_SHIFT                                                    1
#define EFLASH_CELL_INIT_CTRL_RCELL_MASK                                                     0x00000002
#define EFLASH_CELL_INIT_CTRL_RCELL_SET(n)                                                   (((uint32_t)(n) << 1  ) & 0x00000002)
#define EFLASH_CELL_INIT_CTRL_RCELL_CLR                                                      0xFFFFFFFD

#define EFLASH_CELL_INIT_CTRL_TERS_SHIFT                                                     0
#define EFLASH_CELL_INIT_CTRL_TERS_MASK                                                      0x00000001
#define EFLASH_CELL_INIT_CTRL_TERS_SET(n)                                                    (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_CELL_INIT_CTRL_TERS_CLR                                                       0xFFFFFFFE

#define EFLASH_WR_LOCK_WR_LOCK_SHIFT                                                         0
#define EFLASH_WR_LOCK_WR_LOCK_MASK                                                          0x00000001
#define EFLASH_WR_LOCK_WR_LOCK_SET(n)                                                        (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_WR_LOCK_WR_LOCK_CLR                                                           0xFFFFFFFE

#define EFLASH_NVR_PROT_NVR_PROT_SHIFT                                                       0
#define EFLASH_NVR_PROT_NVR_PROT_MASK                                                        0x00000001
#define EFLASH_NVR_PROT_NVR_PROT_SET(n)                                                      (((uint32_t)(n) << 0  ) & 0x00000001)
#define EFLASH_NVR_PROT_NVR_PROT_CLR                                                         0xFFFFFFFE

// Register Structure Definition
typedef struct
{
    __IO uint32_t REVISION_ID                          : 8  ; // 7  : 0
    __IO uint32_t RESERVED0                            : 24 ; // 31 : 8
} EFLASH_REVISION_ID_FIELD_T;

typedef struct
{
    __IO uint32_t RC_CYC                               : 2  ; // 1  : 0
    __IO uint32_t RESERVED2                            : 2  ; // 3  : 2
    __IO uint32_t CFS_CFH_CYC                          : 2  ; // 5  : 4
    __IO uint32_t RESERVED1                            : 6  ; // 11 : 6
    __IO uint32_t CONFEN_CYC                           : 2  ; // 13 : 12
    __IO uint32_t RESERVED0                            : 18 ; // 31 : 14
} EFLASH_RD_TIME_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t PROG_ESECT_RCV_CYC                   : 8  ; // 7  : 0
    __IO uint32_t ECHIP_RCV_CYC                        : 10 ; // 17 : 8
    __IO uint32_t RESERVED0                            : 14 ; // 31 : 18
} EFLASH_RCV_TIME_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t PROG_ESECT_NVS_CYC                   : 7  ; // 6  : 0
    __IO uint32_t RESERVED1                            : 1  ; // 7  : 7
    __IO uint32_t ECHIP_NVS_CYC                        : 9  ; // 16 : 8
    __IO uint32_t RESERVED0                            : 15 ; // 31 : 17
} EFLASH_NVS_TIME_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t PGS_CYC                              : 8  ; // 7  : 0
    __IO uint32_t ADH_ADS_PGH_CYC                      : 5  ; // 12 : 8
    __IO uint32_t RESERVED1                            : 3  ; // 15 : 13
    __IO uint32_t PROG_CYC                             : 5  ; // 20 : 16
    __IO uint32_t RESERVED0                            : 11 ; // 31 : 21
} EFLASH_PROG_TIME_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t ERASE_SECT_CYC                       : 4  ; // 3  : 0
    __IO uint32_t ERASE_CHIP_CYC                       : 8  ; // 11 : 4
    __IO uint32_t RESERVED0                            : 20 ; // 31 : 12
} EFLASH_ERASE_TIME_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t LATENCY_ES_PROG_CYC                  : 5  ; // 4  : 0
    __IO uint32_t RESERVED1                            : 3  ; // 7  : 5
    __IO uint32_t LATENCY_ECHIP_CYC                    : 6  ; // 13 : 8
    __IO uint32_t RESERVED0                            : 18 ; // 31 : 14
} EFLASH_LATENCY_TIME_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t SECTOR_INDEX                         : 7  ; // 6  : 0
    __IO uint32_t RESERVED1                            : 17 ; // 23 : 7
    __IO uint32_t NVR_SECTOR_SEL                       : 1  ; // 24 : 24
    __IO uint32_t RESERVED0                            : 7  ; // 31 : 25
} EFLASH_ERASE_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t SECTOR_ERASE_TRIG                    : 1  ; // 0  : 0
    __IO uint32_t RESERVED1                            : 15 ; // 15 : 1
    __IO uint32_t CHIP_ERASE_TRIG                      : 1  ; // 16 : 16
    __IO uint32_t RESERVED0                            : 15 ; // 31 : 17
} EFLASH_ERASE_TRIG_FIELD_T;

typedef struct
{
    __IO uint32_t DEEP_STANDBY                         : 1  ; // 0  : 0
    __IO uint32_t RESERVED1                            : 3  ; // 3  : 1
    __IO uint32_t WR_BUF_CLR                           : 1  ; // 4  : 4
    __IO uint32_t RESERVED0                            : 27 ; // 31 : 5
} EFLASH_MISC_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t PROG_ADDR_CHK_EN0                    : 1  ; // 0  : 0
    __IO uint32_t PROG_ADDR_CHK_EN1                    : 1  ; // 1  : 1
    __IO uint32_t PROG_RD_CHK_EN                       : 1  ; // 2  : 2
    __IO uint32_t RESERVED1                            : 1  ; // 3  : 3
    __IO uint32_t RDONLY_EN                            : 1  ; // 4  : 4
    __IO uint32_t CFG_WR_EN                            : 1  ; // 5  : 5
    __IO uint32_t RESERVED0                            : 26 ; // 31 : 6
} EFLASH_OP_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t PROG_ADDR_PROT_START0                : 7  ; // 6  : 0
    __IO uint32_t RESERVED1                            : 1  ; // 7  : 7
    __IO uint32_t PROG_ADDR_PROT_END0                  : 7  ; // 14 : 8
    __IO uint32_t RESERVED0                            : 17 ; // 31 : 15
} EFLASH_PROG_ADDR_PROT0_FIELD_T;

typedef struct
{
    __IO uint32_t PROG_ADDR_PROT_START1                : 7  ; // 6  : 0
    __IO uint32_t RESERVED1                            : 1  ; // 7  : 7
    __IO uint32_t PROG_ADDR_PROT_END1                  : 7  ; // 14 : 8
    __IO uint32_t RESERVED0                            : 17 ; // 31 : 15
} EFLASH_PROG_ADDR_PROT1_FIELD_T;

typedef struct
{
    __IO uint32_t NVM_PROG_ADDR_CHK_ERR_INT_CLR        : 1  ; // 0  : 0
    __IO uint32_t NVR_ACCESS_ERR_INT_CLR               : 1  ; // 1  : 1
    __IO uint32_t PROG_RD_CHK_ERR_INT_CLR              : 1  ; // 2  : 2
    __IO uint32_t PROG_DONE_INT_CLR                    : 1  ; // 3  : 3
    __IO uint32_t ERASE_DONE_INT_CLR                   : 1  ; // 4  : 4
    __IO uint32_t ECC_1BIT_ERR_INT_CLR                 : 1  ; // 5  : 5
    __IO uint32_t ECC_2BIT_ERR_INT_CLR                 : 1  ; // 6  : 6
    __IO uint32_t NVM_NVR_WR_ERR_INT_CLR               : 1  ; // 7  : 7
    __IO uint32_t NVM_NVR_WR_SIZE_ERR_INT_CLR          : 1  ; // 8  : 8
    __IO uint32_t RESERVED0                            : 23 ; // 31 : 9
} EFLASH_ICR_FIELD_T;

typedef struct
{
    __IO uint32_t NVM_PROG_ADDR_CHK_ERR_INT_MSK        : 1  ; // 0  : 0
    __IO uint32_t NVR_ACCESS_ERR_INT_MSK               : 1  ; // 1  : 1
    __IO uint32_t PROG_RD_CHK_ERR_INT_MSK              : 1  ; // 2  : 2
    __IO uint32_t PROG_DONE_INT_MSK                    : 1  ; // 3  : 3
    __IO uint32_t ERASE_DONE_INT_MSK                   : 1  ; // 4  : 4
    __IO uint32_t ECC_1BIT_ERR_INT_MSK                 : 1  ; // 5  : 5
    __IO uint32_t ECC_2BIT_ERR_INT_MSK                 : 1  ; // 6  : 6
    __IO uint32_t NVM_NVR_WR_ERR_INT_MSK               : 1  ; // 7  : 7
    __IO uint32_t NVM_NVR_WR_SIZE_ERR_INT_MSK          : 1  ; // 8  : 8
    __IO uint32_t RESERVED0                            : 23 ; // 31 : 9
} EFLASH_IMR_FIELD_T;

typedef struct
{
    __IO uint32_t NVM_PROG_ADDR_CHK_ERR_INT_STATUS     : 1  ; // 0  : 0
    __IO uint32_t NVR_ACCESS_ERR_INT_STATUS            : 1  ; // 1  : 1
    __IO uint32_t PROG_RD_CHK_ERR_INT_STATUS           : 1  ; // 2  : 2
    __IO uint32_t PROG_DONE_INT_STATUS                 : 1  ; // 3  : 3
    __IO uint32_t ERASE_DONE_INT_STATUS                : 1  ; // 4  : 4
    __IO uint32_t ECC_1BIT_ERR_INT_STATUS              : 1  ; // 5  : 5
    __IO uint32_t ECC_2BIT_ERR_INT_STATUS              : 1  ; // 6  : 6
    __IO uint32_t NVM_NVR_WR_ERR_INT_STATUS            : 1  ; // 7  : 7
    __IO uint32_t NVM_NVR_WR_SIZE_ERR_INT_STATUS       : 1  ; // 8  : 8
    __IO uint32_t RESERVED0                            : 23 ; // 31 : 9
} EFLASH_ISR_FIELD_T;

typedef struct
{
    __IO uint32_t NVM_PROG_ADDR_CHK_ERR_INT_RAW_STATUS : 1  ; // 0  : 0
    __IO uint32_t NVR_ACCESS_ERR_INT_RAW_STATUS        : 1  ; // 1  : 1
    __IO uint32_t PROG_RD_CHK_ERR_INT_RAW_STATUS       : 1  ; // 2  : 2
    __IO uint32_t PROG_DONE_INT_RAW_STATUS             : 1  ; // 3  : 3
    __IO uint32_t ERASE_DONE_INT_RAW_STATUS            : 1  ; // 4  : 4
    __IO uint32_t ECC_1BIT_ERR_INT_RAW_STATUS          : 1  ; // 5  : 5
    __IO uint32_t ECC_2BIT_ERR_INT_RAW_STATUS          : 1  ; // 6  : 6
    __IO uint32_t NVM_NVR_WR_ERR_INT_RAW_STATUS        : 1  ; // 7  : 7
    __IO uint32_t NVM_NVR_WR_SIZE_ERR_INT_RAW_STATUS   : 1  ; // 8  : 8
    __IO uint32_t RESERVED0                            : 23 ; // 31 : 9
} EFLASH_IRSR_FIELD_T;

typedef struct
{
    __IO uint32_t ERR_ADDR                             : 14 ; // 13 : 0
    __IO uint32_t RESERVED0                            : 18 ; // 31 : 14
} EFLASH_ERR_ADDR_FIELD_T;

typedef struct
{
    __IO uint32_t BUF_WADDR                            : 15 ; // 14 : 0
    __IO uint32_t RESERVED1                            : 1  ; // 15 : 15
    __IO uint32_t WR_CNT_VAL                           : 2  ; // 17 : 16
    __IO uint32_t RESERVED0                            : 14 ; // 31 : 18
} EFLASH_WR_BUF_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t TRIM_TAG_RD_ERR_STATUS               : 1  ; // 0  : 0
    __IO uint32_t ERASE_BUSY_STATUS                    : 1  ; // 1  : 1
    __IO uint32_t PROG_BUSY_STATUS                     : 1  ; // 2  : 2
    __IO uint32_t SWD_PAT_ECC_2BIT_ERR_STATUS          : 1  ; // 3  : 3
    __IO uint32_t TM_PAT_ECC_2BIT_ERR_STATUS           : 1  ; // 4  : 4
    __IO uint32_t RESERVED0                            : 27 ; // 31 : 5
} EFLASH_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t TERS                                 : 1  ; // 0  : 0
    __IO uint32_t RCELL                                : 1  ; // 1  : 1
    __IO uint32_t RESERVED0                            : 30 ; // 31 : 2
} EFLASH_CELL_INIT_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t WR_LOCK                              : 1  ; // 0  : 0
    __IO uint32_t RESERVED0                            : 31 ; // 31 : 1
} EFLASH_WR_LOCK_FIELD_T;

typedef struct
{
    __IO uint32_t NVR_PROT                             : 1  ; // 0  : 0
    __IO uint32_t RESERVED0                            : 31 ; // 31 : 1
} EFLASH_NVR_PROT_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t REVISION_ID                      ; // 0x0000
        __IO  EFLASH_REVISION_ID_FIELD_T REVISION_ID_F  ;
    };

    union
    {
        __IO  uint32_t RD_TIME_CFG                      ; // 0x0004
        __IO  EFLASH_RD_TIME_CFG_FIELD_T RD_TIME_CFG_F  ;
    };

    union
    {
        __IO  uint32_t RCV_TIME_CFG                     ; // 0x0008
        __IO  EFLASH_RCV_TIME_CFG_FIELD_T RCV_TIME_CFG_F;
    };

    union
    {
        __IO  uint32_t NVS_TIME_CFG                     ; // 0x000C
        __IO  EFLASH_NVS_TIME_CFG_FIELD_T NVS_TIME_CFG_F;
    };

    union
    {
        __IO  uint32_t PROG_TIME_CFG                    ; // 0x0010
        __IO  EFLASH_PROG_TIME_CFG_FIELD_T PROG_TIME_CFG_F;
    };

    union
    {
        __IO  uint32_t ERASE_TIME_CFG                   ; // 0x0014
        __IO  EFLASH_ERASE_TIME_CFG_FIELD_T ERASE_TIME_CFG_F;
    };

    union
    {
        __IO  uint32_t LATENCY_TIME_CFG                 ; // 0x0018
        __IO  EFLASH_LATENCY_TIME_CFG_FIELD_T LATENCY_TIME_CFG_F;
    };

    union
    {
        __IO  uint32_t ERASE_CFG                        ; // 0x001C
        __IO  EFLASH_ERASE_CFG_FIELD_T ERASE_CFG_F      ;
    };

    union
    {
        __IO  uint32_t ERASE_TRIG                       ; // 0x0020
        __IO  EFLASH_ERASE_TRIG_FIELD_T ERASE_TRIG_F    ;
    };

    union
    {
        __IO  uint32_t MISC_CTRL                        ; // 0x0024
        __IO  EFLASH_MISC_CTRL_FIELD_T MISC_CTRL_F      ;
    };

    union
    {
        __IO  uint32_t OP_CTRL                          ; // 0x0028
        __IO  EFLASH_OP_CTRL_FIELD_T OP_CTRL_F          ;
    };

    union
    {
        __IO  uint32_t PROG_ADDR_PROT0                  ; // 0x002C
        __IO  EFLASH_PROG_ADDR_PROT0_FIELD_T PROG_ADDR_PROT0_F;
    };

    union
    {
        __IO  uint32_t PROG_ADDR_PROT1                  ; // 0x0030
        __IO  EFLASH_PROG_ADDR_PROT1_FIELD_T PROG_ADDR_PROT1_F;
    };

    union
    {
        __IO  uint32_t ICR                              ; // 0x0034
        __IO  EFLASH_ICR_FIELD_T ICR_F                  ;
    };

    union
    {
        __IO  uint32_t IMR                              ; // 0x0038
        __IO  EFLASH_IMR_FIELD_T IMR_F                  ;
    };

    union
    {
        __IO  uint32_t ISR                              ; // 0x003C
        __IO  EFLASH_ISR_FIELD_T ISR_F                  ;
    };

    union
    {
        __IO  uint32_t IRSR                             ; // 0x0040
        __IO  EFLASH_IRSR_FIELD_T IRSR_F                ;
    };

    union
    {
        __IO  uint32_t ERR_ADDR                         ; // 0x0044
        __IO  EFLASH_ERR_ADDR_FIELD_T ERR_ADDR_F        ;
    };

    union
    {
        __IO  uint32_t WR_BUF_STATUS                    ; // 0x0048
        __IO  EFLASH_WR_BUF_STATUS_FIELD_T WR_BUF_STATUS_F;
    };

    union
    {
        __IO  uint32_t STATUS                           ; // 0x004C
        __IO  EFLASH_STATUS_FIELD_T STATUS_F            ;
    };

    union
    {
        __IO  uint32_t CELL_INIT_CTRL                   ; // 0x0050
        __IO  EFLASH_CELL_INIT_CTRL_FIELD_T CELL_INIT_CTRL_F;
    };

    uint32_t RESERVED0[41]                              ;

    union
    {
        __IO  uint32_t WR_LOCK                          ; // 0x00F8
        __IO  EFLASH_WR_LOCK_FIELD_T WR_LOCK_F          ;
    };

    union
    {
        __IO  uint32_t NVR_PROT                         ; // 0x00FC
        __IO  EFLASH_NVR_PROT_FIELD_T NVR_PROT_F        ;
    };

} EFLASH_REG_TypeDef;

#endif /* __EFLASH_DEF_H__ */
