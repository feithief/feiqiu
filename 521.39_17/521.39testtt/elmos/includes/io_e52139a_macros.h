/* ============================================================================== */
/*
   Macro declarations used for peripheral I/O
   declarations for ICC430/A430 IAR C/C++ Compiler
*/
/* ============================================================================== */

#ifndef __IO_MACROS_H
#define __IO_MACROS_H

/* ============================================================================== */

#ifdef __IAR_SYSTEMS_ICC__

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#pragma language=save
#pragma language=extended

#define __REG8  unsigned char
#define __REG16 unsigned short
#define __REG32 unsigned long

/* ============================================================================== */
/* IO register attributes */

#define __READ_WRITE
#ifdef __cplusplus
#define __READ          /* Not supported */
#else
#define __READ          const
#endif
#define __WRITE         /* Not supported */

/* ============================================================================== */

#pragma language=restore

#include "intrinsics.h"

#pragma language=save
#pragma language=extended

/* ============================================================================== */
/* standard bits */

typedef struct
{
  unsigned char no0:1;
  unsigned char no1:1;
  unsigned char no2:1;
  unsigned char no3:1;
  unsigned char no4:1;
  unsigned char no5:1;
  unsigned char no6:1;
  unsigned char no7:1;
} __BITS8;

typedef struct
{
  unsigned short no0:1;
  unsigned short no1:1;
  unsigned short no2:1;
  unsigned short no3:1;
  unsigned short no4:1;
  unsigned short no5:1;
  unsigned short no6:1;
  unsigned short no7:1;
  unsigned short no8:1;
  unsigned short no9:1;
  unsigned short no10:1;
  unsigned short no11:1;
  unsigned short no12:1;
  unsigned short no13:1;
  unsigned short no14:1;
  unsigned short no15:1;
} __BITS16;

/* ============================================================================== */
/* IO register handling defines */
/* PRQA S 0289++ #justification: @ is an IAR operator to locate a parameter to a specific address. */
#define __IO_REG8(NAME, ADDRESS, ATTRIBUTE) \
          volatile __no_init ATTRIBUTE unsigned char NAME @ ADDRESS

#define __IO_REG16(NAME, ADDRESS, ATTRIBUTE) \
          volatile __no_init ATTRIBUTE unsigned short NAME @ ADDRESS

#define __IO_REG32(NAME, ADDRESS, ATTRIBUTE) \
          volatile __no_init ATTRIBUTE unsigned long NAME @ ADDRESS

#define __IO_REG8_BIT(NAME, ADDRESS, ATTRIBUTE, BIT_STRUCT) \
          volatile __no_init ATTRIBUTE union \
          { \
            unsigned char NAME; \
            BIT_STRUCT NAME ## _bit; \
          } @ ADDRESS

#define __IO_REG16_BIT(NAME, ADDRESS, ATTRIBUTE,BIT_STRUCT) \
          volatile __no_init ATTRIBUTE union \
          { \
            unsigned short NAME; \
            BIT_STRUCT NAME ## _bit; \
          } @ ADDRESS

#define __IO_REG32_BIT(NAME, ADDRESS, ATTRIBUTE, BIT_STRUCT) \
          volatile __no_init ATTRIBUTE union \
          { \
            unsigned long NAME; \
            BIT_STRUCT NAME ## _bit; \
          } @ ADDRESS
/* PRQA S 0289-- */
/* ============================================================================== */

#pragma language=restore

#endif /* __IAR_SYSTEMS_ICC__ */

#endif /* __IO_MACROS_H */

