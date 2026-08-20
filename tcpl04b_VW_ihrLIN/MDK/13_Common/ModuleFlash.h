#ifndef MODULE_FLASH_COMPAT_H
#define MODULE_FLASH_COMPAT_H

#include <stdint.h>

#ifndef btrue
#define btrue                    (1U)
#endif
#ifndef bfalse
#define bfalse                   (0U)
#endif
#define Slave_Address_eol        (0U)
#define Slave_Address_auto       (1U)

typedef enum
{
    EFlashUpdateFree = 0x00,
    EFlashUpdateProcessing = 0x01,
    EFlashAutoAddressStart = 0x02,
    EFlashAutoAddressSaved = 0x04,
    EFlashUpdateLocked = 0x80
} EFlashUpdate;

/* Compatibility view used by the frozen production IHR core.  Persistent
 * storage remains owned by AUTOSAR NvM/Rte_SwscConfigType. */
typedef struct
{
    uint16_t singleAddr;
    uint8_t pART;
    uint16_t AutoAddr;
    uint8_t ProductID[5];
    uint8_t CurrentNAD;
} SFlashContent;

extern volatile EFlashUpdate flashFlag;
extern SFlashContent savedConfig;

void moduleFlashSave(void);

#endif
