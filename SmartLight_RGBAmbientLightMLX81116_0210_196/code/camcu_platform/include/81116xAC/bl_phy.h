/**
 * @file
 * @brief The common bootloader's interface.
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git 6d04ab96
 *
 * Melexis N.V. is supplying this code for use with Melexis N.V. processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.  MELEXIS N.V. SHALL NOT IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @endinternal
 *
 * @ingroup Bootloaders
 *
 * @details
 * The PHY-layer wrapper used to manage the Melibus, fast-protocol LIN and the SPI physical interfaces. All functions from the interfaces should be provided externally in case if they will be used.
 */

#ifndef BL_PHY_H
#define BL_PHY_H

#include <stdint.h>

/* The outside info should can have some set from the following defines:
   BL_PHY_USE_MELIBUS
   BL_PHY_USE_PPM
   BL_PHY_USE_SPI
 */

/** PHY layer state type */
typedef enum {
    phyDisabled=0u, /**< PHY is disabled */
    phyEnabled,     /**< PHY is enabled */
} phyState_t;

/** PHY layer error type */
typedef enum {
    phy_NoError=0,      /** PHY no error */
    phy_HeaderError,    /** PHY error in data header */
    phy_StopBitError,   /** PHY error in Stop-Bit */
    phy_ParityError,    /** PHY error in parity bit */
    phy_CrcError,       /** PHY error in Checksum */
    phy_TimeoutError,   /** PHY timeout error */
    phy_HwError,        /** PHY Hardware error */
    /* ... */
    phy_OtherError   = 0xFF    /** PHY unspecified error */
} phyError_t;

/*
   void phy_Init(void);
   void phy_SetState(fpResult_t enabled);
   void phy_SetResponse(fpError_t );
 */
void bl_DataReceived(uint16_t *data, uint16_t size);
void bl_Error(phyError_t error);

void phy_Init(void);
void phy_SetState(phyState_t enabled);

void phy_BL_Tick(void);

#ifndef BL_PHY_USE_MELIBUS
extern void fpError(phyError_t Error);
#endif

#endif /* BL_PHY_H */
