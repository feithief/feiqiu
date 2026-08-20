#ifndef SWSC_SYSTEM_STATUS_H
#define SWSC_SYSTEM_STATUS_H

#include "Algorithm.h"
#include "Rte_Types.h"
#include "Swc_Types.h"

typedef enum
{
    ESystemErrorNone = 0x0,
    ESystemErrorBatError = 0x1,
    ESystemErrorLedError = 0x2
} ESystemError;

typedef enum
{
    ESystemSleepAwake = 0x1,
    ESystemSleepPrepare = 0x2,
    ESystemSleepProcessing = 0x3,
    ESystemSleepSleep = 0x4
} ESystemSleep;

typedef struct
{
    ESystemSleep sleepFlag;
    ESystemError errFlag;
    bool_t newLinSignal;
} SSystemStatus;

extern SSystemStatus sysStatus;
extern Rte_LightCommandType sysLin;

void Swc_Status_Init(void);
void Swc_Status_Update(void);

#endif
