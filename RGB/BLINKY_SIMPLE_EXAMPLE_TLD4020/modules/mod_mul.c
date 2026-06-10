/***************************************************************************//**
 * @file		mod_mul.c
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

#include "mod_mul.h"
#include "io_e52139b_IAR.h"

uint32_t GulResultMpy;
uint32_t GulResultMac;
uint16_t GusOp1 = 0x1234;
uint16_t GusOp2 = 0x5678;
uint16_t GusOp3 = 0x1234;
uint16_t GusOp4 = 0x2345;


typedef union {
    struct {
        uint16_t usLo16;
        uint16_t usHi16;
    } sData16;
    uint32_t ulData32;
} uResult32;



uint32_t mulTest(void)
{
    uResult32 uResult;
    
    H430_MUL_MPY = GusOp1;
    H430_MUL_OP2 = GusOp2;
    
    uResult.sData16.usLo16 = H430_MUL_RESLO;
    uResult.sData16.usHi16 = H430_MUL_RESHI;
    
    return uResult.ulData32;
}



uint32_t macTest(void)
{
    uResult32 uResult;
    
    H430_MUL_MPY = GusOp1;
    H430_MUL_OP2 = GusOp2;
    H430_MUL_MAC = GusOp3;
    H430_MUL_OP2 = GusOp4;
    
    uResult.sData16.usLo16 = H430_MUL_RESLO;
    uResult.sData16.usHi16 = H430_MUL_RESHI;
    
    return uResult.ulData32;
}
