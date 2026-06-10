/***************************************************************************//**
 * @file		mod_nvm.c
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

#include "mod_nvm.h"
#include "LinDrvImp_Config.h"

static uint8_t GbTestEn = 0;
static uint8_t GucNvmWriteData[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};


uint8_t nvmTest(void)
{
    if ( 0 == GbTestEn ) {
        return 1;
    }
    GbTestEn = 0;
    
    if ( EEPROM_INTERFACE_MODULE_API_VERSION != EEImp_IfFuns->InterfaceVersion ) {
        return 2;
    }
    
    if ( TRUE != EEImp_IfFuns->MemoryErase( EE_PASSWORD, (eeprom_pAddress_t)EE_START, sizeof(GucNvmWriteData), 0x00, 0x00 ) ) {//erase == write 0
        return 3;
    }
    
    if ( TRUE != EEImp_IfFuns->MemoryWrite( EE_PASSWORD, &GucNvmWriteData[0], (eeprom_pAddress_t)EE_START, sizeof(GucNvmWriteData), 0x00, 0x00 ) ) {
        return 4;
    }
    
    return 0;
}

