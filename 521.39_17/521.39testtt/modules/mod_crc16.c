/***************************************************************************//**
 * @file		mod_crc.c
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

#include "mod_crc16.h"
#include "io_e52139b_IAR.h"
#include "el_types_H430_IAR.h"



uint16_t calc_ccitt_crc_16(const uint16_t *buffer, uint16_t words);
uint16_t GusCrcData[] = {
    0x1234, 0x5678, 0x00FF, 0xFF00, 0xF00F, 0x0FF0, 0x9ABC, 0xDEF0,
};

uint8_t crc16Test(void)
{
    uint16_t usCrcExpect;
    
    usCrcExpect = calc_ccitt_crc_16( &GusCrcData[0], sizeof(GusCrcData)/2 );
    
    CRC16_CONFIG       = 0x03;
    CRC16_LENGTH       = sizeof(GusCrcData);
    CRC16_EXPECTED_CRC = usCrcExpect;
    CRC16_START        = (uint16_t)&GusCrcData[0];
    
    while ( 0 == CRC16_STATUS );
    if ( 1 == CRC16_STATUS ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

uint16_t calc_ccitt_crc_16(const uint16_t *buffer, uint16_t words)
{
    uint16_t poly = 0x8408; // reverse 0x1021
    uint16_t crc  = 0xFFFF;
    
    int n, b;
    uint8_t *byte_buffer = (uint8_t *)buffer;
    int bytes            = words * 2;
    
    for (n = 0; n < bytes; n++) {
        crc ^= (uint16_t)byte_buffer[n];
        for (b = 0; b < 8; b++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ poly;
            } else {
                crc = (crc >> 1);
            }
        }
    }
    return crc;
}


