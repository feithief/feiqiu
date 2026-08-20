#include "BswM.h"

#include "Dcm.h"
#include "Dem.h"
#include "Lin.h"
#include "NvM.h"

void BswM_MainFunction(void)
{
    Lin_MainFunction();
    /* ld_task() inside Lin_MainFunction() already schedules the restored
     * IHR transport layer through ld_tp_task(). */
    Dcm_MainFunction();
    NvM_MainFunction();
    Dem_MainFunction();
}
