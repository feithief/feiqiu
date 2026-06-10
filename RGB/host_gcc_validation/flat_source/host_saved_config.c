#include "ModuleFlash.h"
#include "SystemLinComm.h"

SFlashContent savedConfig =
{
    32768u, /* factor */
    22170u, /* eolAddr */
    7025u,  /* redx */
    2967u,  /* redy */
    25080u, /* redY */
    1402u, /* greenx */
    7123u, /* greeny */
    43830u, /* greenY */
    1519u,  /* bluex */
    289u,  /* bluey */
    7792u, /* blueY */
    3330u, /* whitex */
    3480u, /* whitey */
    48820u, /* whiteY */
    100u,   /* brightness_factor */
    80u,    /* Utemp */
    188u,   /* Vtemp */
    {2400u, 2770u, 8858u, 1141u}
};

SHostSysLinStack sysLin_Stack = {0};