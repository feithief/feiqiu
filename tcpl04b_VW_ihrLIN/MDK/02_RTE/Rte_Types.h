#ifndef RTE_TYPES_H
#define RTE_TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t Rte_LightWorkModeType;

#define RTE_LIGHT_WORK_MODE_NORMAL         ((Rte_LightWorkModeType)0U)
#define RTE_LIGHT_WORK_MODE_CRASH          ((Rte_LightWorkModeType)1U)
#define RTE_LIGHT_WORK_MODE_SHOWROOM       ((Rte_LightWorkModeType)2U)
#define RTE_LIGHT_WORK_MODE_ACTUATOR_TEST  ((Rte_LightWorkModeType)3U)
#define RTE_LIGHT_WORK_MODE_COUNT          ((Rte_LightWorkModeType)4U)

/* Stable RGB application interface. LDF signal names stop at Rte_Com.c. */
typedef struct
{
    uint16_t slave_address;
    bool group_addressing;
    bool brightness_normalized;
    bool dimming_time_relative;
    bool valid;
    bool dimming_time_base_250ms;
    Rte_LightWorkModeType work_mode;
    uint8_t dimming_time;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t brightness;
} Rte_LightCommandType;

typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint8_t nad;
    uint8_t brightness_factor;
    uint16_t application_address;
    uint16_t group_address_mask;
    uint16_t redx;
    uint16_t redy;
    uint32_t redY;
    uint16_t greenx;
    uint16_t greeny;
    uint32_t greenY;
    uint16_t bluex;
    uint16_t bluey;
    uint32_t blueY;
    uint8_t Utemp;
    uint8_t Vtemp;
    uint8_t x1, y1l, y1h;
    uint8_t x2, y2l, y2h;
    uint8_t x3, y3l, y3h;
    uint8_t x4, y4l, y4h;
    uint8_t x5, y5l, y5h;
    uint8_t x6, y6l, y6h;
    uint8_t x7, y7l, y7h;
    uint8_t x8, y8l, y8h;
    uint8_t x9, y9l, y9h;
    uint16_t led_supplier;
    uint16_t platform;
    uint16_t intensity_factor;
    uint8_t part_number[11];
    uint8_t hardware_version[3];
    uint8_t serial_number[20];
    uint16_t calibration_voltage[3];
    uint16_t calibration_temperature;
    /* The production IHR stack persists these independently from the
     * signal-frame address in nad. */
    uint8_t ihr_current_nad;
    uint8_t ihr_auto_nad;
} Rte_SwscConfigType;

#endif
