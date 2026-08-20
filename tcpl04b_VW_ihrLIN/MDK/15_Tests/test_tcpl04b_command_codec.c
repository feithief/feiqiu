#include "Swc_LightCommand.h"

#include <assert.h>
#include <stdio.h>

static void test_individual_rgb_command(void)
{
    const uint8_t frame[8] = {0x08,0x00,11,22,33,100,0x4A,0x06};
    Rte_LightCommandType command;

    assert(Swc_LightCommand_Decode(frame, 0x0008U, 0x0001U, &command) ==
           SWC_LIGHT_COMMAND_OK);
    assert(command.slave_address == 8U);
    assert(!command.group_addressing);
    assert(command.red == 11U && command.green == 22U && command.blue == 33U);
    assert(command.brightness == 100U);
    assert(command.dimming_time == 10U);
    assert(command.work_mode == RTE_LIGHT_WORK_MODE_SHOWROOM);
    assert(command.valid && command.dimming_time_base_250ms);
}

static void test_group_addressing(void)
{
    const uint8_t frame[8] = {0x04,0x80,1,2,3,1,0,0x02};
    Rte_LightCommandType command;

    assert(Swc_LightCommand_Decode(frame, 0x0008U, 0x0004U, &command) ==
           SWC_LIGHT_COMMAND_OK);
    assert(command.group_addressing);
    assert(Swc_LightCommand_Decode(frame, 0x0008U, 0x0002U, &command) ==
           SWC_LIGHT_COMMAND_NOT_FOR_NODE);
}

static void test_rejects_invalid_payload(void)
{
    Rte_LightCommandType command;
    uint8_t frame[8] = {0x08,0,1,2,3,100,0,0};

    assert(Swc_LightCommand_Decode(frame, 8U, 1U, &command) ==
           SWC_LIGHT_COMMAND_NOT_VALID);
    frame[7] = 0x02;
    frame[5] = SWC_LIGHT_COMMAND_BRIGHTNESS_INIT;
    assert(Swc_LightCommand_Decode(frame, 8U, 1U, &command) ==
           SWC_LIGHT_COMMAND_INVALID_BRIGHTNESS);
    frame[5] = 10U;
    frame[6] = 0x80U;
    assert(Swc_LightCommand_Decode(frame, 8U, 1U, &command) ==
           SWC_LIGHT_COMMAND_INVALID_WORK_MODE);
    assert(Swc_LightCommand_Decode(frame, 0U, 1U, &command) ==
           SWC_LIGHT_COMMAND_INVALID_NODE_ADDRESS);
    assert(Swc_LightCommand_Decode(0, 8U, 1U, &command) ==
           SWC_LIGHT_COMMAND_INVALID_ARGUMENT);
}

int main(void)
{
    test_individual_rgb_command();
    test_group_addressing();
    test_rejects_invalid_payload();
    puts("MQB RGB command codec tests passed");
    return 0;
}
