/**
 *****************************************************************************
 * @brief   trim_reg_def header file.
 *
 * @file    trim_reg_def.h
 * @author  AE/FAE team
 * @date    2025.08.01
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

#ifndef __TRIM_REG_DEF_H__
#define __TRIM_REG_DEF_H__

typedef struct
{
    __IO uint32_t DIE_VER                   : 8;
    __IO uint32_t TRIM_VER                  : 6;
    __IO uint32_t BIN_CODE                  : 3;
    __IO uint32_t RESERVED0                 : 15;
} TRIM_VALUE_BASIC_INFO_FIELD_T;

typedef struct
{
    __IO uint32_t LRC32K_TRIM_STATUS               : 1;
    __IO uint32_t HRC28M_TRIM_STATUS               : 1;
    __IO uint32_t TPREF_TRIM_STATUS                : 1;
    __IO uint32_t BIAS_TRIM_STATUS                 : 1;
    __IO uint32_t LDO15_TRIM_STATUS                : 1;
    __IO uint32_t LDO33_TRIM_STATUS                : 1;
    __IO uint32_t LED_LDO5_TRIM_STATUS             : 1;
    __IO uint32_t LED_TRIM_STATUS                  : 1;
    __IO uint32_t LED_IB_DIAG_TRIM_STATUS          : 1;
    __IO uint32_t OTP_TRIM_STATUS                  : 1;
    __IO uint32_t LIN_TRIM_STATUS                  : 1;
    __IO uint32_t RESERVED0                        : 1;
    __IO uint32_t ADC_TRIM_0_16_STATUS             : 1;
    __IO uint32_t ADC_GAIN_OS_STATUS               : 1;
    __IO uint32_t ADC_VS_DIV_TRIM_STATUS           : 1;
    __IO uint32_t ADC_LED0_TRIM_STATUS             : 1;
    __IO uint32_t ADC_LED1_TRIM_STATUS             : 1;
    __IO uint32_t ADC_LED2_TRIM_STATUS             : 1;
    __IO uint32_t ADC_DIFF_VDD5_LED0_TRIM_STATUS   : 1;
    __IO uint32_t ADC_DIFF_VDD5_LED1_TRIM_STATUS   : 1;
    __IO uint32_t ADC_DIFF_VDD5_LED2_TRIM_STATUS   : 1;
    __IO uint32_t ADC_TSENSOR0_TRIM_STATUS         : 1;
    __IO uint32_t ADC_TSENSOR1_TRIM_STATUS         : 1;
    __IO uint32_t ADC_LINAA_TRIM_STATUS            : 1;
    __IO uint32_t ADC_IO2_IO4_TRIM_STATUS          : 1;
    __IO uint32_t ADC_DIFF_IO2_IO4_GND_TRIM_STATUS : 1;
    __IO uint32_t ADC_TPREF_3POINT_CALIBRATION     : 1;
    __IO uint32_t ADC_TSENSOR_CALIBRATION          : 1;
    __IO uint32_t VAON_3POINT_CALIBRATION          : 1;
    __IO uint32_t RESERVED1                        : 3;
} TRIM_VALUE_TRIM_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t LRC32K_CAP_TRIM           : 2;
    __IO uint32_t RESERVED0                 : 2;
    __IO uint32_t LRC32K_RES_TRIM           : 4;
    __IO uint32_t RESERVED1                 : 24;
} TRIM_VALUE_LRC32K_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t HRC48M_TP_TRIM            : 6;
    __IO uint32_t RESERVED0                 : 2;
    __IO uint32_t HRC48M_I_TRIM             : 10;
    __IO uint32_t RESERVED1                 : 2;
    __IO uint32_t HRC48M_LDO_TRIM           : 2;
    __IO uint32_t RESERVED2                 : 10;
} TRIM_VALUE_HRC48M_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t TPREF_TRIM                : 7;
    __IO uint32_t RESERVED0                 : 25;
} TRIM_VALUE_TPREF_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t BIAD_TRIM                 : 3;
    __IO uint32_t RESERVED0                 : 29;
} TRIM_VALUE_BIAS_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LDO15_TRIM                : 3;
    __IO uint32_t RESERVED0                 : 1;
    __IO uint32_t LDO15_PORB_SEL            : 2;
    __IO uint32_t RESERVED1                 : 26;
} TRIM_VALUE_LDO15_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LDO33_TRIM                : 3;
    __IO uint32_t RESERVED0                 : 1;
    __IO uint32_t LDO33_PORB_SEL            : 2;
    __IO uint32_t RESERVED1                 : 26;
} TRIM_VALUE_LDO33_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LED_LDO5_OCP_TRIM         : 2;
    __IO uint32_t RESERVED0                 : 2;
    __IO uint32_t LED_LDO5_OUT_TRIM         : 4;
    __IO uint32_t LED_RES_TC_TRIM           : 5;
    __IO uint32_t RESERVED1                 : 3;
    __IO uint32_t LED_LDO5_OCP_EN           : 1;
    __IO uint32_t RESERVED2                 : 15;
} TRIM_VALUE_LED_LDO5_TRIM_FIELD_T;


typedef struct
{
    __IO uint32_t LED_IBIAS_TRIM            : 8;
    __IO uint32_t LED_LC0_IOUT_TRIM         : 8;
    __IO uint32_t LED_LC1_IOUT_TRIM         : 8;
    __IO uint32_t LED_LC2_IOUT_TRIM         : 8;
} TRIM_VALUE_LED_TRIM_FIELD_T;


typedef struct
{
    __IO uint32_t LED_IB_DIAG0_TRIM         : 8;
    __IO uint32_t LED_IB_DIAG1_TRIM         : 8;
    __IO uint32_t LED_IB_DIAG2_TRIM         : 8;
    __IO uint32_t RESERVED0                 : 8;
} TRIM_VALUE_LED_IB_DIAG_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t OTP_TRIM                  : 5;
    __IO uint32_t RESERVED0                 : 27;
} TRIM_VALUE_OTP_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LIN_TRIM_BUSLIM           : 4;
    __IO uint32_t LINAA_ITRIM               : 4;
    __IO uint32_t RESERVED0                 : 24;
} TRIM_VALUE_LIN_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL0          : 6;
    __IO uint32_t RESERVED0                 : 2;
    __IO uint32_t SARADC_TRIM_VAL1          : 6;
    __IO uint32_t RESERVED1                 : 2;
    __IO uint32_t SARADC_TRIM_VAL2          : 7;
    __IO uint32_t RESERVED2                 : 1;
    __IO uint32_t SARADC_TRIM_VAL3          : 8;
} TRIM_VALUE_ADC_TRIM0_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL4          : 8;
    __IO uint32_t RESERVED0                 : 8;
    __IO uint32_t SARADC_TRIM_VAL5          : 9;
    __IO uint32_t RESERVED1                 : 7;
} TRIM_VALUE_ADC_TRIM1_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL6          : 9;
    __IO uint32_t RESERVED0                 : 7;
    __IO uint32_t SARADC_TRIM_VAL7          : 10;
    __IO uint32_t RESERVED1                 : 6;
} TRIM_VALUE_ADC_TRIM2_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL8          : 11;
    __IO uint32_t RESERVED0                 : 5;
    __IO uint32_t SARADC_TRIM_VAL9          : 12;
    __IO uint32_t RESERVED1                 : 4;
} TRIM_VALUE_ADC_TRIM3_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL10         : 12;
    __IO uint32_t RESERVED0                 : 4;
    __IO uint32_t SARADC_TRIM_VAL11         : 13;
    __IO uint32_t RESERVED1                 : 3;
} TRIM_VALUE_ADC_TRIM4_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL12         : 14;
    __IO uint32_t RESERVED0                 : 2;
    __IO uint32_t SARADC_TRIM_VAL13         : 13;
    __IO uint32_t RESERVED1                 : 3;
} TRIM_VALUE_ADC_TRIM5_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL14         : 14;
    __IO uint32_t RESERVED0                 : 2;
    __IO uint32_t SARADC_TRIM_VAL15         : 15;
    __IO uint32_t RESERVED1                 : 1;
} TRIM_VALUE_ADC_TRIM6_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL14         : 16;
    __IO uint32_t RESERVED0                 : 16;
} TRIM_VALUE_ADC_TRIM7_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_GAIN               : 14;
    __IO uint32_t RESERVED0                 : 2;
    __IO uint32_t SARADC_OFFSET             : 14;
    __IO uint32_t RESERVED1                 : 2;
} TRIM_VALUE_ADC_GAIN_OS_FIELD_T;

typedef struct
{
    __IO uint32_t B                         : 14;
    __IO uint32_t RESERVED0                 : 2;
    __IO uint32_t A                         : 14;
    __IO uint32_t RESERVED1                 : 2;
} TRIM_VALUE_ADC_COEF_FIELD_T;

typedef struct
{
    __IO uint32_t TOS                       : 16;
    __IO uint32_t K                         : 16;
} TRIM_VALUE_ADC_TSENSOR_FIELD_T;

typedef struct
{
    __IO uint32_t LINAA_RS                  : 12;
    __IO uint32_t RESERVED0                 : 20;
} TRIM_VALUE_ADC_LINAA_FIELD_T;

typedef struct
{
    __IO uint32_t TPREF                     : 20;
    __IO uint32_t TA                        : 12;
} TRIM_VALUE_TPREF_TA_FIELD_T;

typedef struct
{
    __IO uint32_t CODE                      : 16;
    __IO uint32_t TA                        : 16;
} TRIM_VALUE_ADC_TSENSOR_CODE_TA_FIELD_T;

typedef struct
{
    __IO uint32_t VAON                      : 16;
    __IO uint32_t TCODE                     : 16;
} TRIM_VALUE_VAON_TSENSOR_FIELD_T;

typedef struct
{
    __IO uint32_t COEF                      : 32;
} TRIM_VALUE_VAON_COEF_FIELD_T;

#define TRIM_REG_UNION(REG_NAME, FIELD_TYPE)           \
    union {                                            \
        __IO uint32_t REG_NAME;                        \
        __IO FIELD_TYPE REG_NAME##_F;                  \
    };

typedef struct
{
    TRIM_REG_UNION(BASIC_INFO, TRIM_VALUE_BASIC_INFO_FIELD_T)                           // 0x0000
    TRIM_REG_UNION(TRIM_STATUS, TRIM_VALUE_TRIM_STATUS_FIELD_T)                         // 0x0004
    TRIM_REG_UNION(LRC32K_TRIM, TRIM_VALUE_LRC32K_TRIM_FIELD_T)                         // 0x0008
    TRIM_REG_UNION(HRC48M_TRIM, TRIM_VALUE_HRC48M_TRIM_FIELD_T)                         // 0x000C
    TRIM_REG_UNION(TPREF_TRIM, TRIM_VALUE_TPREF_TRIM_FIELD_T)                           // 0x0010
    TRIM_REG_UNION(BIAS_TRIM, TRIM_VALUE_BIAS_TRIM_FIELD_T)                             // 0x0014
    TRIM_REG_UNION(LDO15_TRIM, TRIM_VALUE_LDO15_TRIM_FIELD_T)                           // 0x0018
    TRIM_REG_UNION(LDO33_TRIM, TRIM_VALUE_LDO33_TRIM_FIELD_T)                           // 0x001C
    TRIM_REG_UNION(LED_LDO5_TRIM, TRIM_VALUE_LED_LDO5_TRIM_FIELD_T)                     // 0x0020
    TRIM_REG_UNION(LED_TRIM, TRIM_VALUE_LED_TRIM_FIELD_T)                               // 0x0024
    TRIM_REG_UNION(LED_IB_DIAG_TRIM, TRIM_VALUE_LED_IB_DIAG_TRIM_FIELD_T)               // 0x0028
    TRIM_REG_UNION(OTP_TRIM, TRIM_VALUE_OTP_TRIM_FIELD_T)                               // 0x002C
    TRIM_REG_UNION(LIN_TRIM, TRIM_VALUE_LIN_TRIM_FIELD_T)                               // 0x0030
    uint32_t RESERVED0;
    TRIM_REG_UNION(ADC_TRIM0, TRIM_VALUE_ADC_TRIM0_FIELD_T)                             // 0x0038
    TRIM_REG_UNION(ADC_TRIM1, TRIM_VALUE_ADC_TRIM1_FIELD_T)                             // 0x003C
    TRIM_REG_UNION(ADC_TRIM2, TRIM_VALUE_ADC_TRIM2_FIELD_T)                             // 0x0040
    TRIM_REG_UNION(ADC_TRIM3, TRIM_VALUE_ADC_TRIM3_FIELD_T)                             // 0x0044
    TRIM_REG_UNION(ADC_TRIM4, TRIM_VALUE_ADC_TRIM4_FIELD_T)                             // 0x0048
    TRIM_REG_UNION(ADC_TRIM5, TRIM_VALUE_ADC_TRIM5_FIELD_T)                             // 0x004C
    TRIM_REG_UNION(ADC_TRIM6, TRIM_VALUE_ADC_TRIM6_FIELD_T)                             // 0x0050
    TRIM_REG_UNION(ADC_TRIM7, TRIM_VALUE_ADC_TRIM7_FIELD_T)                             // 0x0054
    TRIM_REG_UNION(ADC_GAIN_OS, TRIM_VALUE_ADC_GAIN_OS_FIELD_T)                         // 0x0058
    TRIM_REG_UNION(ADC_VS_DIV, TRIM_VALUE_ADC_COEF_FIELD_T)                             // 0x005C
    TRIM_REG_UNION(ADC_LED0, TRIM_VALUE_ADC_COEF_FIELD_T)                               // 0x0060
    TRIM_REG_UNION(ADC_LED1, TRIM_VALUE_ADC_COEF_FIELD_T)                               // 0x0064
    TRIM_REG_UNION(ADC_LED2, TRIM_VALUE_ADC_COEF_FIELD_T)                               // 0x0068
    TRIM_REG_UNION(ADC_DIFF_VDD5_LED0, TRIM_VALUE_ADC_COEF_FIELD_T)                     // 0x006C
    TRIM_REG_UNION(ADC_DIFF_VDD5_LED1, TRIM_VALUE_ADC_COEF_FIELD_T)                     // 0x0070
    TRIM_REG_UNION(ADC_DIFF_VDD5_LED2, TRIM_VALUE_ADC_COEF_FIELD_T)                     // 0x0074
    TRIM_REG_UNION(ADC_TSENSOR0, TRIM_VALUE_ADC_TSENSOR_FIELD_T)                        // 0x0078
    TRIM_REG_UNION(ADC_TSENSOR1, TRIM_VALUE_ADC_TSENSOR_FIELD_T)                        // 0x007C
    TRIM_REG_UNION(ADC_LINAA, TRIM_VALUE_ADC_LINAA_FIELD_T)                             // 0x0080
    TRIM_REG_UNION(ADC_IO2_IO4, TRIM_VALUE_ADC_COEF_FIELD_T)                            // 0x0084
    TRIM_REG_UNION(ADC_DIFF_IO4_GND, TRIM_VALUE_ADC_COEF_FIELD_T)                       // 0x0088
    TRIM_REG_UNION(ADC_DIFF_IO2_GND, TRIM_VALUE_ADC_COEF_FIELD_T)                       // 0x008C
    TRIM_REG_UNION(TPREF_TA_LOW, TRIM_VALUE_TPREF_TA_FIELD_T)                           // 0x0090
    TRIM_REG_UNION(TPREF_TA_NORMAL, TRIM_VALUE_TPREF_TA_FIELD_T)                        // 0x0094
    TRIM_REG_UNION(TPREF_TA_HIGH, TRIM_VALUE_TPREF_TA_FIELD_T)                          // 0x0098
    TRIM_REG_UNION(ADC_TSENSOR0_CODE_TA_NORMAL, TRIM_VALUE_ADC_TSENSOR_CODE_TA_FIELD_T) // 0x009C
    TRIM_REG_UNION(ADC_TSENSOR1_CODE_TA_NORMAL, TRIM_VALUE_ADC_TSENSOR_CODE_TA_FIELD_T) // 0x00A0
    TRIM_REG_UNION(ADC_TSENSOR0_CODE_TA_HIGH, TRIM_VALUE_ADC_TSENSOR_CODE_TA_FIELD_T)   // 0x00A4
    TRIM_REG_UNION(ADC_TSENSOR1_CODE_TA_HIGH, TRIM_VALUE_ADC_TSENSOR_CODE_TA_FIELD_T)   // 0x00A8
    TRIM_REG_UNION(VAON_TROOM_TSENSOR1, TRIM_VALUE_VAON_TSENSOR_FIELD_T)                // 0x00AC
    TRIM_REG_UNION(VAON_THIGH_TSENSOR1, TRIM_VALUE_VAON_TSENSOR_FIELD_T)                // 0x00B0
    TRIM_REG_UNION(VAON_TLOW_TSENSOR1, TRIM_VALUE_VAON_TSENSOR_FIELD_T)                 // 0x00B4
    TRIM_REG_UNION(VAON_COEF_A, TRIM_VALUE_VAON_COEF_FIELD_T)                           // 0x00B8
    TRIM_REG_UNION(VAON_COEF_B, TRIM_VALUE_VAON_COEF_FIELD_T)                           // 0x00BC
    TRIM_REG_UNION(VAON_COEF_C, TRIM_VALUE_VAON_COEF_FIELD_T)                           // 0x00C0
} TRIM_REG_TypeDef;

#endif /* __TRIM_REG_DEF_H__ */
