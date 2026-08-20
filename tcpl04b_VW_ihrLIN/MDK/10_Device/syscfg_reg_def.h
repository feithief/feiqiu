/**
 *****************************************************************************
 * @brief   syscfg_reg_def header file.
 *
 * @file    syscfg_reg_def.h
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

#ifndef __SYSCFG_DEF_H__
#define __SYSCFG_DEF_H__

// Register Offset Macro Definition
#define SYSCFG_REVISION_OFFSET                                  0x0000
#define SYSCFG_REMAP_OFFSET                                     0x0004
#define SYSCFG_MISC_CTRL_OFFSET                                 0x0008
#define SYSCFG_SWD_STATUS_OFFSET                                0x000C
#define SYSCFG_PATCH_BIT_OFFSET                                 0x00F8
#define SYSCFG_SYSCFG_LOCK_OFFSET                               0x00FC

// Register Address Macro Definition
#define SYSCFG_REVISION_ADDR                                    ( SYSCFG_BASE_ADDR + SYSCFG_REVISION_OFFSET)
#define SYSCFG_REMAP_ADDR                                       ( SYSCFG_BASE_ADDR + SYSCFG_REMAP_OFFSET)
#define SYSCFG_MISC_CTRL_ADDR                                   ( SYSCFG_BASE_ADDR + SYSCFG_MISC_CTRL_OFFSET)
#define SYSCFG_SWD_STATUS_ADDR                                  ( SYSCFG_BASE_ADDR + SYSCFG_SWD_STATUS_OFFSET)
#define SYSCFG_PATCH_BIT_ADDR                                   ( SYSCFG_BASE_ADDR + SYSCFG_PATCH_BIT_OFFSET)
#define SYSCFG_SYSCFG_LOCK_ADDR                                 ( SYSCFG_BASE_ADDR + SYSCFG_SYSCFG_LOCK_OFFSET)

// Register Field Macro Definition
#define SYSCFG_REVISION_REV_ID_SHIFT                                  16
#define SYSCFG_REVISION_REV_ID_MASK                                   0x007F0000
#define SYSCFG_REVISION_REV_ID_SET(n)                                 (((uint32_t)(n) << 16 ) & 0x007F0000)
#define SYSCFG_REVISION_REV_ID_CLR                                    0xFF80FFFF

#define SYSCFG_REVISION_CHIP_ID_SHIFT                                 0
#define SYSCFG_REVISION_CHIP_ID_MASK                                  0x0000FFFF
#define SYSCFG_REVISION_CHIP_ID_SET(n)                                (((uint32_t)(n) << 0  ) & 0x0000FFFF)
#define SYSCFG_REVISION_CHIP_ID_CLR                                   0xFFFF0000

#define SYSCFG_REMAP_CM0_VECT_BASE_ADDR_SHIFT                         4
#define SYSCFG_REMAP_CM0_VECT_BASE_ADDR_MASK                          0x0FFFFFF0
#define SYSCFG_REMAP_CM0_VECT_BASE_ADDR_SET(n)                        (((uint32_t)(n) << 4  ) & 0x0FFFFFF0)
#define SYSCFG_REMAP_CM0_VECT_BASE_ADDR_CLR                           0xF000000F

#define SYSCFG_REMAP_CM0_REMAP_EN_SHIFT                               0
#define SYSCFG_REMAP_CM0_REMAP_EN_MASK                                0x00000001
#define SYSCFG_REMAP_CM0_REMAP_EN_SET(n)                              (((uint32_t)(n) << 0  ) & 0x00000001)
#define SYSCFG_REMAP_CM0_REMAP_EN_CLR                                 0xFFFFFFFE

#define SYSCFG_MISC_CTRL_OTP_DIS_PWM_EN_SHIFT                         1
#define SYSCFG_MISC_CTRL_OTP_DIS_PWM_EN_MASK                          0x00000002
#define SYSCFG_MISC_CTRL_OTP_DIS_PWM_EN_SET(n)                        (((uint32_t)(n) << 1  ) & 0x00000002)
#define SYSCFG_MISC_CTRL_OTP_DIS_PWM_EN_CLR                           0xFFFFFFFD

#define SYSCFG_MISC_CTRL_VS_LVD_DIS_PWM_EN_SHIFT                      0
#define SYSCFG_MISC_CTRL_VS_LVD_DIS_PWM_EN_MASK                       0x00000001
#define SYSCFG_MISC_CTRL_VS_LVD_DIS_PWM_EN_SET(n)                     (((uint32_t)(n) << 0  ) & 0x00000001)
#define SYSCFG_MISC_CTRL_VS_LVD_DIS_PWM_EN_CLR                        0xFFFFFFFE

#define SYSCFG_SWD_STATUS_NVM_DIS_SWD_ALL_SHIFT                       1
#define SYSCFG_SWD_STATUS_NVM_DIS_SWD_ALL_MASK                        0x00000002
#define SYSCFG_SWD_STATUS_NVM_DIS_SWD_ALL_SET(n)                      (((uint32_t)(n) << 1  ) & 0x00000002)
#define SYSCFG_SWD_STATUS_NVM_DIS_SWD_ALL_CLR                         0xFFFFFFFD

#define SYSCFG_SWD_STATUS_NVM_DIS_SWD_SHIFT                           0
#define SYSCFG_SWD_STATUS_NVM_DIS_SWD_MASK                            0x00000001
#define SYSCFG_SWD_STATUS_NVM_DIS_SWD_SET(n)                          (((uint32_t)(n) << 0  ) & 0x00000001)
#define SYSCFG_SWD_STATUS_NVM_DIS_SWD_CLR                             0xFFFFFFFE

#define SYSCFG_PATCH_BIT_PATCH_BIT_SHIFT                              0
#define SYSCFG_PATCH_BIT_PATCH_BIT_MASK                               0x0000000F
#define SYSCFG_PATCH_BIT_PATCH_BIT_SET(n)                             (((uint32_t)(n) << 0  ) & 0x0000000F)
#define SYSCFG_PATCH_BIT_PATCH_BIT_CLR                                0xFFFFFFF0

#define SYSCFG_SYSCFG_LOCK_SYSCFG_LOCK_SHIFT                          0
#define SYSCFG_SYSCFG_LOCK_SYSCFG_LOCK_MASK                           0x00000001
#define SYSCFG_SYSCFG_LOCK_SYSCFG_LOCK_SET(n)                         (((uint32_t)(n) << 0  ) & 0x00000001)
#define SYSCFG_SYSCFG_LOCK_SYSCFG_LOCK_CLR                            0xFFFFFFFE

// Register Structure Definition
typedef struct
{
    __IO uint32_t CHIP_ID            : 16 ; // 15 : 0
    __IO uint32_t REV_ID             : 7  ; // 22 : 16
    __IO uint32_t RESERVED0          : 9  ; // 31 : 23
} SYSCFG_REVISION_FIELD_T;

typedef struct
{
    __IO uint32_t CM0_REMAP_EN       : 1  ; // 0  : 0
    __IO uint32_t RESERVED1          : 3  ; // 3  : 1
    __IO uint32_t CM0_VECT_BASE_ADDR : 24 ; // 27 : 4
    __IO uint32_t RESERVED0          : 4  ; // 31 : 28
} SYSCFG_REMAP_FIELD_T;

typedef struct
{
    __IO uint32_t VS_LVD_DIS_PWM_EN  : 1  ; // 0  : 0
    __IO uint32_t OTP_DIS_PWM_EN     : 1  ; // 1  : 1
    __IO uint32_t RESERVED0          : 30 ; // 31 : 2
} SYSCFG_MISC_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t NVM_DIS_SWD        : 1  ; // 0  : 0
    __IO uint32_t NVM_DIS_SWD_ALL    : 1  ; // 1  : 1
    __IO uint32_t RESERVED0          : 30 ; // 31 : 2
} SYSCFG_SWD_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t PATCH_BIT          : 4  ; // 3  : 0
    __IO uint32_t RESERVED0          : 28 ; // 31 : 4
} SYSCFG_PATCH_BIT_FIELD_T;

typedef struct
{
    __IO uint32_t SYSCFG_LOCK        : 1  ; // 0  : 0
    __IO uint32_t RESERVED0          : 31 ; // 31 : 1
} SYSCFG_SYSCFG_LOCK_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t REVISION                        ; // 0x0000
        __IO  SYSCFG_REVISION_FIELD_T REVISION_F       ;
    };

    union
    {
        __IO  uint32_t REMAP                           ; // 0x0004
        __IO  SYSCFG_REMAP_FIELD_T REMAP_F             ;
    };

    union
    {
        __IO  uint32_t MISC_CTRL                       ; // 0x0008
        __IO  SYSCFG_MISC_CTRL_FIELD_T MISC_CTRL_F     ;
    };

    union
    {
        __IO  uint32_t SWD_STATUS                      ; // 0x000C
        __IO  SYSCFG_SWD_STATUS_FIELD_T SWD_STATUS_F   ;
    };

    uint32_t RESERVED0[58]                             ;

    union
    {
        __IO  uint32_t PATCH_BIT                       ; // 0x00F8
        __IO  SYSCFG_PATCH_BIT_FIELD_T PATCH_BIT_F     ;
    };

    union
    {
        __IO  uint32_t SYSCFG_LOCK                     ; // 0x00FC
        __IO  SYSCFG_SYSCFG_LOCK_FIELD_T SYSCFG_LOCK_F ;
    };

} SYSCFG_REG_TypeDef;

#endif /* __SYSCFG_DEF_H__ */
