#include <string.h>
#include "HostRatioKDebug.h"

volatile SHostRatioKDebug g_hostRatioKDebug;

void HostRatioKDebug_Reset(void)
{
    memset((void *)&g_hostRatioKDebug, 0, sizeof(g_hostRatioKDebug));
}