/**
 * @file
 * @brief ADC pins and settings definitions
 * @internal
 *
 * @copyright (C) 2017-2018 Melexis N.V.
 * git 6d04ab96
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
 * The ADC Signal definitions. Define a set of signals and refrences available for the MLX81116 chip
 */

#ifndef LIB_ADC_REFS_H
#define LIB_ADC_REFS_H

#include <stdint.h>
#include "static_assert.h"
#include "compiler_abstraction.h"

/** ADC reference definition
 *
 *
 */
typedef enum {
    ADC_VREF_NONE=0u,   /**< VREF is disconnected */
    ADC_VREF_0_75V,     /**< VREF=0.75V */
    ADC_VREF_1_5V,      /**< VREF=1.5V */
    ADC_VREF_2_5V,      /**< VREF=2.5V */
    ADC_VREF_MEAS,      /**< switch VDDA to VREF for ratiometric measurements */
} Adc_Vref_t;

/** ADC reference definition, splitted for differential configuration. Group of defines used for #ADC_VREF_DIFF_HIGH_SIGNAL selection */
typedef enum {
    ADC_VREF_DIFF_VS=0u,
    ADC_VREF_DIFF_HSW0_SW,
    ADC_VREF_DIFF_HSW1_SW,
    ADC_VREF_DIFF_HSW2_SW,
} Adc_VrefDiffHigh_t;

/** ADC channels (signals) definition
 *
 *
 */
typedef enum {
    ADC_SIG_VS_DIV14=0u,    /**< Internal high ohmic divider 1:14  */
    ADC_SIG_TEMP,           /**< Internal temperature sensor */
    ADC_SIG_VDDD,           /**< Digital supply voltage */
    ADC_SIG_VDDA_DIV2,      /**< Analogue supply voltage, divider 1:2 */
    ADC_SIG_VBG_D,          /**< Bandgap voltage, second digital BG */
    ADC_SIG_VAUX_DIV4,      /**< Auxiliary analogue supply, divider 1:4 */
    ADC_SIG_LINAAMP,        /**< LIN autoconfig : amplifier output / SHUNT */
    ADC_SIG_LINVCMO,        /**< LIN autoconfig : common mode output */
    ADC_SIG_AA_IN,          /**< auto addressing digital input, 5V */
    ADC_SIG_AA_OUT,         /**< auto addressing digital output, 5V */
    ADC_SIG_AIN3,           /**< reserved */
    ADC_SIG_HVIODIFF,       /**< differential amp needs only 1 ADC channel, but multiple input selection bits */
} Adc_Signal_t;

/** Differential measurements selection: signals */
typedef enum {
    ADC_DIFF_DIAG=0u,
    ADC_DIFF_LC0,
    ADC_DIFF_LC1,
    ADC_DIFF_LC2,
    ADC_DIFF_LC3,
    ADC_DIFF_LC4,
    ADC_DIFF_LC5,
    ADC_DIFF_LC6,
    ADC_DIFF_LC7,
    ADC_DIFF_LC8,
    ADC_DIFF_HSWLC0,
    ADC_DIFF_HSWLC1,
    ADC_DIFF_HSW0,
    ADC_DIFF_HSW1,
    ADC_DIFF_HSW2,
} Adc_DiffSignal_t;

/** ADC [1:0] Signs definitions  */
typedef enum {
    ADC_NO_SIGN = 0b00,     /**< Used when no signs are needed */
    ADC_NO_SIGN2 = 0b01,    /**< Used when no signs are needed */
    ADC_EOF_SIGN = 0b10,    /**< End-of-Frame sign */
    ADC_EOS_SIGN = 0b11,    /**< End-of-Sequence sign */
} Adc_Eos_t;


/** ADC Trigger source definition */
typedef enum {
    ADC_TRIG_PWM_MA1_CMP = 0u,  /**< PWM compare interrupt */
    ADC_TRIG_PWM_MA1_END,       /**< PWM counter interrupt */
    ADC_TRIG_PWM_SL1_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL2_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL3_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL4_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL5_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL6_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL7_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL8_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL9_CMP,       /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL10_CMP,      /**< PWM compare interrupt */
    ADC_TRIG_PWM_SL11_CMP,      /**< PWM compare interrupt */
} Adc_TrigSel_t;


/** Gain for diff measurement */
typedef uint16_t Adc_SelScGain_t;

/** Reference delay (for tests) */
typedef uint16_t Adc_ScRefDelay_t;

/** Settle Delay (for tests) */
typedef uint16_t Adc_ScSettleDelay_t;

/** ADC SData structure definition */
typedef union {
    struct __attribute__((packed)){
        Adc_Eos_t AdcEosSign : 2;                    /**< End-of-Sequense sign, used by ADC interface to mark end of frame (=10) or end of sequence */
        Adc_Signal_t AdcChan : 5;                    /**< ADC channel number [0-31] */
        Adc_Vref_t AdcRef : 3;                       /**< ADC reference selector */
        Adc_DiffSignal_t AdcSelscSig : 4;            /**< differential measurement,  signal input selection */
        Adc_VrefDiffHigh_t AdcSelscRef : 2;          /**< differential measurement,  reference input selection, can be #Adc_VrefDiffHigh_t */
        uint16_t : 1;                                /**< differential measurement,  reference input selection, High part, not used */
        Adc_TrigSel_t AdcTrig : 4;                   /**< ADC hardware trigger selection */
        uint16_t AdcDSel : 1;                        /**< selection if Adc_D from shell or from SDATA */
        Adc_SelScGain_t AdcSelscGain : 2;            /**< Gain selection for differential measurement, for TEST */
        Adc_ScRefDelay_t AdcScRefDelay : 3;          /**< Delay to wait after reference switching, 1-8us at 4MHz ADC clock, for TEST */
        Adc_ScSettleDelay_t AdcScSettleDelay : 5;    /**< Delay to wait for settling of the SC amplifier after input activation, 1-32us at 4MHz ADC clock, for TEST */
    } s;

    struct {
        uint16_t low_word;
        uint16_t high_word;
    } u16;

    uint32_t u32;

} Adc_SData_t;

ASSERT( sizeof(Adc_SData_t) == 4 );

/** Example for the SDATA array defintion */
#define ADC_FRAME_SAMPLES_EXAMPLE  (0x10)
typedef struct {
    uint16_t *AdcData[ADC_FRAME_SAMPLES_EXAMPLE];   /**< Pointer to the ADC data frame destination buffer. The buffer should be the size of the SData structure */
    Adc_SData_t SData[ADC_FRAME_SAMPLES_EXAMPLE];   /**< SData for the frame samples */
    Adc_SData_t sign; /**< End-of-Sequence or End-of-Frame sign */
} Adc_FrameExample_t;



#endif /* LIB_ADC_REFS_H */
