/***************************************************************************//**
 * @file        el_helper_H430_IAR.h
 *
 * @creator		sbai
 * @created		17.06.2015
 *
 * @brief  		Provides helper macros
 *
 * @purpose
 *
 * Provides helper functions for Elmos's CPU APIs and development
 * on ELMOS'S intelligent ICs.
 *
 * $Id: $
 *
 * $Revision: $
 *
 ******************************************************************************
 *
 * Demo Code Usage Restrictions\:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer\:
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
 ******************************************************************************/

#ifndef EL_HELPER_H430_IAR_H_
#define EL_HELPER_H430_IAR_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"
#include <intrinsics.h>

#if defined(EL_DEVICE_M52139A)
  #include <io_m52139a.h>
#elif defined(EL_DEVICE_M52139C)
  #include <io_m52139c.h>
#elif defined(EL_DEVICE_E52139A)
  #include <io_e52139a.h>
#elif defined(EL_DEVICE_E52139B)
  #include <io_e52139b.h>
#elif defined(EL_DEVICE_E52398A) || defined(EL_DEVICE_E52306A)
#include <io_e52398a.h>
#elif defined(EL_DEVICE_E52398B) || defined(EL_DEVICE_E52306B)
  #include <io_e52398b.h>
#elif defined(EL_DEVICE_52399A)
  #include <io_m52399b.h>
#else
  #error "No IC target defined! <'EL_DEVICE_XXXXX'>"
#endif

/* ***************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#ifndef NON_ZERO_TO_ONE
  #define NON_ZERO_TO_ONE(value) (((value) == 0) ? 0 : 1)
#endif

#ifndef BM
  #define BM(n) (1 << (n))
#endif

#ifndef BF
  #define BF(x,b,s) (((x) & (b)) >> (s))
#endif

#ifndef MIN
  #define MIN(n,m) (((n) < (m)) ? (n) : (m))
#endif

#ifndef MAX
 #define MAX(n,m) (((n) < (m)) ? (m) : (n))
#endif

#ifndef ABS
  #define ABS(n) (((n) < 0u) ? -(n) : (n))
#endif

/***************************************************************************//**
 * Bidirectional handling of SFR or other direct access
 ******************************************************************************/
#define MEM_IO_I08(addr)          (*((vint8_t   *)(addr)))
#define MEM_IO_I16(addr)          (*((vint16_t  *)(addr)))
#define MEM_IO_I32(addr)          (*((vint32_t  *)(addr)))
#define MEM_IO_I64(addr)          (*((vint64_t  *)(addr)))

#define MEM_IO_U08(addr)          (*((vuint8_t  *)(addr)))
#define MEM_IO_U16(addr)          (*((vuint16_t *)(addr)))
#define MEM_IO_U32(addr)          (*((vuint32_t *)(addr)))
#define MEM_IO_U64(addr)          (*((vuint64_t *)(addr)))

/***************************************************************************//**
 * Use num_types from stdint.h if available
 ******************************************************************************/
#if !defined(_STDINT) && !defined(__STDINT_DECLS) 
/***************************************************************************//**
 * Standard MIN-MAX-definitions are not available from compiler-environment
 ******************************************************************************/
#define INT8_MIN              (int8_t)(-128)
#define INT16_MIN             (int16_t)(-32768)
#define INT32_MIN             (int32_t)(-2147483647L - 1L)

#define INT8_MAX              (uint8_t)127
#define INT16_MAX             (uint16_t)32767
#define INT32_MAX             (uint32_t)2147483647L

#define UINT8_MAX             (uint8_t)0xffU /* 255U */
#define UINT16_MAX            (uint16_t)0xffffU /* 65535U */
#define UINT32_MAX            (uint32_t)0xffffffffUL  /* 4294967295U */

/* Limits of integer types capable of holding object pointers */
#define INTPTR_MIN            INT16_MIN
#define INTPTR_MAX            INT16_MAX
#define UINTPTR_MAX           UINT16_MAX

/* Limits of greatest-width integer types */
#define INTMAX_MIN            INT32_MIN
#define INTMAX_MAX            INT32_MAX
#define UINTMAX_MAX           UINT32_MAX

/* Limits of other integer types */
#define PTRDIFF_MIN           INT16_MIN
#define PTRDIFF_MAX           INT16_MAX

#define SIZE_MAX              UINT16_MAX

#endif

/***************************************************************************//**
 * Bit macros 0 to 15, A to F
 ******************************************************************************/
#ifndef BIT_0

#define BIT_0               (0x01)
#define BIT_1               (0x02)
#define BIT_2               (0x04)
#define BIT_3               (0x08)
#define BIT_4               (0x10)
#define BIT_5               (0x20)
#define BIT_6               (0x40)
#define BIT_7               (0x80)
#define BIT_8               (0x0100u)
#define BIT_9               (0x0200u)
#define BIT_10              (0x0400u)
#define BIT_11              (0x0800u)
#define BIT_12              (0x1000u)
#define BIT_13              (0x2000u)
#define BIT_14              (0x4000u)
#define BIT_15              (0x8000u)

#define BIT_A               (0x0400u)
#define BIT_B               (0x0800u)
#define BIT_C               (0x1000u)
#define BIT_D               (0x2000u)
#define BIT_E               (0x4000u)
#define BIT_F               (0x8000u)

#endif

/***************************************************************************//**
 * Little endian to big endian conversion
 ******************************************************************************/
#define EL_SWAP_ENDIANESS_16BIT(x) ((x>>8) | (x<<8))
#define EL_SWAP_ENDIANESS_32BIT(x) (((x>>24)&0xff) | ((x<<8)&0xff0000) | ((x>>8)&0xff00) | ((x<<24)&0xff000000))

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 * @pre				  (optional) Which are the conditions to call this function? i.e. none
 *
 * @post        (optional) How has the status changed, after the function ran?
 *
 * TODO: A more detailed description.
 *
 * @see        (optional) Crossreference
 *
 ******************************************************************************/
extern void el_CopyBytes(const uint8_t *src, uint8_t *dest, uint16_t srclen);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 * @pre         (optional) Which are the conditions to call this function? i.e. none
 *
 * @post        (optional) How has the status changed, after the function ran?
 *
 * TODO: A more detailed description.
 *
 * @see        (optional) Crossreference
 *
 ******************************************************************************/
extern void el_FillBytes(const uint8_t value, uint8_t *dest, uint16_t destlen);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 * @pre         (optional) Which are the conditions to call this function? i.e. none
 *
 * @post        (optional) How has the status changed, after the function ran?
 *
 * TODO: A more detailed description.
 *
 * @see        (optional) Crossreference
 *
 ******************************************************************************/
 extern void el_FillWords(const uint16_t value, uint16_t *dest, uint16_t destlen);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 * @pre         (optional) Which are the conditions to call this function? i.e. none
 *
 * @post        (optional) How has the status changed, after the function ran?
 *
 * TODO: A more detailed description.
 *
 * @see        (optional) Crossreference
 *
 ******************************************************************************/
extern void el_CopyAndFillBytes(const uint8_t *src, uint8_t *dest, uint16_t srclen, const uint8_t value, uint16_t destlen);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 * @pre         (optional) Which are the conditions to call this function? i.e. none
 *
 * @post        (optional) How has the status changed, after the function ran?
 *
 * TODO: A more detailed description.
 *
 * @see        (optional) Crossreference
 *
 ******************************************************************************/
extern bool_t el_CompareBytes(const uint8_t * a, const uint8_t * b, uint16_t size);
#endif /* EL_HELPER_H430_IAR_H_ */






