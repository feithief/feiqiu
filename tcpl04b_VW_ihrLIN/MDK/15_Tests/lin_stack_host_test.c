#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "Com.h"
#include "Dcm.h"
#include "Dcm_Cfg.h"
#include "Lin.h"
#include "LinTp.h"
#include "Com_Cfg.h"
#include "MemIf.h"
#include "NvM_Cfg.h"
#include "IoHwAb_Led.h"
#include "Swc_Diagnostic.h"

static Lin_DriverNotificationType s_notification;
static uint8_t s_pid;
static uint8_t s_rxBytes[9];
static uint8_t s_rxOffset;
static uint8_t s_lastTx[8];
static uint8_t s_lastTxLength;
static uint8_t s_calibrationControl;
static Rte_SwscConfigType s_flashConfig =
{
    .magic = 0x53575343UL,
    .version = 4U,
    .nad = 1U,
    .application_address = 8U,
    .group_address_mask = 1U,
    .intensity_factor = 0x8000U,
    .serial_number =
        {'1','2','3','4','5','6','7','8','9','0',
         '0','9','8','7','6','5','4','3','2','1'},
    .ihr_current_nad = 1U,
    .ihr_auto_nad = 1U
};

void MemIf_Init(void) {}

Std_ReturnType MemIf_Read(NvM_BlockIdType BlockId, void *DataPtr,
                          uint16_t Length)
{
    assert(BlockId == NVM_BLOCK_SWSC_CONFIG);
    assert(DataPtr != NULL);
    assert(Length <= sizeof(s_flashConfig));
    memcpy(DataPtr, &s_flashConfig, Length);
    return E_OK;
}

Std_ReturnType MemIf_Write(NvM_BlockIdType BlockId, const void *DataPtr,
                           uint16_t Length)
{
    assert(BlockId == NVM_BLOCK_SWSC_CONFIG);
    assert(DataPtr != NULL);
    assert(Length == sizeof(s_flashConfig));
    memcpy(&s_flashConfig, DataPtr, Length);
    return E_OK;
}

int16_t IoHwAb_Led_GetVoltage(ELedChannel Channel)
{
    return (int16_t)(100U + (uint8_t)Channel);
}

bool IoHwAb_Led_SetCalibrationControl(uint8_t Control)
{
    if (Control > 4U) { return false; }
    s_calibrationControl = Control;
    return true;
}

int16_t IoHwAb_Temperature_GetMcu(void) { return 250; }

Std_ReturnType Lin_Driver_Init(uint8_t Channel, uint32_t Baudrate,
                              Lin_DriverNotificationType Notification)
{
    assert(Channel == 0U);
    assert(Baudrate == 19584UL);
    s_notification = Notification;
    return E_OK;
}

void Lin_Driver_DeInit(uint8_t Channel) { (void)Channel; }
uint8_t Lin_Driver_ReadPid(uint8_t Channel) { (void)Channel; return s_pid; }
uint8_t Lin_Driver_ReadByte(uint8_t Channel)
{
    (void)Channel;
    return s_rxBytes[s_rxOffset++];
}
Std_ReturnType Lin_Driver_StartRx(uint8_t Channel, uint8_t ProtectedId,
                                 Lin_FrameCsModelType Checksum, uint8_t Length)
{
    (void)Channel; (void)ProtectedId; (void)Checksum;
    assert(Length == 8U);
    s_rxOffset = 0U;
    return E_OK;
}
Std_ReturnType Lin_Driver_StartTx(uint8_t Channel, uint8_t ProtectedId,
                                 Lin_FrameCsModelType Checksum,
                                 const uint8_t *Data, uint8_t Length)
{
    (void)Channel; (void)ProtectedId; (void)Checksum;
    assert(Length == 8U);
    memcpy(s_lastTx, Data, Length);
    s_lastTxLength = Length;
    return E_OK;
}
void Lin_Driver_Abort(uint8_t Channel) { (void)Channel; }
void Lin_Driver_CheckAutoBaud(uint8_t Channel) { (void)Channel; }
void Lin_Driver_EnterCritical(void) {}
void Lin_Driver_ExitCritical(void) {}

static uint8_t MakePid(uint8_t frameId)
{
    uint8_t p0 = (uint8_t)(((frameId >> 0U) ^ (frameId >> 1U) ^
                            (frameId >> 2U) ^ (frameId >> 4U)) & 1U);
    uint8_t p1 = (uint8_t)(~((frameId >> 1U) ^ (frameId >> 3U) ^
                              (frameId >> 4U) ^ (frameId >> 5U)) & 1U);
    return (uint8_t)(frameId | (uint8_t)(p0 << 6U) |
                     (uint8_t)(p1 << 7U));
}

static uint8_t Checksum(uint8_t pid, const uint8_t data[8], int classic)
{
    uint16_t sum = classic ? 0U : pid;
    uint8_t index;
    for (index = 0U; index < 8U; index++)
    {
        sum = (uint16_t)(sum + data[index]);
        if (sum > 0xFFU) { sum = (uint16_t)(sum - 0xFFU); }
    }
    return (uint8_t)(~sum);
}

