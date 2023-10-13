/**
 * @file
 * @brief ADC support library
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
 * The ADC SAR support library
 */

#ifndef LIB_ADC_H
#define LIB_ADC_H

#include <syslib.h>
#include "sys_tools.h"
#include "plib.h"
#include "io.h"
#include "adc_refs.h"

/** ADC reference configuration */
typedef enum {
    ADCREF_PORTS = 0u,                  /**< ADC_TRIMPORT will be connected from ports */
    ADCREF_PINS,                        /**< ADC_TRIMPORT will be connected to TR_ADCREF pins */
} AdcRef_t;

/** Types of Start-Of-Sequence */
typedef enum {
    AdcSOS_SECOND_HARDWARE_TRIGGER = 0u,/**< Second hardware trigger mode */
    AdcSOS_FIRST_HARDWARE_TRIGGER,      /**< First hardware trigger mode */
    AdcSOS_FIRMWARE_TRIGGER,            /**< Firmware trigger mode */
    AdcSOS_PERMANENT_TRIGGER,           /**< Permanent trigger mode */
} AdcSOS_Source_t;

/** Types of Start-Of-Conversion */
typedef enum {
    AdcSOC_HARDWARE = 0u,               /**< Hardware SOC */
    AdcSOC_FIRMWARE,                    /**< Firmware SOC */
    AdcSOC_HARDWARE_FW,                 /**< Hardware + Software SOC */
    AdcSOC_PERMANENT,                   /**< Permanent SOC */
} AdcSOC_Source_t;

/** Types of ADC Interrupts */
typedef enum {
    AdcInt_NOINT = 0u,                  /**< No Interrupts */
    AdcInt_EOC,                         /**< End of conversion Interrupt */
    AdcInt_EOF,                         /**< End of Frame Interrupt */
    AdcInt_EOS,                         /**< End of Sequence Interrupt */
} AdcIntScheme_t;

/** ADC StandBy configuration types */
typedef enum {
    AdcAsb_NOTUSED = 0u,                /**< Auto-StandBy is used when ADC not used. */
    AdcAsb_WAITTRIGGERS,                /**< Auto-StandBy is used when waiting for triggers. */
    AdsAsb_NEVER,                       /**< Never use StandBy */
} AdcAutoStandby_t;

/** ADC Data width types */
typedef enum {
    AdcWdt_16Bit = 0u,                  /**< ADC data width is 16bit */
    AdcWdt_32Bit,                       /**< ADC data width is 32bit */
} AdcWidth_t;

/** ADC Configuration and modes type */
typedef union {
    struct
    __attribute__((packed)) {
        uint16_t START : 1;                 /**< START command bit */
        uint16_t STOP : 1;                  /**< STOP command bit */
        AdcSOS_Source_t SOS_SOURCE : 2;     /**< Start-Of-Sequence source */
        AdcSOC_Source_t SOC_SOURCE : 2;     /**< Start-Of-Conversion source */
        uint16_t NoInterleave : 1;          /**< 1 - when EOC triggers SDATA update, When 0 - EOA triggers SDATA update */
        uint16_t Saturate : 1;              /**< 1 - data saturated to 2^N-1 in case of overflow (for N bit in DATA) and 0 in undeflow; 0 - DATA is garbage in under-/over-flow  */
        AdcIntScheme_t IntScheme : 2;       /**< Interrupts Scheme */
        AdcAutoStandby_t ASB : 2;           /**< Auto StandBy Mode */
        AdcWidth_t AdcWidth : 1;            /**< ADC convertions width */
    } s;
    uint16_t data; /**< Grouped field to write the IO-port at once */
} Adc_Control_t;

/** ADC phase type definition */
typedef enum {
    AdcPhase_IDLE = 0u,                     /**< ADC no operation */
    AdcPhase_MEMTRANSFER,                   /**< ADC is transferring the data */
    AdcPhase_CONVERTION,                    /**< ADC performs the conversion */
    AdcPhase_WAITTRIGGER,                   /**< ADC waits a trigger */
} AdcPhaseState_t;

