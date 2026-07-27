#ifndef __MOD_PWM_H
#define __MOD_PWM_H

#include "ModuleAdc.h"
#define PWM_ADC_TRIGGERDELAY_SET(ch,value)  (ADC_PWM_TRIGGER_DELAY_SET((ch),(value)))

typedef enum {
  PWM_CH0  = 0u,
  PWM_CH1  = 1u,
  PWM_CH2  = 2u,
  PWM_CH3  = 3u,
} pwm_ch_t;

#define PWM_INVALID_VALUE 0xFFFF

#define PWM_PRESCALER_SET(ch,value)         (*(&PWM_PRESCALER0+(ch)) = (uint16_t)(value))
#define PWM_PERIOD_SET(ch,value)            (*(&PWM_PERIOD0+((ch)<<2L)) = (uint16_t)(value))
#define PWM_START_SET(ch,value)             (*(&PWM_START0+((ch)<<2L)) = (uint16_t)(value))
#define PWM_STOP_SET(ch,value)              (*(&PWM_STOP0+((ch)<<2L)) = (uint16_t)(value))
#define PWM_COUNTER_GET(ch,value)           *(value) = *(&PWM_COUNTER0+(ch))

#define PWM_CH_ENABLE(ch)                   (PWM_CONTROL |= (1L<<(uint16_t)(ch)))
#define PWM_CH_DISABLE(ch)                  (PWM_CONTROL &= ~(1L<<(uint16_t)(ch)))
#define PWM_CH_ENABLE_ALL()                 (PWM_CONTROL |=  0x0F)
#define PWM_CH_DISABLE_ALL()                (PWM_CONTROL &= ~(0x0F))

#define PWM_CH_DRIVER_ENABLE(ch)            (PWM_CONTROL |= (1L<<((uint16_t)(ch)))<<4L)//it should be 4L,not 3L.
#define PWM_CH_DRIVER_DISABLE(ch)           (PWM_CONTROL &= ~(1L<<((uint16_t)(ch)))<<4L)
#define PWM_CH_DRIVER_ENABLE_ALL()          (PWM_CONTROL |= 0xF0)
#define PWM_CH_DRIVER_DISABLE_ALL()         (PWM_CONTROL &= ~(0xF0))


#define PWM_CH_DRIVER_CURRENT_SET(ch,val)   (PWM_DRIVER = (PWM_DRIVER  & ((0xFFFF  & ~(1L<<((3L*((ch)+1))+1))) & ~ (1L<<((3L*((ch)+1))+2)))& ~(1L<<((3L*((ch)+1))+3))) |(((val) << 4L) << (3L*(ch))))
#define PWM_CH_DRIVER_PULLUP_SET(ch)        (PWM_DRIVER |= (1L << (uint16_t)(ch)))
#define PWM_CH_DRIVER_PULLUP_CLEAR(ch)      (PWM_DRIVER &= ~(1L << (uint16_t)(ch)))
#define PWM_CH_DRIVER_PULLUP_SET_ALL()      (PWM_DRIVER |= (uint16_t)0x07)
#define PWM_CH_DRIVER_PULLUP_CLEAR_ALL()    (PWM_DRIVER &= ~((uint16_t)0x07))


#define PWM_POLARITY_SET(ch)                (PWM_POLARITY &= ~(1L<<(uint16_t)(ch)))
#define PWM_POLARITY_SET_INV(ch)            (PWM_POLARITY |= (1L<<(uint16_t)(ch)))


#define PWM_TERM_SET(ch)                    (PWM_POLARITY |= (1L<<((uint16_t)(ch)+4L)))
#define PWM_TERM_CLEAR(ch)                  (PWM_POLARITY &= ~(1L<<((uint16_t)(ch)+4L)))

#define PWM_ZERO_SET(ch)                    (PWM_POLARITY |= (1L<<((uint16_t)(ch)+8L)))
#define PWM_ZERO_CLEAR(ch)                  (PWM_POLARITY &= ~(1L<<((uint16_t)(ch)+8L)))


#define PWM_ADC_TRIGGERDELAY_SET(ch,value)  (ADC_PWM_TRIGGER_DELAY_SET((ch),(value)))

#define PWM_IRQ_CLEAR_ALL()                 (PWM_IRQ_MASK = ((uint16_t)0x00)
#define PWM_IRQ_ENABLE(irq)                 (PWM_IRQ_MASK |= (1L<<(irq)))
#define PWM_IRQ_DISABLE()                   (PWM_IRQ_MASK &= ~(1L<<(irq)))
#define PWM_IRQ_VNO_ENABLE(vno)             (PWM_IRQ_VENABLE = (0x07 & (uint16_t)(vno)))

#define PWM_RUNNING()                       (PWM_STATUS_bit.running)



void pwm_init(void);
void pwm_off(void);


#endif                                                                  /* __MOD_PWM_H                   */