static void Header(uint8_t frameId)
{
    s_pid = MakePid(frameId);
    s_notification(0U, LIN_DRV_EVENT_BREAK);
    s_notification(0U, LIN_DRV_EVENT_SYNC);
    s_notification(0U, LIN_DRV_EVENT_PID);
}

static void Receive(uint8_t frameId, const uint8_t data[8], int classic)
{
    uint8_t index;
    memcpy(s_rxBytes, data, 8U);
    s_rxBytes[8] = Checksum(MakePid(frameId), data, classic);
    Header(frameId);
    for (index = 0U; index < 9U; index++)
    {
        s_notification(0U, LIN_DRV_EVENT_RX_BYTE);
    }
}

static void Service(void)
{
    Dcm_MainFunction();
    NvM_MainFunction();
    Swc_Diagnostic_MainFunction();
}

static void ConfirmTx(void)
{
    s_notification(0U, LIN_DRV_EVENT_TX_DONE);
}

static void TestSignalPath(void)
{
    const uint8_t command[8] = {0x34U, 0x12U, 10U, 20U, 30U,
                                50U, 0x03U, 0x02U};
    uint16_t address;
    uint8_t value;
    Receive(0x15U, command, 0);
    assert(Com_IsSignalGroupUpdated(COM_SIGNAL_GROUP_LIGHT_COMMAND));
    assert(Com_ReceiveSignalGroup(COM_SIGNAL_GROUP_LIGHT_COMMAND) == E_OK);
    assert(Com_ReceiveSignal(COM_SIG_RX_SLAVE_ADDRESS, &address) == E_OK);
    assert(address == 0x1234U);
    assert(Com_ReceiveSignal(COM_SIG_RX_GREEN, &value) == E_OK);
    assert(value == 20U);

    value = 2U;
    assert(Com_SendSignal(COM_SIG_TX_RED_SHORT, &value) == E_OK);
    s_lastTxLength = 0U;
    Header(0x01U);
    assert(s_lastTxLength == 8U);
    assert((s_lastTx[1] & 0x03U) == 2U);
    ConfirmTx();
}

static void TestAssignNadAndReadDid(void)
{
    const uint8_t assignNad[8] = {1U, 6U, 0xB0U, 0U, 0U, 0U, 0U, 0xA0U};
    const uint8_t readDid0006[8] = {0xA0U, 3U, 0x22U, 0U, 6U,
                                    0xFFU, 0xFFU, 0xFFU};
    uint8_t changedNad = 0U;

    Receive(0x3CU, assignNad, 1);
    Service();
    assert(Dcm_TakeDiagnosticAddressChange(&changedNad));
    assert(changedNad == 0xA0U);
    s_lastTxLength = 0U;
    Header(0x3DU);
    assert(s_lastTxLength == 8U);
    assert(s_lastTx[0] == 1U);
    assert(s_lastTx[1] == 1U);
    assert(s_lastTx[2] == 0xF0U);
    ConfirmTx();

    Receive(0x3CU, readDid0006, 1);
    Service();
    s_lastTxLength = 0U;
    Header(0x3DU);
    assert(s_lastTxLength == 8U);
    assert(s_lastTx[0] == 0xA0U);
    assert(s_lastTx[1] == 5U);
    assert(s_lastTx[2] == 0x62U);
    assert(s_lastTx[3] == 0x00U);
    assert(s_lastTx[4] == 0x06U);
    assert(s_lastTx[5] == 0x00U);
    assert(s_lastTx[6] == 0x80U);
    assert(s_lastTx[7] == 0xFFU);
    ConfirmTx();
}

static void TestUnscheduledOldResponseIsCancelled(void)
{
    const uint8_t assignNad[8] = {1U, 6U, 0xB0U, 0U, 0U, 0U, 0U, 0xA0U};
    const uint8_t readDid0002[8] = {0xA0U, 3U, 0x22U, 0U, 2U,
                                    0xFFU, 0xFFU, 0xFFU};
    uint8_t changedNad = 0U;

    Dcm_SetActiveDiagnosticAddress(1U);
    Receive(0x3CU, assignNad, 1);
    Service();
    assert(Dcm_TakeDiagnosticAddressChange(&changedNad));
    assert(changedNad == 0xA0U);

    /* Reproduce the CANoe trace: no SRF after B0; the next MRF arrives first. */
    Receive(0x3CU, readDid0002, 1);
    Service();
    s_lastTxLength = 0U;
    Header(0x3DU);
    assert(s_lastTxLength == 8U);
    assert(s_lastTx[0] == 0xA0U);
    assert(s_lastTx[1] == 5U);
    assert(s_lastTx[2] == 0x62U);
    assert(s_lastTx[3] == 0x00U);
    assert(s_lastTx[4] == 0x02U);
    ConfirmTx();
}

