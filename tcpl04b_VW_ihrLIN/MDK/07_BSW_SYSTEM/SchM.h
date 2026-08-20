#ifndef SCHM_H
#define SCHM_H

#include "Lin_Driver.h"

#define SchM_Enter_LinStack() Lin_Driver_EnterCritical()
#define SchM_Exit_LinStack()  Lin_Driver_ExitCritical()

#endif
