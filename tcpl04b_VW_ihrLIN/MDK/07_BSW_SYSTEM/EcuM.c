#include "EcuM.h"

#include "Dem.h"
#include "genLinConfig.h"
#include "Lin.h"
#include "NvM.h"
#include "NvM_Cfg.h"

void EcuM_Init(void)
{
    const Rte_SwscConfigType *config;
    uint8_t diagnosticNad;

    NvM_Init();
    Dem_Init();
    config = NvM_CfgGetSwscConfig();
    diagnosticNad = config->application_address;
    if ((diagnosticNad < 1U) || (diagnosticNad > 16U))
    {
        diagnosticNad = config->nad;
    }
    /* The restored IHR communication kernel owns LinIf/LinTp initialization.
     * l_sys_init(), called by Lin_Init(), enters the proven lin_main_init()
     * sequence and consumes SlaveNodeNAD before the frame table is selected. */
    SlaveNodeNAD = diagnosticNad;
    Lin_Init(NULL);
}
