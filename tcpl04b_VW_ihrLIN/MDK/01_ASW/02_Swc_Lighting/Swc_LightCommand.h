#ifndef SWC_LIGHT_COMMAND_H
#define SWC_LIGHT_COMMAND_H

#include <stdint.h>

#include "Rte_Types.h"

#define SWC_LIGHT_COMMAND_FRAME_SIZE       (8U)
#define SWC_LIGHT_COMMAND_NAD_MIN          (1U)
#define SWC_LIGHT_COMMAND_NAD_MAX          (15U)
#define SWC_LIGHT_COMMAND_BRIGHTNESS_MAX   (100U)
#define SWC_LIGHT_COMMAND_BRIGHTNESS_INIT  (126U)

typedef enum
{
    SWC_LIGHT_COMMAND_OK = 0,
    SWC_LIGHT_COMMAND_NOT_FOR_NODE,
    SWC_LIGHT_COMMAND_INVALID_ARGUMENT,
    SWC_LIGHT_COMMAND_INVALID_NODE_ADDRESS,
    SWC_LIGHT_COMMAND_INVALID_WORK_MODE,
    SWC_LIGHT_COMMAND_INVALID_BRIGHTNESS,
    SWC_LIGHT_COMMAND_NOT_VALID,
} Swc_LightCommandStatusType;

Swc_LightCommandStatusType Swc_LightCommand_Decode(
    const uint8_t frame[SWC_LIGHT_COMMAND_FRAME_SIZE],
    uint16_t application_address,
    uint16_t group_address_mask,
    Rte_LightCommandType *command);

Swc_LightCommandStatusType Swc_LightCommand_Validate(
    const Rte_LightCommandType *command,
    uint16_t application_address,
    uint16_t group_address_mask);

#endif
