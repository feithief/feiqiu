#include "genLinConfig.h"

/* Integration callback required by the unchanged IHR lin_main_init().
 * EcuM loads the configured NAD before Lin_Init() starts the IHR kernel. */
l_u8 l_callback_Get_Initial_NAD(void)
{
    return SlaveNodeNAD;
}