static void TestForeignNadDoesNotCancelResponse(void)
{
    const uint8_t readDid0002[8] = {0xA0U, 3U, 0x22U, 0U, 2U,
                                    0xFFU, 0xFFU, 0xFFU};
    const uint8_t foreignRequest[8] = {3U, 3U, 0x22U, 0U, 2U,
                                       0xFFU, 0xFFU, 0xFFU};

    Receive(0x3CU, readDid0002, 1);
    Service();
    Receive(0x3CU, foreignRequest, 1);
    Service();
    s_lastTxLength = 0U;
    Header(0x3DU);
    assert(s_lastTxLength == 8U);
    assert(s_lastTx[0] == 0xA0U);
    assert(s_lastTx[2] == 0x62U);
    assert(s_lastTx[4] == 0x02U);
    ConfirmTx();
}

static void TestWriteDidService(void)
{
    const uint8_t writeDid0006[8] = {0xA0U, 5U, 0x2EU, 0U, 6U,
                                     0x34U, 0x12U, 0xFFU};
    const uint8_t readDid0006[8] = {0xA0U, 3U, 0x22U, 0U, 6U,
                                    0xFFU, 0xFFU, 0xFFU};

    Receive(0x3CU, writeDid0006, 1);
    Service();
    Header(0x3DU);
    assert(s_lastTx[1] == 3U);
    assert(s_lastTx[2] == 0x6EU);
    assert(s_lastTx[3] == 0x00U);
    assert(s_lastTx[4] == 0x06U);
    ConfirmTx();

    Receive(0x3CU, readDid0006, 1);
    Service();
    Header(0x3DU);
    assert(s_lastTx[5] == 0x34U);
    assert(s_lastTx[6] == 0x12U);
    ConfirmTx();
}

static void TestSegmentedReadDid(void)
{
    const uint8_t readSerial[8] = {0xA0U, 3U, 0x22U, 0x6AU, 0x69U,
                                   0xFFU, 0xFFU, 0xFFU};
    uint8_t response[23];
    uint16_t offset = 0U;
    uint8_t expectedSequence = 1U;

    Receive(0x3CU, readSerial, 1);
    Service();

    Header(0x3DU);
    assert((s_lastTx[1] & 0xF0U) == 0x10U);
    assert(s_lastTx[2] == 23U);
    memcpy(&response[offset], &s_lastTx[3], 5U);
    offset += 5U;
    ConfirmTx();

    while (offset < sizeof(response))
    {
        uint16_t remaining = (uint16_t)(sizeof(response) - offset);
        uint16_t amount = (remaining < 6U) ? remaining : 6U;
        Header(0x3DU);
        assert((s_lastTx[1] & 0xF0U) == 0x20U);
        assert((s_lastTx[1] & 0x0FU) == expectedSequence);
        memcpy(&response[offset], &s_lastTx[2], amount);
        offset = (uint16_t)(offset + amount);
        expectedSequence = (uint8_t)((expectedSequence + 1U) & 0x0FU);
        ConfirmTx();
    }

    assert(response[0] == 0x62U);
    assert(response[1] == 0x6AU);
    assert(response[2] == 0x69U);
    assert(memcmp(&response[3], "12345678900987654321", 20U) == 0);
}

static void TestDidSideEffects(void)
{
    const uint8_t ledControl[8] = {0xA0U, 4U, 0x2EU, 0U, 0x0AU,
                                   1U, 0xFFU, 0xFFU};
    const uint8_t writeAddress[8] = {0xA0U, 5U, 0x2EU, 0U, 3U,
                                     8U, 0U, 0xFFU};

    Receive(0x3CU, ledControl, 1);
    Service();
    Header(0x3DU);
    assert(s_lastTx[2] == 0x6EU);
    ConfirmTx();
    assert(s_calibrationControl == 1U);

    Receive(0x3CU, writeAddress, 1);
    Service();
    Header(0x3DU);
    assert(s_lastTx[0] == 0xA0U);
    assert(s_lastTx[2] == 0x6EU);
    ConfirmTx();
    assert(s_flashConfig.application_address == 8U);
    assert(s_flashConfig.nad == 8U);
    assert(Dcm_GetActiveDiagnosticAddress() == 8U);
}

int main(void)
{
    Com_Init();
    NvM_Init();
    Swc_Diagnostic_Init();
    Dcm_Init(&Dcm_Config, 1U);
    LinTp_Init();
    Lin_Init(&Lin_Config);
    assert(s_notification != NULL);
    TestSignalPath();
    TestAssignNadAndReadDid();
    TestUnscheduledOldResponseIsCancelled();
    TestForeignNadDoesNotCancelResponse();
    TestWriteDidService();
    TestSegmentedReadDid();
    TestDidSideEffects();
    puts("LIN AUTOSAR diagnostic host tests passed");
    return 0;
}