/** ADC statuses */
typedef union {
    struct
    __attribute__((packed)) {
        uint16_t Paused : 1;                /**< Paused flag */
        uint16_t Resumed : 1;               /**< Resumed flag */
        uint16_t _sw_trig : 1;              /**< Firmware trigger control bit */
        uint16_t Ready : 1;                 /**< Ready flag */
        AdcIntScheme_t LasLIntSource : 2;   /**< The Last Interrupt source */
        AdcPhaseState_t state : 2;          /**< The current ADC phase */
        uint16_t ADC_OVF : 1;               /**< ADC Overflow flag */
        uint16_t ADC_ERR : 1;               /**< ADC Error flag */
        uint16_t MEM_ERR : 1;               /**< ADC Memory error flag */
        uint16_t FRAME_ERR : 1;             /**< ADC Frame error flag */
        uint16_t ABORTED : 1;               /**< ADC aborted flag */
    } s;
    uint16_t data; /**< The grouped 16bit status interface to get statuses in scope */
} AdcState_t;


/** AdcGetDAC returns the current DAC output value [9:0]
 * @return The current DAC value.
 */
STATIC INLINE uint16_t AdcGetDAC(void)
{
    return IO_GET(ADC_SAR, ADC_DATA);
}



/** AdcInit configures the ADC
 * @param[in] AdcDiv the MCU clock divider.
 * @param[in] SBase the initial SBase offset.
 * @param[in] ctrl specifies the Sequence and Confirsion sources, Interleave, Saturate and Interrupt scheme, StandBy mode
 */
STATIC INLINE void AdcInit(uint8_t AdcDiv, void *SBase,
                           Adc_Control_t ctrl)
{
    IO_SET(PORT_ADC_CTRL, ADC_EN, 1u);
    IO_SET(ADC_SAR, STOP, 1u); /* STOP the ADC from any ADC mode. */
    IO_SET(ADC_SAR, ADC_CLK_DIV, AdcDiv);
    IO_SET(ADC_SAR, SBASE_0, (uint16_t )SBase);
    IO_SET(ADC_SAR, START, 0u, /* Don't impact on Start-Stop */
           STOP, 0u, SOS_SOURCE, ctrl.s.SOS_SOURCE, SOC_SOURCE,
           ctrl.s.SOC_SOURCE, NO_INTERLEAVE, ctrl.s.NoInterleave,
           SATURATE, ctrl.s.Saturate, INT_SCHEME, ctrl.s.IntScheme,
           ASB, ctrl.s.ASB);
}

/** AdcStart starts the ADC from any ADC mode. */
STATIC INLINE void AdcStart(void)
{
    IO_HOST(ADC_SAR, START) = (uint16_t)1u << IO_OFFSET(ADC_SAR, START); /* START the ADC. */
}

/** AdcStop Stops the ADC from any ADC mode. */
STATIC INLINE void AdcStop(void)
{
    IO_HOST(ADC_SAR, STOP) = (uint16_t)1u << IO_OFFSET(ADC_SAR, STOP); /* STOP the ADC. */
}

/** AdcPause disables all ADC triggers. */
STATIC INLINE void AdcPause(void)
{
    IO_HOST(ADC_SAR, PAUSE) = (uint16_t)1u << IO_OFFSET(ADC_SAR, PAUSE);
}

/** AdcSwTrigger triggers the sequence/conversion by the SW */
STATIC INLINE void AdcSwTrigger(void)
{
    IO_SET(ADC_SAR, SW_TRIG, 1u);
}

/** AdcStopBlocking Stops ADC and waits until the ADC will stop the sequence
 *
 * @note: Function does not react on AWD
 */
STATIC INLINE void AdcStopBlocking(void)
{
    if (IO_GET(ADC_SAR, STOP)!=1u) { /* If ADC is not stopped, we need to stop it first */
        IO_SET(ADC_SAR, STOP, 1u);
        DELAY(2u);  /* Make sure we will have the ADC HW reaction */
        while (IO_GET(ADC_SAR, ABORTED )==0u) {
            /* Wait for the ADC will be aborted after the STOP command */
        }
    }
}

/** AdcResume Enables the selected ADC triggers. */
STATIC INLINE void AdcResume(void)
{
    IO_HOST(ADC_SAR, RESUME) = (uint16_t)1u << IO_OFFSET(ADC_SAR, RESUME);
}

/** AdcGetState returns the current ADC's state
 * @return the current ADC's state
 */
STATIC INLINE AdcState_t AdcGetState(void)
{
    AdcState_t tmp;
    tmp.data = IO_HOST(ADC_SAR, READY);
    return (AdcState_t)tmp;
}

#endif /* LIB_ADC_H */

