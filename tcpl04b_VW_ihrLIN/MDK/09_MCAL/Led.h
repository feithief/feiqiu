#ifndef TCPL04B_LED_H
#define TCPL04B_LED_H

#include <stdbool.h>
#include <stdint.h>

#define LED_MCAL_RED       (0U)
#define LED_MCAL_GREEN     (1U)
#define LED_MCAL_BLUE      (2U)

void Led_Init(void);
void Led_SetDuty(const uint16_t Duty[3]);
void Led_Enable(bool Enable);

#endif
