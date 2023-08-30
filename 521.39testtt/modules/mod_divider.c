/***************************************************************************//**
 * @file		mod_divider.c
 *
 * @creator		wca
 * @created		2022.06.23
 *
 * @brief  		Top application layer with main() entry point.
 *
 * @purpose             Hardware and LIN driver intialization, 
 *                      top application layer.
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the 
 * integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 * $Id: $
 *
 * $Revision:  $
 *
 ******************************************************************************/

#include "mod_divider.h"
#include "io_e52139b_IAR.h"


typedef union {
    struct {
        uint16_t usLo16;
        uint16_t usHi16;
    } sData16;
    uint32_t ulData32;
} uResult32;



uResult32 GuDivident = {
    .ulData32 = 0x12345678
};
uint16_t  GusDivisor = 0x123;
uResult32 GuResult;
uint16_t  GusRemainder;



uint16_t dividerTest(void)
{
    DIVIDER_OP1LO = GuDivident.sData16.usLo16;
    DIVIDER_OP1HI = GuDivident.sData16.usHi16;
    DIVIDER_OP2   = GusDivisor;
    
    GuResult.sData16.usLo16 = DIVIDER_RESULTLO;
    GuResult.sData16.usHi16 = DIVIDER_RESULTHI;
    GusRemainder            = DIVIDER_REMAINDER;
    
    return GuResult.ulData32;
}
