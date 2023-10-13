/*
 * Copyright (C) 2014 Melexis N.V.
 * git 5f2794fe
 */

/* 
 * Automatic generation by MlxCCT 1.10.0
 * Template version 2.8.3
 * 
 * File sections:
 * 1. General for use macros.
 * 2. Auxillary macros.
 * 3. I/O declarations:
 * 3.1 Hosting variables.
 * 3.2 Get macros.
 * 3.3 Ports.
 * 3.4 Dummy ports.
 * 4. Statistics 
 */

#ifndef IO1_H
#define IO1_H

#include <stdint.h>

/*****************************************************************************/
/* 1. General macros. */

/* 
 * Get an I/O bit field.
 * @param BLOCK is the digital block holding the given I/O field.
 * @param FIELD is the field to read.
 */

#ifndef IO_GET
#define IO_GET(BLOCK, FIELD) IO_ ## BLOCK ## __ ## FIELD ## _GET ()
#endif

/* 
 * Set one or multiple I/O bit fields (ports) to some values. The fields set 
 * have to be more than one, but less than 16.
 *
 * @param BLOCK is the digital block holding the given port.
 * @param FIELD the port to set.
 * @param VALUE the value of the port to set (should be in the value's range).
 */

#ifndef IO_SET
#define IO_SET(BLOCK, ...) \
	CCT_IO_SET (IO_ ## BLOCK, __VA_ARGS__, \
		DUMMY2,  0U, DUMMY3,  0U, DUMMY4,  0U, DUMMY5,  0U,  DUMMY6, 0U, \
		DUMMY7,  0U, DUMMY8,  0U, DUMMY9,  0U, DUMMY10, 0U, DUMMY11, 0U, \
		DUMMY12, 0U, DUMMY13, 0U, DUMMY14, 0U, DUMMY15, 0U, DUMMY16, 0U, \
		DUMMY17, 0U)
#endif

/*
 * Reports what is the first address occupied by I/O variable.
 */

#ifndef IO_ADDRESS_BEGIN
#define IO_ADDRESS_BEGIN (0x0002u)
#endif

/*
 * Reports what is the first address past the last one occupied by I/O variable.
 */

#ifndef IO_ADDRESS_END
#define IO_ADDRESS_END (0x0254u)
#endif

/*****************************************************************************/
/* 2. Auxillary (helper) macros. */
/* These macros are subject to change without notice! */

/* TODO: Optimize to always setb/clrb. */

#define CCT_IO_SET(B, \
	    F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  F5,  V5,  F6,  V6,  \
	    F7,  V7,  F8,  V8,  F9,  V9,  F10, V10, F11, V11, F12, V12, \
		F13, V13, F14, V14, F15, V15, F16, V16, F17, V17, \
		...) \
    if (CCT_IO_TEST_WRITABLE (B, \
		F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  \
		F9, F10, F11, F12, F13, F14, F15, F16) !=0u) \
	{ \
          extern volatile unsigned int B ## __ ## F1 ## _has_no_app_write_access; \
          (void)B ## __ ## F1 ## _has_no_app_write_access; \
	} \
    else if (CCT_IO_BIT_ACCESSIBLE (B, \
		F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  \
		F9, F10, F11, F12, F13, F14, F15, F16)) \
	{ \
		if (CCT_IO_BIT_ACCESSIBLE (B, F1)) { \
    		if ((V1 ) != 0u) { CCT_IO_SETB(B, F1);  } \
			else             { CCT_IO_CLRB(B, F1);  } \
		} \
		if (!CCT_IO_DUMMY (B, F2) && CCT_IO_BIT_ACCESSIBLE (B, F2)) { \
    		if ((V2 ) != 0u) { CCT_IO_SETB(B, F2);  } \
			else             { CCT_IO_CLRB(B, F2);  } \
		} \
		if (!CCT_IO_DUMMY (B, F3) && CCT_IO_BIT_ACCESSIBLE (B, F3)) { \
    		if ((V3 ) != 0u) { CCT_IO_SETB(B, F3);  } \
			else             { CCT_IO_CLRB(B, F3);  } \
		} \
		if (!CCT_IO_DUMMY (B, F4) && CCT_IO_BIT_ACCESSIBLE (B, F4)) { \
    		if ((V4 ) != 0u) { CCT_IO_SETB(B, F4);  } \
			else             { CCT_IO_CLRB(B, F4);  } \
		} \
		if (!CCT_IO_DUMMY (B, F5) && CCT_IO_BIT_ACCESSIBLE (B, F5)) { \
    		if ((V5 ) != 0u) { CCT_IO_SETB(B, F5);  } \
			else             { CCT_IO_CLRB(B, F5);  } \
		} \
		if (!CCT_IO_DUMMY (B, F6) && CCT_IO_BIT_ACCESSIBLE (B, F6)) { \
    		if ((V6 ) != 0u) { CCT_IO_SETB(B, F6);  } \
			else             { CCT_IO_CLRB(B, F6);  } \
		} \
		if (!CCT_IO_DUMMY (B, F7) && CCT_IO_BIT_ACCESSIBLE (B, F7)) { \
    		if ((V7 ) != 0u) { CCT_IO_SETB(B, F7);  } \
			else             { CCT_IO_CLRB(B, F7);  } \
		} \
		if (!CCT_IO_DUMMY (B, F8) && CCT_IO_BIT_ACCESSIBLE (B, F8)) { \
    		if ((V8 ) != 0u) { CCT_IO_SETB(B, F8);  } \
			else             { CCT_IO_CLRB(B, F8);  } \
		} \
		if (!CCT_IO_DUMMY (B, F9) && CCT_IO_BIT_ACCESSIBLE (B, F9)) { \
    		if ((V9 ) != 0u) { CCT_IO_SETB(B, F9);  } \
			else             { CCT_IO_CLRB(B, F9);  } \
		} \
		if (!CCT_IO_DUMMY (B, F10) && CCT_IO_BIT_ACCESSIBLE (B, F10)) { \
    		if ((V10) != 0u) { CCT_IO_SETB(B, F10);  } \
			else             { CCT_IO_CLRB(B, F10);  } \
		} \
		if (!CCT_IO_DUMMY (B, F11) && CCT_IO_BIT_ACCESSIBLE (B, F11)) { \
    		if ((V11) != 0u) { CCT_IO_SETB(B, F11);  } \
			else             { CCT_IO_CLRB(B, F11);  } \
		} \
		if (!CCT_IO_DUMMY (B, F12) && CCT_IO_BIT_ACCESSIBLE (B, F12)) { \
    		if ((V12) != 0u) { CCT_IO_SETB(B, F12);  } \
			else             { CCT_IO_CLRB(B, F12);  } \
		} \
		if (!CCT_IO_DUMMY (B, F13) && CCT_IO_BIT_ACCESSIBLE (B, F13)) { \
    		if ((V13) != 0u) { CCT_IO_SETB(B, F13);  } \
			else             { CCT_IO_CLRB(B, F13);  } \
		} \
		if (!CCT_IO_DUMMY (B, F14) && CCT_IO_BIT_ACCESSIBLE (B, F14)) { \
    		if ((V14) != 0u) { CCT_IO_SETB(B, F14);  } \
			else             { CCT_IO_CLRB(B, F14);  } \
		} \
		if (!CCT_IO_DUMMY (B, F15) && CCT_IO_BIT_ACCESSIBLE (B, F15)) { \
    		if ((V15) != 0u) { CCT_IO_SETB(B, F15);  } \
			else             { CCT_IO_CLRB(B, F15);  } \
		} \
		if (!CCT_IO_DUMMY (B, F16) && CCT_IO_BIT_ACCESSIBLE (B, F16)) { \
    		if ((V16) != 0u) { CCT_IO_SETB(B, F16);  } \
			else             { CCT_IO_CLRB(B, F16);  } \
		} \
	} \
    else if (CCT_IO_SAME_BYTE(B, \
    	F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  \
		F9, F10, F11, F12, F13, F14, F15, F16)) \
    { \
        CCT_IO_SET_WITHIN_BYTE(B, \
        F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  F5,  V5,  F6,  V6,  \
	    F7,  V7,  F8,  V8,  F9,  V9,  \
        ...) \
    } \
    else {  \
        CCT_IO_SET_WITHIN_WORD(B, \
        F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  F5,  V5,  F6,  V6,  \
	    F7,  V7,  F8,  V8,  F9,  V9,  F10, V10, F11, V11, F12, V12, \
		F13, V13, F14, V14, F15, V15, F16, V16, F17, V17, \
        ...) \
    }

#define CCT_IO_SET_WITHIN_WORD(B, \
	    F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  F5,  V5,  F6,  V6,  \
	    F7,  V7,  F8,  V8,  F9,  V9,  F10, V10, F11, V11, F12, V12, \
		F13, V13, F14, V14, F15, V15, F16, V16, F17, V17, \
		...) \
	if ((CCT_IO_HOST_UNUSED (B, F1) | CCT_IO_HOST_RISED (B, F1) \
		| CCT_IO_MASK (B, \
					F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16)) == 0xFFFFU) {\
		CCT_IO_HOST (B, F1) = 0u \
		| ((V1 ) << CCT_IO_OFFSET (B, F1 )) \
		| ((V2 ) << CCT_IO_OFFSET (B, F2 )) \
		| ((V3 ) << CCT_IO_OFFSET (B, F3 )) \
		| ((V4 ) << CCT_IO_OFFSET (B, F4 )) \
		| ((V5 ) << CCT_IO_OFFSET (B, F5 )) \
		| ((V6 ) << CCT_IO_OFFSET (B, F6 )) \
		| ((V7 ) << CCT_IO_OFFSET (B, F7 )) \
		| ((V8 ) << CCT_IO_OFFSET (B, F8 )) \
		| ((V9 ) << CCT_IO_OFFSET (B, F9 )) \
		| ((V10) << CCT_IO_OFFSET (B, F10)) \
		| ((V11) << CCT_IO_OFFSET (B, F11)) \
		| ((V12) << CCT_IO_OFFSET (B, F12)) \
		| ((V13) << CCT_IO_OFFSET (B, F13)) \
		| ((V14) << CCT_IO_OFFSET (B, F14)) \
		| ((V15) << CCT_IO_OFFSET (B, F15)) \
		| ((V16) << CCT_IO_OFFSET (B, F16)) \
			; \
	} \
	else { \
		CCT_IO_HOST (B, F1) = (CCT_IO_HOST (B, F1) \
			& ~(CCT_IO_HOST_RISED (B, F1) | CCT_IO_MASK (B, \
					F1, F2, F3, F4, F5, F6, F7, F8,   \
					F9, F10, F11, F12, F13, F14, F15, F16))) \
		| ((V1 ) << CCT_IO_OFFSET (B, F1 )) \
		| ((V2 ) << CCT_IO_OFFSET (B, F2 )) \
		| ((V3 ) << CCT_IO_OFFSET (B, F3 )) \
		| ((V4 ) << CCT_IO_OFFSET (B, F4 )) \
		| ((V5 ) << CCT_IO_OFFSET (B, F5 )) \
		| ((V6 ) << CCT_IO_OFFSET (B, F6 )) \
		| ((V7 ) << CCT_IO_OFFSET (B, F7 )) \
		| ((V8 ) << CCT_IO_OFFSET (B, F8 )) \
		| ((V9 ) << CCT_IO_OFFSET (B, F9 )) \
		| ((V10) << CCT_IO_OFFSET (B, F10)) \
		| ((V11) << CCT_IO_OFFSET (B, F11)) \
		| ((V12) << CCT_IO_OFFSET (B, F12)) \
		| ((V13) << CCT_IO_OFFSET (B, F13)) \
		| ((V14) << CCT_IO_OFFSET (B, F14)) \
		| ((V15) << CCT_IO_OFFSET (B, F15)) \
		| ((V16) << CCT_IO_OFFSET (B, F16)) \
			; \
	} \
	CCT_IO_ASSERT_WRITABLES (B, \
			F1, F2, F3, F4, F5, F6, F7, F8, \
			F9, F10, F11, F12, F13, F14, F15, F16); \
	CCT_IO_ASSERT_SAME_HOST (B, \
			F1, F2, F3, F4, F5, F6, F7, F8, \
			F9, F10, F11, F12, F13, F14, F15, F16); \
	CCT_IO_ASSERT_ARGS_FIT  (B, \
			F1, F2, F3, F4, F5, F6, F7, F8, \
			F9, F10, F11, F12, F13, F14, F15, F16, F17);

#define CCT_IO_SET_WITHIN_BYTE(B, \
	    F1,  V1,  F2,  V2,  F3,  V3,  F4,  V4,  \
        F5,  V5,  F6,  V6,  F7,  V7,  F8,  V8,  F9,  V9,  \
		...) \
	if ((CCT_IO_HOST_UNUSED_WITHIN_BYTE (B, F1) | CCT_IO_HOST_RISED_WITHIN_BYTE (B, F1) \
		| CCT_IO_MASK_WITHIN_BYTE (B, \
					F1, F2, F3, F4, F5, F6, F7, F8)) == 0xFFU) { \
		CCT_IO_BYTE_HOST (B, F1) = (uint8_t)(0u \
    	| ((V1 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F1 ))\
    	| ((V2 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F2 ))\
    	| ((V3 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F3 ))\
    	| ((V4 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F4 ))\
    	| ((V5 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F5 ))\
    	| ((V6 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F6 ))\
    	| ((V7 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F7 ))\
    	| ((V8 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F8 ))\
			); \
	} \
	else { \
		CCT_IO_BYTE_HOST (B, F1) = (uint8_t)((CCT_IO_BYTE_HOST (B, F1) \
			& ~(CCT_IO_HOST_RISED_WITHIN_BYTE (B, F1) | CCT_IO_MASK_WITHIN_BYTE (B, \
					F1, F2, F3, F4, F5, F6, F7, F8)))   \
    	| ((V1 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F1 ))\
    	| ((V2 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F2 ))\
    	| ((V3 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F3 ))\
    	| ((V4 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F4 ))\
    	| ((V5 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F5 ))\
    	| ((V6 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F6 ))\
    	| ((V7 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F7 ))\
    	| ((V8 ) << CCT_IO_OFFSET_WITHIN_BYTE (B, F8 ))\
			); \
	}


#define CCT_IO_SETB(B, F)               CCT_IO_SETB2(CCT_IO_BYTE_ADDRESS(B, F), CCT_IO_OFFSET_WITHIN_BYTE(B, F))
#define CCT_IO_SETB2(IO_BYTE, IO_BIT)   CCT_IO_SETB3(IO_BYTE, IO_BIT)
#define CCT_IO_SETB3(IO_BYTE, IO_BIT)   asm volatile ("setb io:" # IO_BYTE "." # IO_BIT "")

#define CCT_IO_CLRB(B, F)               CCT_IO_CLRB2(CCT_IO_BYTE_ADDRESS(B, F), CCT_IO_OFFSET_WITHIN_BYTE(B, F))
#define CCT_IO_CLRB2(IO_BYTE, IO_BIT)   CCT_IO_CLRB3(IO_BYTE, IO_BIT)
#define CCT_IO_CLRB3(IO_BYTE, IO_BIT)   asm volatile ("clrb io:" # IO_BYTE "." # IO_BIT "")


/* Return a dereferenced pointer to the address where the given I/O resides. */

#define IO_HOST(B, F) CCT_IO_HOST2 (IO_ ## B ## __ ## F)
#define CCT_IO_HOST(B, F) CCT_IO_HOST2 (B ## __ ## F)
#define CCT_IO_HOST2(F) CCT_IO_HOST3 (F)
#define CCT_IO_HOST3(...) CCT_IO_HOST4 (__VA_ARGS__)
#define CCT_IO_HOST4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (H)

/* Return a dereferenced pointer to the byte where the given field F resides. */

#define IO_BYTE_HOST(B, F) CCT_IO_BYTE_HOST2 (IO_ ## B ## __ ## F)
#define CCT_IO_BYTE_HOST(B, F) CCT_IO_BYTE_HOST2 (B ## __ ## F)
#define CCT_IO_BYTE_HOST2(F) CCT_IO_BYTE_HOST3 (F)
#define CCT_IO_BYTE_HOST3(...) CCT_IO_BYTE_HOST4 (__VA_ARGS__)
#define CCT_IO_BYTE_HOST4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (HB)

/* Return the word address where the given I/O resides. */

#define IO_ADDRESS(B, F) CCT_IO_ADDRESS2 (IO_ ## B ## __ ## F)
#define CCT_IO_ADDRESS(B, F) CCT_IO_ADDRESS2 (B ## __ ## F)
#define CCT_IO_ADDRESS2(F) CCT_IO_ADDRESS3 (F)
#define CCT_IO_ADDRESS3(...) CCT_IO_ADDRESS4 (__VA_ARGS__)
#define CCT_IO_ADDRESS4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (A)

/* Return the byte address where the given I/O resides. */

#define IO_BYTE_ADDRESS(B, F)  CCT_IO_BYTE_ADDRESS2 (IO_ ## B ## __ ## F)
#define CCT_IO_BYTE_ADDRESS(B, F) CCT_IO_BYTE_ADDRESS2 (B ## __ ## F)
#define CCT_IO_BYTE_ADDRESS2(F)   CCT_IO_BYTE_ADDRESS3 (F)
#define CCT_IO_BYTE_ADDRESS3(...) CCT_IO_BYTE_ADDRESS4 (__VA_ARGS__)
#define CCT_IO_BYTE_ADDRESS4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (AB)

/* Get a mask of all risable I/Os for the host of the given I/O. */

#define IO_HOST_RISED(B, F) CCT_IO_HOST_RISED2 (IO_ ## B ## __ ## F)
#define CCT_IO_HOST_RISED(B, F) CCT_IO_HOST_RISED2 (B ## __ ## F)
#define CCT_IO_HOST_RISED2(F) CCT_IO_HOST_RISED3 (F)
#define CCT_IO_HOST_RISED3(...) CCT_IO_HOST_RISED4 (__VA_ARGS__)
#define CCT_IO_HOST_RISED4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (R)

/* Returns a mask of raised (W1C) bits in the byte that stores specified field F */
#define IO_HOST_RISED_WITHIN_BYTE(B, F)     CCT_IO_HOST_RISED_WITHIN_BYTE(IO_ ## B, F)
#define CCT_IO_HOST_RISED_WITHIN_BYTE(B, F) \
    ((((uint16_t)CCT_IO_BYTE_ADDRESS(B, F) & 1u) == 0u) ? \
        CCT_IO_HOST_RISED(B, F) & 0x00FFu : \
        CCT_IO_HOST_RISED(B, F) >> 8u)

/* Get a mask of all unused host bits for giveb I/O field. */

#define IO_HOST_UNUSED(B, F) CCT_IO_HOST_UNUSED2 (IO_ ## B ## __ ## F)
#define CCT_IO_HOST_UNUSED(B, F) CCT_IO_HOST_UNUSED2 (B ## __ ## F)
#define CCT_IO_HOST_UNUSED2(F) CCT_IO_HOST_UNUSED3 (F)
#define CCT_IO_HOST_UNUSED3(...) CCT_IO_HOST_UNUSED4 (__VA_ARGS__)
#define CCT_IO_HOST_UNUSED4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (U)

/* Returns a mask of unused bits in the byte that stores specified field F */
#define IO_HOST_UNUSED_WITHIN_BYTE(B, F)     CCT_IO_HOST_UNUSED_WITHIN_BYTE(IO_ ## B, F)
#define CCT_IO_HOST_UNUSED_WITHIN_BYTE(B, F) \
    ((((uint16_t)CCT_IO_BYTE_ADDRESS(B, F) & 1u) == 0u) ? \
        CCT_IO_HOST_UNUSED(B, F) & 0x00FFu : \
        CCT_IO_HOST_UNUSED(B, F) >> 8u)

/* Get the bit offset (within word) from where the given I/O begins in its host. */

#define IO_OFFSET(B, F) CCT_IO_OFFSET2 (IO_ ## B ## __ ## F)
#define CCT_IO_OFFSET(B, F) CCT_IO_OFFSET2 (B ## __ ## F)
#define CCT_IO_OFFSET2(F) CCT_IO_OFFSET3 (F)
#define CCT_IO_OFFSET3(...) CCT_IO_OFFSET4 (__VA_ARGS__)
#define CCT_IO_OFFSET4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (O)

/* Get the bit offset (within byte) from where the given I/O begins in its host. */

#define IO_OFFSET_WITHIN_BYTE(B, F)  CCT_IO_OFFSET_WITHIN_BYTE2 (IO_ ## B ## __ ## F)
#define CCT_IO_OFFSET_WITHIN_BYTE(B, F) CCT_IO_OFFSET_WITHIN_BYTE2 (B ## __ ## F)
#define CCT_IO_OFFSET_WITHIN_BYTE2(F)   CCT_IO_OFFSET_WITHIN_BYTE3 (F)
#define CCT_IO_OFFSET_WITHIN_BYTE3(...) CCT_IO_OFFSET_WITHIN_BYTE4 (__VA_ARGS__)
#define CCT_IO_OFFSET_WITHIN_BYTE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (OB)

/* Checks whether the given I/O is dummy one i.e. it is a MlxCCT private. */

#define IO_DUMMY(B, F) CCT_IO_DUMMY2 (IO_ ## B ## __ ## F)
#define CCT_IO_DUMMY(B, F) CCT_IO_DUMMY2 (B ## __ ## F)
#define CCT_IO_DUMMY2(F) CCT_IO_DUMMY3 (F)
#define CCT_IO_DUMMY3(...) CCT_IO_DUMMY4 (__VA_ARGS__)
#define CCT_IO_DUMMY4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) ((B) == 0u)

/* Checks whether the given I/Os can be bit accessed (setb, clrb, ...). */

#define IO_BIT_ACCESSIBLE(B, F, ...) \
	CCT_IO_BIT_ACCESSIBLE2 (IO_ ## B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_BIT_ACCESSIBLE(B, F, ...) \
	CCT_IO_BIT_ACCESSIBLE2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_BIT_ACCESSIBLE2(B, F1, F2, F3, F4, F5, F6, F7, F8,  \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	(  CCT_IO_BIT_ACCESSIBLE3 (B##__##F1 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F2 )\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F3 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F4 )\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F5 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F6 )\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F7 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F8 )\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F9 ) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F10)\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F11) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F12)\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F13) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F14)\
	&& CCT_IO_BIT_ACCESSIBLE3 (B##__##F15) && CCT_IO_BIT_ACCESSIBLE3 (B##__##F16))
#define CCT_IO_BIT_ACCESSIBLE3(...) CCT_IO_BIT_ACCESSIBLE4 (__VA_ARGS__)
#define CCT_IO_BIT_ACCESSIBLE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (( ((B) & 1u) != 0u) || ((B) == 0u))

/* Checks if byte access can be used for given I/Os fields. */

#define IO_BYTE_ACCESSIBLE(B, F, ...)  CCT_IO_BYTE_ACCESSIBLE(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_BYTE_ACCESSIBLE(B, F, ...) \
	CCT_IO_BYTE_ACCESSIBLE2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_BYTE_ACCESSIBLE2(B, F1, F2, F3, F4, F5, F6, F7, F8,  \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	(  CCT_IO_BYTE_ACCESSIBLE3 (B##__##F1 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F2 )\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F3 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F4 )\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F5 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F6 )\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F7 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F8 )\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F9 ) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F10)\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F11) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F12)\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F13) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F14)\
	&& CCT_IO_BYTE_ACCESSIBLE3 (B##__##F15) && CCT_IO_BYTE_ACCESSIBLE3 (B##__##F16))
#define CCT_IO_BYTE_ACCESSIBLE3(...) CCT_IO_BYTE_ACCESSIBLE4 (__VA_ARGS__)
#define CCT_IO_BYTE_ACCESSIBLE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (( ((B) & 4u) != 0u) || ((B) == 0u))

/* Checks if TEST-ONLY mode access can be used for given I/Os fields (for WRITE). */

#define IO_TEST_WRITABLE(B, F, ...)  CCT_IO_TEST_WRITABLE(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_TEST_WRITABLE(B, F, ...) \
	CCT_IO_TEST_WRITABLE2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_TEST_WRITABLE2(B, F1, F2, F3, F4, F5, F6, F7, F8,  \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	(  CCT_IO_TEST_WRITABLE3 (B##__##F1 ) && CCT_IO_TEST_WRITABLE3 (B##__##F2 )\
	&& CCT_IO_TEST_WRITABLE3 (B##__##F3 ) && CCT_IO_TEST_WRITABLE3 (B##__##F4 )\
	&& CCT_IO_TEST_WRITABLE3 (B##__##F5 ) && CCT_IO_TEST_WRITABLE3 (B##__##F6 )\
	&& CCT_IO_TEST_WRITABLE3 (B##__##F7 ) && CCT_IO_TEST_WRITABLE3 (B##__##F8 )\
	&& CCT_IO_TEST_WRITABLE3 (B##__##F9 ) && CCT_IO_TEST_WRITABLE3 (B##__##F10)\
	&& CCT_IO_TEST_WRITABLE3 (B##__##F11) && CCT_IO_TEST_WRITABLE3 (B##__##F12)\
	&& CCT_IO_TEST_WRITABLE3 (B##__##F13) && CCT_IO_TEST_WRITABLE3 (B##__##F14)\
	&& CCT_IO_TEST_WRITABLE3 (B##__##F15) && CCT_IO_TEST_WRITABLE3 (B##__##F16))
#define CCT_IO_TEST_WRITABLE3(...) CCT_IO_TEST_WRITABLE4 (__VA_ARGS__)
#define CCT_IO_TEST_WRITABLE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (( ((B) & 16u) != 0u) || ((B) == 0u))


/* Get a compound mask for all the given I/Os. */

#define IO_MASK(B, F, ...) \
	CCT_IO_MASK2 (IO_ ## B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_MASK(B, F, ...) \
	CCT_IO_MASK2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_MASK2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	( CCT_IO_MASK3 (B ## __ ## F1 ) | CCT_IO_MASK3 (B ## __ ## F2 ) \
	| CCT_IO_MASK3 (B ## __ ## F3 ) | CCT_IO_MASK3 (B ## __ ## F4 ) \
	| CCT_IO_MASK3 (B ## __ ## F5 ) | CCT_IO_MASK3 (B ## __ ## F6 ) \
	| CCT_IO_MASK3 (B ## __ ## F7 ) | CCT_IO_MASK3 (B ## __ ## F8 ) \
	| CCT_IO_MASK3 (B ## __ ## F9 ) | CCT_IO_MASK3 (B ## __ ## F10) \
	| CCT_IO_MASK3 (B ## __ ## F11) | CCT_IO_MASK3 (B ## __ ## F12) \
	| CCT_IO_MASK3 (B ## __ ## F13) | CCT_IO_MASK3 (B ## __ ## F14) \
	| CCT_IO_MASK3 (B ## __ ## F15) | CCT_IO_MASK3 (B ## __ ## F16))
#define CCT_IO_MASK3(...) CCT_IO_MASK4 (__VA_ARGS__)
#define CCT_IO_MASK4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (M)

/* Get a compound mask for all the given I/Os. */

#define IO_MASK_WITHIN_BYTE(B, F, ...) \
	CCT_IO_MASK_WITHIN_BYTE2 (IO_ ## B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8)
#define CCT_IO_MASK_WITHIN_BYTE(B, F, ...) \
	CCT_IO_MASK_WITHIN_BYTE2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8)
#define CCT_IO_MASK_WITHIN_BYTE2(B, F1, F2, F3, F4, F5, F6, F7, F8, ...) \
	( CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F1 ) | CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F2 ) \
	| CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F3 ) | CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F4 ) \
	| CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F5 ) | CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F6 ) \
	| CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F7 ) | CCT_IO_MASK_WITHIN_BYTE3 (B ## __ ## F8 ))
#define CCT_IO_MASK_WITHIN_BYTE3(...) CCT_IO_MASK_WITHIN_BYTE4 (__VA_ARGS__)
#define CCT_IO_MASK_WITHIN_BYTE4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) (MB)

/* Returns true if byte access is possible for the requested fields, i.e. 
    - all requested fields (Fn) are located in the byte
    - hadrware supports byte access for requested fileds
*/

#define IO_SAME_BYTE(B, F, ...)  CCT_IO_SAME_BYTE(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_SAME_BYTE(B, F, ...) \
    CCT_IO_SAME_BYTE2 (B, F, __VA_ARGS__, \
    DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
    DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_SAME_BYTE2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
                            F9, F10, F11, F12, F13, F14, F15, F16, ...) \
    CCT_IO_BYTE_ACCESSIBLE (B, F1, F2, F3, F4, F5, F6, F7, F8, \
                               F9, F10, F11, F12, F13, F14, F15, F16) \
    && (CCT_IO_DUMMY (B, F2 ) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F2 )) \
    && (CCT_IO_DUMMY (B, F3 ) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F3 )) \
    && (CCT_IO_DUMMY (B, F4 ) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F4 )) \
    && (CCT_IO_DUMMY (B, F5 ) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F5 )) \
    && (CCT_IO_DUMMY (B, F6 ) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F6 )) \
    && (CCT_IO_DUMMY (B, F7 ) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F7 )) \
    && (CCT_IO_DUMMY (B, F8 ) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F8 )) \
    && (CCT_IO_DUMMY (B, F9 ) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F9 )) \
    && (CCT_IO_DUMMY (B, F10) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F10)) \
    && (CCT_IO_DUMMY (B, F11) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F11)) \
    && (CCT_IO_DUMMY (B, F12) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F12)) \
    && (CCT_IO_DUMMY (B, F13) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F13)) \
    && (CCT_IO_DUMMY (B, F14) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F14)) \
    && (CCT_IO_DUMMY (B, F15) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F15)) \
    && (CCT_IO_DUMMY (B, F16) || CCT_IO_BYTE_ADDRESS (B, F1)==CCT_IO_BYTE_ADDRESS (B, F16))

/* Type of somewhat expressive, compile-time check. */

#define IO_STATIC_ASSERT(MSG, EXPR) { enum { MSG = 1/(!!(EXPR)) }; }

/* Compile time assert that the given I/Os can be written. */

#define IO_ASSERT_WRITABLES(B, F, ...)  CCT_IO_ASSERT_WRITABLES(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_ASSERT_WRITABLES(B, F, ...) \
	CCT_IO_WRITABLES2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_WRITABLES2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	CCT_IO_WRITABLES3 (F1 , B##__##F1 ) CCT_IO_WRITABLES3 (F2 , B##__##F2 ) \
	CCT_IO_WRITABLES3 (F3 , B##__##F3 ) CCT_IO_WRITABLES3 (F4 , B##__##F4 ) \
	CCT_IO_WRITABLES3 (F5 , B##__##F5 ) CCT_IO_WRITABLES3 (F6 , B##__##F6 ) \
	CCT_IO_WRITABLES3 (F7 , B##__##F7 ) CCT_IO_WRITABLES3 (F8 , B##__##F8 ) \
	CCT_IO_WRITABLES3 (F9 , B##__##F9 ) CCT_IO_WRITABLES3 (F10, B##__##F10) \
	CCT_IO_WRITABLES3 (F11, B##__##F11) CCT_IO_WRITABLES3 (F12, B##__##F12) \
	CCT_IO_WRITABLES3 (F13, B##__##F13) CCT_IO_WRITABLES3 (F14, B##__##F14) \
	CCT_IO_WRITABLES3 (F15, B##__##F15) CCT_IO_WRITABLES3 (F16, B##__##F16)
#define CCT_IO_WRITABLES3(...) CCT_IO_WRITABLES4 (__VA_ARGS__)
#if !defined(UNITTEST)
#define CCT_IO_WRITABLES4(F, H, HB, A, AB, M, MB, O, OB, W, B, U, R) \
	IO_STATIC_ASSERT (F ## _IS_WRITABLE, W)
#else
#define CCT_IO_WRITABLES4(F, H, HB, A, AB, M, MB, O, OB, W, B, U, R)
#endif

/* Compile time assert that the given I/Os share the same host. */

#define IO_ASSERT_SAME_HOST(B, F, ...)  CCT_IO_ASSERT_SAME_HOST(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_ASSERT_SAME_HOST(B, F, ...) \
	CCT_IO_SAME_HOST2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16)
#define CCT_IO_SAME_HOST2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16, ...) \
	IO_STATIC_ASSERT (FIELDS_SHARE_SAME_ADDRESS, \
	   (CCT_IO_DUMMY (B, F2 ) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F2 )) \
	&& (CCT_IO_DUMMY (B, F3 ) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F3 )) \
	&& (CCT_IO_DUMMY (B, F4 ) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F4 )) \
	&& (CCT_IO_DUMMY (B, F5 ) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F5 )) \
	&& (CCT_IO_DUMMY (B, F6 ) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F6 )) \
	&& (CCT_IO_DUMMY (B, F7 ) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F7 )) \
	&& (CCT_IO_DUMMY (B, F8 ) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F8 )) \
	&& (CCT_IO_DUMMY (B, F9 ) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F9 )) \
	&& (CCT_IO_DUMMY (B, F10) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F10)) \
	&& (CCT_IO_DUMMY (B, F11) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F11)) \
	&& (CCT_IO_DUMMY (B, F12) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F12)) \
	&& (CCT_IO_DUMMY (B, F13) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F13)) \
	&& (CCT_IO_DUMMY (B, F14) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F14)) \
	&& (CCT_IO_DUMMY (B, F15) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F15)) \
	&& (CCT_IO_DUMMY (B, F16) || CCT_IO_ADDRESS (B, F1)==CCT_IO_ADDRESS (B, F16)))

/* Assert whether these arguments fits in the IO_ macros. */

#define IO_ASSERT_ARGS_FIT(B, F, ...)  CCT_IO_ASSERT_ARGS_FIT(IO_ ## B, F, __VA_ARGS__)
#define CCT_IO_ASSERT_ARGS_FIT(B, F, ...) \
	CCT_IO_ASSERT_ARGS_FIT2 (B, F, __VA_ARGS__, \
	DUMMY2,  DUMMY3,  DUMMY4,  DUMMY5,  DUMMY6,  DUMMY7,  DUMMY8,  DUMMY9, \
	DUMMY10, DUMMY11, DUMMY12, DUMMY13, DUMMY14, DUMMY15, DUMMY16, DUMMY17)
#define CCT_IO_ASSERT_ARGS_FIT2(B, F1, F2, F3, F4, F5, F6, F7, F8, \
					F9, F10, F11, F12, F13, F14, F15, F16, F17, ...) \
	CCT_IO_ASSERT_ARGS_FIT3 (B ## __ ## F17)
#define CCT_IO_ASSERT_ARGS_FIT3(...) CCT_IO_ASSERT_ARGS_FIT4 (__VA_ARGS__)
#define CCT_IO_ASSERT_ARGS_FIT4(H, HB, A, AB, M, MB, O, OB, W, B, U, R) \
	IO_STATIC_ASSERT (ARGUMENTS_FIT_IN_MACRO, (B) == 0u)

/*****************************************************************************/
/* 3. I/O declarations. */

/*****************************************************************************/
/* 3.1 Hosting variables. */

#if !defined(IO_CONST)
#   if defined(UNITTEST)
#       define IO_CONST
#   else
#       define IO_CONST const
#   endif
#endif

/* Word variables (16-bit) */
extern volatile uint16_t io0x2 __attribute__((io, addr(0x2)));
extern volatile uint16_t io0x6 __attribute__((io, addr(0x6)));
extern volatile uint16_t io0x8 __attribute__((io, addr(0x8)));
extern volatile uint16_t io0xa __attribute__((io, addr(0xa)));
extern volatile uint16_t io0xc __attribute__((io, addr(0xc)));
extern volatile uint16_t error_IO_AWD__ACK_has_no_word_access; /* to catch attempts of word access when not supported */
extern volatile uint16_t error_IO_AWD__WIN_OPEN_has_no_word_access; /* to catch attempts of word access when not supported */
extern volatile uint16_t io0x10 __attribute__((io, addr(0x10)));
extern volatile uint16_t io0x12 __attribute__((io, addr(0x12)));
extern volatile uint16_t io0x14 __attribute__((io, addr(0x14)));
extern volatile uint16_t io0x16 __attribute__((io, addr(0x16)));
extern volatile uint16_t io0x18 __attribute__((io, addr(0x18)));
extern volatile uint16_t io0x1a __attribute__((io, addr(0x1a)));
extern volatile uint16_t io0x1c __attribute__((io, addr(0x1c)));
extern volatile uint16_t io0x1e __attribute__((io, addr(0x1e)));
extern volatile uint16_t io0x20 __attribute__((io, addr(0x20)));
extern volatile uint16_t io0x22 __attribute__((io, addr(0x22)));
extern volatile uint16_t io0x24 __attribute__((io, addr(0x24)));
extern volatile uint16_t io0x28 __attribute__((io, addr(0x28)));
extern volatile uint16_t io0x2a __attribute__((io, addr(0x2a)));
extern IO_CONST volatile uint16_t io0x2c __attribute__((io, addr(0x2c)));
extern volatile uint16_t io0x2e __attribute__((io, addr(0x2e)));
extern volatile uint16_t io0x30 __attribute__((io, addr(0x30)));
extern volatile uint16_t io0x32 __attribute__((io, addr(0x32)));
extern IO_CONST volatile uint16_t io0x34 __attribute__((io, addr(0x34)));
extern volatile uint16_t io0x36 __attribute__((io, addr(0x36)));
extern volatile uint16_t io0x38 __attribute__((io, addr(0x38)));
extern volatile uint16_t io0x3a __attribute__((io, addr(0x3a)));
extern volatile uint16_t io0x3c __attribute__((io, addr(0x3c)));
extern volatile uint16_t io0x3e __attribute__((io, addr(0x3e)));
extern volatile uint16_t io0x40 __attribute__((nodp, addr(0x40)));
extern volatile uint16_t io0x42 __attribute__((nodp, addr(0x42)));
extern volatile uint16_t io0x44 __attribute__((nodp, addr(0x44)));
extern volatile uint16_t io0x46 __attribute__((nodp, addr(0x46)));
extern volatile uint16_t io0x48 __attribute__((nodp, addr(0x48)));
extern volatile uint16_t io0x4a __attribute__((nodp, addr(0x4a)));
extern volatile uint16_t io0x4c __attribute__((nodp, addr(0x4c)));
extern volatile uint16_t io0x4e __attribute__((nodp, addr(0x4e)));
extern volatile uint16_t io0x50 __attribute__((nodp, addr(0x50)));
extern volatile uint16_t io0x52 __attribute__((nodp, addr(0x52)));
extern volatile uint16_t io0x54 __attribute__((nodp, addr(0x54)));
extern volatile uint16_t io0x6a __attribute__((nodp, addr(0x6a)));
extern volatile uint16_t io0x6c __attribute__((nodp, addr(0x6c)));
extern volatile uint16_t io0x6e __attribute__((nodp, addr(0x6e)));
extern volatile uint16_t io0x70 __attribute__((nodp, addr(0x70)));
extern volatile uint16_t io0x88 __attribute__((nodp, addr(0x88)));
extern volatile uint16_t io0x8a __attribute__((nodp, addr(0x8a)));
extern volatile uint16_t io0x8c __attribute__((nodp, addr(0x8c)));
extern volatile uint16_t io0x8e __attribute__((nodp, addr(0x8e)));
extern volatile uint16_t io0x90 __attribute__((nodp, addr(0x90)));
extern IO_CONST volatile uint16_t io0xbe __attribute__((nodp, addr(0xbe)));
extern volatile uint16_t io0xc0 __attribute__((nodp, addr(0xc0)));
extern IO_CONST volatile uint16_t io0x100 __attribute__((nodp, addr(0x100)));
extern IO_CONST volatile uint16_t io0x102 __attribute__((nodp, addr(0x102)));
extern volatile uint16_t io0x104 __attribute__((nodp, addr(0x104)));
extern IO_CONST volatile uint16_t io0x106 __attribute__((nodp, addr(0x106)));
extern IO_CONST volatile uint16_t io0x108 __attribute__((nodp, addr(0x108)));
extern IO_CONST volatile uint16_t io0x10a __attribute__((nodp, addr(0x10a)));
extern IO_CONST volatile uint16_t io0x10c __attribute__((nodp, addr(0x10c)));
extern volatile uint16_t io0x10e __attribute__((nodp, addr(0x10e)));
extern volatile uint16_t io0x110 __attribute__((nodp, addr(0x110)));
extern volatile uint16_t io0x112 __attribute__((nodp, addr(0x112)));
extern volatile uint16_t io0x114 __attribute__((nodp, addr(0x114)));
extern volatile uint16_t io0x116 __attribute__((nodp, addr(0x116)));
extern volatile uint16_t io0x118 __attribute__((nodp, addr(0x118)));
extern volatile uint16_t io0x11a __attribute__((nodp, addr(0x11a)));
extern IO_CONST volatile uint16_t io0x11c __attribute__((nodp, addr(0x11c)));
extern volatile uint16_t io0x11e __attribute__((nodp, addr(0x11e)));
extern volatile uint16_t io0x120 __attribute__((nodp, addr(0x120)));
extern volatile uint16_t io0x122 __attribute__((nodp, addr(0x122)));
extern volatile uint16_t io0x124 __attribute__((nodp, addr(0x124)));
extern volatile uint16_t io0x126 __attribute__((nodp, addr(0x126)));
extern volatile uint16_t io0x128 __attribute__((nodp, addr(0x128)));
extern volatile uint16_t io0x12a __attribute__((nodp, addr(0x12a)));
extern volatile uint16_t io0x12c __attribute__((nodp, addr(0x12c)));
extern volatile uint16_t io0x12e __attribute__((nodp, addr(0x12e)));
extern IO_CONST volatile uint16_t io0x130 __attribute__((nodp, addr(0x130)));
extern volatile uint16_t io0x132 __attribute__((nodp, addr(0x132)));
extern volatile uint16_t io0x134 __attribute__((nodp, addr(0x134)));
extern volatile uint16_t io0x136 __attribute__((nodp, addr(0x136)));
extern volatile uint16_t io0x138 __attribute__((nodp, addr(0x138)));
extern IO_CONST volatile uint16_t io0x13a __attribute__((nodp, addr(0x13a)));
extern volatile uint16_t io0x13c __attribute__((nodp, addr(0x13c)));
extern volatile uint16_t io0x13e __attribute__((nodp, addr(0x13e)));
extern volatile uint16_t io0x140 __attribute__((nodp, addr(0x140)));
extern volatile uint16_t io0x142 __attribute__((nodp, addr(0x142)));
extern IO_CONST volatile uint16_t io0x144 __attribute__((nodp, addr(0x144)));
extern volatile uint16_t io0x146 __attribute__((nodp, addr(0x146)));
extern volatile uint16_t io0x148 __attribute__((nodp, addr(0x148)));
extern volatile uint16_t io0x14a __attribute__((nodp, addr(0x14a)));
extern volatile uint16_t io0x14c __attribute__((nodp, addr(0x14c)));
extern IO_CONST volatile uint16_t io0x14e __attribute__((nodp, addr(0x14e)));
extern volatile uint16_t io0x150 __attribute__((nodp, addr(0x150)));
extern volatile uint16_t io0x152 __attribute__((nodp, addr(0x152)));
extern volatile uint16_t io0x154 __attribute__((nodp, addr(0x154)));
extern volatile uint16_t io0x156 __attribute__((nodp, addr(0x156)));
extern IO_CONST volatile uint16_t io0x158 __attribute__((nodp, addr(0x158)));
extern volatile uint16_t io0x15a __attribute__((nodp, addr(0x15a)));
extern volatile uint16_t io0x15c __attribute__((nodp, addr(0x15c)));
extern volatile uint16_t io0x15e __attribute__((nodp, addr(0x15e)));
extern volatile uint16_t io0x160 __attribute__((nodp, addr(0x160)));
extern IO_CONST volatile uint16_t io0x162 __attribute__((nodp, addr(0x162)));
extern volatile uint16_t io0x164 __attribute__((nodp, addr(0x164)));
extern volatile uint16_t io0x166 __attribute__((nodp, addr(0x166)));
extern volatile uint16_t io0x168 __attribute__((nodp, addr(0x168)));
extern volatile uint16_t io0x16a __attribute__((nodp, addr(0x16a)));
extern IO_CONST volatile uint16_t io0x16c __attribute__((nodp, addr(0x16c)));
extern volatile uint16_t io0x16e __attribute__((nodp, addr(0x16e)));
extern volatile uint16_t io0x170 __attribute__((nodp, addr(0x170)));
extern volatile uint16_t io0x172 __attribute__((nodp, addr(0x172)));
extern volatile uint16_t io0x174 __attribute__((nodp, addr(0x174)));
extern IO_CONST volatile uint16_t io0x176 __attribute__((nodp, addr(0x176)));
extern volatile uint16_t io0x178 __attribute__((nodp, addr(0x178)));
extern volatile uint16_t io0x17a __attribute__((nodp, addr(0x17a)));
extern volatile uint16_t io0x17c __attribute__((nodp, addr(0x17c)));
extern volatile uint16_t io0x17e __attribute__((nodp, addr(0x17e)));
extern IO_CONST volatile uint16_t io0x180 __attribute__((nodp, addr(0x180)));
extern volatile uint16_t io0x182 __attribute__((nodp, addr(0x182)));
extern volatile uint16_t io0x184 __attribute__((nodp, addr(0x184)));
extern volatile uint16_t io0x186 __attribute__((nodp, addr(0x186)));
extern volatile uint16_t io0x188 __attribute__((nodp, addr(0x188)));
extern IO_CONST volatile uint16_t io0x18a __attribute__((nodp, addr(0x18a)));
extern volatile uint16_t io0x18c __attribute__((nodp, addr(0x18c)));
extern volatile uint16_t io0x18e __attribute__((nodp, addr(0x18e)));
extern volatile uint16_t io0x190 __attribute__((nodp, addr(0x190)));
extern volatile uint16_t io0x192 __attribute__((nodp, addr(0x192)));
extern IO_CONST volatile uint16_t io0x194 __attribute__((nodp, addr(0x194)));
extern volatile uint16_t io0x196 __attribute__((nodp, addr(0x196)));
extern volatile uint16_t io0x198 __attribute__((nodp, addr(0x198)));
extern volatile uint16_t io0x19a __attribute__((nodp, addr(0x19a)));
extern volatile uint16_t io0x19c __attribute__((nodp, addr(0x19c)));
extern volatile uint16_t io0x19e __attribute__((nodp, addr(0x19e)));
extern volatile uint16_t io0x1a0 __attribute__((nodp, addr(0x1a0)));
extern volatile uint16_t io0x1a2 __attribute__((nodp, addr(0x1a2)));
extern volatile uint16_t io0x1a4 __attribute__((nodp, addr(0x1a4)));
extern volatile uint16_t io0x1a6 __attribute__((nodp, addr(0x1a6)));
extern volatile uint16_t io0x1a8 __attribute__((nodp, addr(0x1a8)));
extern volatile uint16_t io0x1aa __attribute__((nodp, addr(0x1aa)));
extern volatile uint16_t io0x1ac __attribute__((nodp, addr(0x1ac)));
extern volatile uint16_t io0x1ae __attribute__((nodp, addr(0x1ae)));
extern volatile uint16_t io0x1b0 __attribute__((nodp, addr(0x1b0)));
extern volatile uint16_t io0x1b2 __attribute__((nodp, addr(0x1b2)));
extern volatile uint16_t io0x1b4 __attribute__((nodp, addr(0x1b4)));
extern volatile uint16_t io0x1b6 __attribute__((nodp, addr(0x1b6)));
extern volatile uint16_t io0x1b8 __attribute__((nodp, addr(0x1b8)));
extern volatile uint16_t io0x1ba __attribute__((nodp, addr(0x1ba)));
extern volatile uint16_t io0x1bc __attribute__((nodp, addr(0x1bc)));
extern volatile uint16_t io0x1be __attribute__((nodp, addr(0x1be)));
extern volatile uint16_t io0x1c0 __attribute__((nodp, addr(0x1c0)));
extern volatile uint16_t io0x1c2 __attribute__((nodp, addr(0x1c2)));
extern volatile uint16_t io0x1c4 __attribute__((nodp, addr(0x1c4)));
extern volatile uint16_t io0x1c6 __attribute__((nodp, addr(0x1c6)));
extern volatile uint16_t io0x1c8 __attribute__((nodp, addr(0x1c8)));
extern volatile uint16_t io0x1d2 __attribute__((nodp, addr(0x1d2)));
extern volatile uint16_t io0x1d4 __attribute__((nodp, addr(0x1d4)));
extern volatile uint16_t io0x1d6 __attribute__((nodp, addr(0x1d6)));
extern volatile uint16_t io0x1d8 __attribute__((nodp, addr(0x1d8)));
extern volatile uint16_t io0x1da __attribute__((nodp, addr(0x1da)));
extern volatile uint16_t io0x1dc __attribute__((nodp, addr(0x1dc)));
extern volatile uint16_t io0x1de __attribute__((nodp, addr(0x1de)));
extern volatile uint16_t io0x1e0 __attribute__((nodp, addr(0x1e0)));
extern volatile uint16_t io0x1e2 __attribute__((nodp, addr(0x1e2)));
extern volatile uint16_t io0x1e4 __attribute__((nodp, addr(0x1e4)));
extern volatile uint16_t io0x1e6 __attribute__((nodp, addr(0x1e6)));
extern volatile uint16_t io0x1e8 __attribute__((nodp, addr(0x1e8)));
extern volatile uint16_t io0x1ea __attribute__((nodp, addr(0x1ea)));
extern volatile uint16_t io0x1ec __attribute__((nodp, addr(0x1ec)));
extern volatile uint16_t io0x1ee __attribute__((nodp, addr(0x1ee)));
extern volatile uint16_t io0x1f0 __attribute__((nodp, addr(0x1f0)));
extern volatile uint16_t io0x1f2 __attribute__((nodp, addr(0x1f2)));
extern volatile uint16_t io0x1f4 __attribute__((nodp, addr(0x1f4)));
extern volatile uint16_t io0x1f6 __attribute__((nodp, addr(0x1f6)));
extern IO_CONST volatile uint16_t io0x1f8 __attribute__((nodp, addr(0x1f8)));
extern IO_CONST volatile uint16_t io0x1fa __attribute__((nodp, addr(0x1fa)));
extern volatile uint16_t io0x1fc __attribute__((nodp, addr(0x1fc)));
extern volatile uint16_t io0x1fe __attribute__((nodp, addr(0x1fe)));
extern volatile uint16_t io0x200 __attribute__((nodp, addr(0x200)));
extern volatile uint16_t io0x202 __attribute__((nodp, addr(0x202)));
extern volatile uint16_t io0x204 __attribute__((nodp, addr(0x204)));
extern volatile uint16_t io0x206 __attribute__((nodp, addr(0x206)));
extern volatile uint16_t io0x208 __attribute__((nodp, addr(0x208)));
extern volatile uint16_t io0x20a __attribute__((nodp, addr(0x20a)));
extern volatile uint16_t io0x20c __attribute__((nodp, addr(0x20c)));
extern volatile uint16_t io0x20e __attribute__((nodp, addr(0x20e)));
extern IO_CONST volatile uint16_t io0x210 __attribute__((nodp, addr(0x210)));
extern volatile uint16_t io0x212 __attribute__((nodp, addr(0x212)));
extern volatile uint16_t io0x214 __attribute__((nodp, addr(0x214)));
extern IO_CONST volatile uint16_t io0x216 __attribute__((nodp, addr(0x216)));
extern volatile uint16_t io0x218 __attribute__((nodp, addr(0x218)));
extern volatile uint16_t io0x21a __attribute__((nodp, addr(0x21a)));
extern volatile uint16_t io0x21c __attribute__((nodp, addr(0x21c)));
extern volatile uint16_t io0x21e __attribute__((nodp, addr(0x21e)));
extern volatile uint16_t io0x220 __attribute__((nodp, addr(0x220)));
extern IO_CONST volatile uint16_t io0x222 __attribute__((nodp, addr(0x222)));
extern volatile uint16_t io0x224 __attribute__((nodp, addr(0x224)));
extern volatile uint16_t io0x226 __attribute__((nodp, addr(0x226)));
extern volatile uint16_t io0x228 __attribute__((nodp, addr(0x228)));
extern volatile uint16_t io0x22a __attribute__((nodp, addr(0x22a)));
extern volatile uint16_t io0x22c __attribute__((nodp, addr(0x22c)));
extern volatile uint16_t io0x22e __attribute__((nodp, addr(0x22e)));
extern volatile uint16_t io0x230 __attribute__((nodp, addr(0x230)));
extern volatile uint16_t io0x232 __attribute__((nodp, addr(0x232)));
extern volatile uint16_t io0x234 __attribute__((nodp, addr(0x234)));
extern volatile uint16_t io0x236 __attribute__((nodp, addr(0x236)));
extern volatile uint16_t io0x238 __attribute__((nodp, addr(0x238)));
extern volatile uint16_t io0x23a __attribute__((nodp, addr(0x23a)));
extern volatile uint16_t io0x23c __attribute__((nodp, addr(0x23c)));
extern volatile uint16_t io0x23e __attribute__((nodp, addr(0x23e)));
extern volatile uint16_t io0x240 __attribute__((nodp, addr(0x240)));
extern volatile uint16_t io0x242 __attribute__((nodp, addr(0x242)));
extern volatile uint16_t io0x244 __attribute__((nodp, addr(0x244)));
extern volatile uint16_t io0x246 __attribute__((nodp, addr(0x246)));
extern volatile uint16_t io0x248 __attribute__((nodp, addr(0x248)));
extern IO_CONST volatile uint16_t io0x24a __attribute__((nodp, addr(0x24a)));
extern volatile uint16_t io0x24c __attribute__((nodp, addr(0x24c)));
extern IO_CONST volatile uint16_t io0x24e __attribute__((nodp, addr(0x24e)));
extern volatile uint16_t io0x250 __attribute__((nodp, addr(0x250)));
extern volatile uint16_t io0x252 __attribute__((nodp, addr(0x252)));

/* Byte variables (8-bit) */

extern volatile uint8_t io_byte_0x2  __attribute__((io, addr(0x2)));
extern volatile uint8_t io_byte_0x3  __attribute__((io, addr(0x3)));
extern volatile uint8_t io_byte_0x6  __attribute__((io, addr(0x6)));
extern volatile uint8_t io_byte_0x7  __attribute__((io, addr(0x7)));
extern volatile uint8_t error_IO_FUNC_TEST__TEST_UNPROT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MUPET__PTC_RECEIVE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MUPET__PTC_SEND_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0xc  __attribute__((io, addr(0xc)));
extern volatile uint8_t io_byte_0xd  __attribute__((io, addr(0xd)));
extern volatile uint8_t io_byte_0xe  __attribute__((io, bit_access, addr(0xe)));
extern volatile uint8_t io_byte_0xf  __attribute__((io, bit_access, addr(0xf)));
extern volatile uint8_t io_byte_0x10  __attribute__((io, addr(0x10)));
extern volatile uint8_t error_IO_ROM_BIST__ADD_START_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x14  __attribute__((io, addr(0x14)));
extern volatile uint8_t error_IO_ROM_BIST__ADD_STOP_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x18  __attribute__((io, addr(0x18)));
extern volatile uint8_t error_IO_ROM_BIST__SIG_EXPECTED_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1c  __attribute__((io, addr(0x1c)));
extern volatile uint8_t io_byte_0x1e  __attribute__((io, addr(0x1e)));
extern volatile uint8_t io_byte_0x1f  __attribute__((io, addr(0x1f)));
extern volatile uint8_t error_IO_ROM_BIST__START_BIST_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_ROM_BIST__SIG_RECEIVED_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x24  __attribute__((io, addr(0x24)));
extern volatile uint8_t io_byte_0x28  __attribute__((io, addr(0x28)));
extern volatile uint8_t io_byte_0x29  __attribute__((io, addr(0x29)));
extern volatile uint8_t io_byte_0x2a  __attribute__((io, addr(0x2a)));
extern volatile uint8_t io_byte_0x2b  __attribute__((io, addr(0x2b)));
extern IO_CONST volatile uint8_t error_IO_STIMER__CURRENT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_STIMER__VALUE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_STIMER__MODE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CTIMER0__TREGB_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CTIMER0__TREGA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_CTIMER0__TCNT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x36  __attribute__((io, addr(0x36)));
extern volatile uint8_t io_byte_0x37  __attribute__((io, addr(0x37)));
extern volatile uint8_t error_IO_MLX16__DBG_DATA0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__DBG_DATA1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__DBG_DATA2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__DBG_DATA3_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__DBG_ADDRESS0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x42  __attribute__((nodp, addr(0x42)));
extern volatile uint8_t error_IO_MLX16__DBG_CONTROL0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x43  __attribute__((nodp, addr(0x43)));
extern volatile uint8_t error_IO_MLX16__DBG_ADDRESS1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x46  __attribute__((nodp, addr(0x46)));
extern volatile uint8_t error_IO_MLX16__DBG_CONTROL1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x47  __attribute__((nodp, addr(0x47)));
extern volatile uint8_t error_IO_MLX16__DBG_ADDRESS2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x4a  __attribute__((nodp, addr(0x4a)));
extern volatile uint8_t error_IO_MLX16__DBG_CONTROL2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x4b  __attribute__((nodp, addr(0x4b)));
extern volatile uint8_t error_IO_MLX16__DBG_ADDRESS3_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x4e  __attribute__((nodp, addr(0x4e)));
extern volatile uint8_t error_IO_MLX16__DBG_CONTROL3_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x4f  __attribute__((nodp, addr(0x4f)));
extern volatile uint8_t error_IO_MLX16__ITC_PEND0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x50  __attribute__((nodp, addr(0x50)));
extern volatile uint8_t io_byte_0x51  __attribute__((nodp, addr(0x51)));
extern volatile uint8_t error_IO_MLX16__ITC_PEND1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x52  __attribute__((nodp, addr(0x52)));
extern volatile uint8_t io_byte_0x53  __attribute__((nodp, addr(0x53)));
extern volatile uint8_t error_IO_MLX16__ITC_PEND2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x54  __attribute__((nodp, addr(0x54)));
extern volatile uint8_t io_byte_0x55  __attribute__((nodp, addr(0x55)));
extern volatile uint8_t io_byte_0x6b  __attribute__((nodp, addr(0x6b)));
extern volatile uint8_t error_IO_MLX16__ITC_MASK0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x6c  __attribute__((nodp, addr(0x6c)));
extern volatile uint8_t io_byte_0x6d  __attribute__((nodp, addr(0x6d)));
extern volatile uint8_t error_IO_MLX16__ITC_MASK1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x6e  __attribute__((nodp, addr(0x6e)));
extern volatile uint8_t io_byte_0x6f  __attribute__((nodp, addr(0x6f)));
extern volatile uint8_t error_IO_MLX16__ITC_MASK2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x70  __attribute__((nodp, addr(0x70)));
extern volatile uint8_t io_byte_0x71  __attribute__((nodp, addr(0x71)));
extern volatile uint8_t error_IO_MLX16__ITC_PRIO0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x88  __attribute__((nodp, addr(0x88)));
extern volatile uint8_t io_byte_0x89  __attribute__((nodp, addr(0x89)));
extern volatile uint8_t error_IO_MLX16__ITC_PRIO1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x8a  __attribute__((nodp, addr(0x8a)));
extern volatile uint8_t io_byte_0x8b  __attribute__((nodp, addr(0x8b)));
extern volatile uint8_t error_IO_MLX16__ITC_PRIO2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x8c  __attribute__((nodp, addr(0x8c)));
extern volatile uint8_t io_byte_0x8d  __attribute__((nodp, addr(0x8d)));
extern volatile uint8_t error_IO_MLX16__ITC_PRIO3_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x8e  __attribute__((nodp, addr(0x8e)));
extern volatile uint8_t io_byte_0x8f  __attribute__((nodp, addr(0x8f)));
extern volatile uint8_t error_IO_MLX16__ITC_PRIO4_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x90  __attribute__((nodp, addr(0x90)));
extern volatile uint8_t io_byte_0x91  __attribute__((nodp, addr(0x91)));
extern IO_CONST volatile uint8_t error_IO_MLX16__SHELL_VERSION_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_MLX16__CPU_FP0ADR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_VERSION__VERSION_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_VERSION__VERSION_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__KEY_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__PHASE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__TRANSPARENT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__REGULAR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__RUNNING_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__VALID_CLOCK_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__REGULAR_BIST_ERROR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__COMPLETED_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__LFSR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__LSFR_GOT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__LFSR_EXPECTED_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__ADL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_RAM_BIST__ADH_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_RAM_BIST__ADD_START_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x110  __attribute__((nodp, addr(0x110)));
extern volatile uint8_t error_IO_RAM_BIST__ADD_STOP_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x114  __attribute__((nodp, addr(0x114)));
extern volatile uint8_t io_byte_0x116  __attribute__((nodp, addr(0x116)));
extern volatile uint8_t io_byte_0x117  __attribute__((nodp, addr(0x117)));
extern volatile uint8_t error_IO_CTIMER1__TREGB_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_CTIMER1__TREGA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_CTIMER1__TCNT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x11e  __attribute__((nodp, addr(0x11e)));
extern volatile uint8_t io_byte_0x11f  __attribute__((nodp, addr(0x11f)));
extern volatile uint8_t error_IO_PWM_MASTER1__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_MASTER1__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_MASTER1__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_MASTER1__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x128  __attribute__((nodp, addr(0x128)));
extern volatile uint8_t io_byte_0x129  __attribute__((nodp, addr(0x129)));
extern volatile uint8_t error_IO_PWM_SLAVE1__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE1__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE1__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE1__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x132  __attribute__((nodp, addr(0x132)));
extern volatile uint8_t io_byte_0x133  __attribute__((nodp, addr(0x133)));
extern volatile uint8_t error_IO_PWM_SLAVE2__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE2__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE2__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE2__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x13c  __attribute__((nodp, addr(0x13c)));
extern volatile uint8_t io_byte_0x13d  __attribute__((nodp, addr(0x13d)));
extern volatile uint8_t error_IO_PWM_SLAVE3__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE3__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE3__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE3__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x146  __attribute__((nodp, addr(0x146)));
extern volatile uint8_t io_byte_0x147  __attribute__((nodp, addr(0x147)));
extern volatile uint8_t error_IO_PWM_SLAVE4__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE4__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE4__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE4__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x150  __attribute__((nodp, addr(0x150)));
extern volatile uint8_t io_byte_0x151  __attribute__((nodp, addr(0x151)));
extern volatile uint8_t error_IO_PWM_SLAVE5__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE5__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE5__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE5__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x15a  __attribute__((nodp, addr(0x15a)));
extern volatile uint8_t io_byte_0x15b  __attribute__((nodp, addr(0x15b)));
extern volatile uint8_t error_IO_PWM_SLAVE6__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE6__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE6__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE6__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x164  __attribute__((nodp, addr(0x164)));
extern volatile uint8_t io_byte_0x165  __attribute__((nodp, addr(0x165)));
extern volatile uint8_t error_IO_PWM_SLAVE7__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE7__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE7__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE7__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x16e  __attribute__((nodp, addr(0x16e)));
extern volatile uint8_t io_byte_0x16f  __attribute__((nodp, addr(0x16f)));
extern volatile uint8_t error_IO_PWM_SLAVE8__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE8__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE8__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE8__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x178  __attribute__((nodp, addr(0x178)));
extern volatile uint8_t io_byte_0x179  __attribute__((nodp, addr(0x179)));
extern volatile uint8_t error_IO_PWM_SLAVE9__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE9__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE9__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE9__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x182  __attribute__((nodp, addr(0x182)));
extern volatile uint8_t io_byte_0x183  __attribute__((nodp, addr(0x183)));
extern volatile uint8_t error_IO_PWM_SLAVE10__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE10__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE10__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE10__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x18c  __attribute__((nodp, addr(0x18c)));
extern volatile uint8_t io_byte_0x18d  __attribute__((nodp, addr(0x18d)));
extern volatile uint8_t error_IO_PWM_SLAVE11__CMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE11__HT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PWM_SLAVE11__LT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PWM_SLAVE11__PER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x196  __attribute__((nodp, addr(0x196)));
extern volatile uint8_t io_byte_0x197  __attribute__((nodp, addr(0x197)));
extern volatile uint8_t io_byte_0x198  __attribute__((nodp, addr(0x198)));
extern volatile uint8_t io_byte_0x199  __attribute__((nodp, addr(0x199)));
extern volatile uint8_t error_IO_ADC_SAR__SBASE_0_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x19c  __attribute__((nodp, addr(0x19c)));
extern volatile uint8_t io_byte_0x19d  __attribute__((nodp, addr(0x19d)));
extern volatile uint8_t io_byte_0x19e  __attribute__((nodp, addr(0x19e)));
extern volatile uint8_t io_byte_0x1a0  __attribute__((nodp, addr(0x1a0)));
extern volatile uint8_t error_IO_ADC_SAR__ADC_DATA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1a3  __attribute__((nodp, addr(0x1a3)));
extern volatile uint8_t io_byte_0x1a4  __attribute__((nodp, addr(0x1a4)));
extern volatile uint8_t error_IO_ADC_SAR__TR_ADCREF2_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1a6  __attribute__((nodp, addr(0x1a6)));
extern volatile uint8_t io_byte_0x1a8  __attribute__((nodp, addr(0x1a8)));
extern volatile uint8_t io_byte_0x1aa  __attribute__((nodp, addr(0x1aa)));
extern volatile uint8_t error_IO_PORT_TEST_ADC__TEST_ADC_IN_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_TEST_ADC__TEST_ADC_SPARE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1ac  __attribute__((nodp, addr(0x1ac)));
extern volatile uint8_t error_IO_EEPROM_FLASH__T_10US_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1ad  __attribute__((nodp, addr(0x1ad)));
extern volatile uint8_t io_byte_0x1ae  __attribute__((nodp, addr(0x1ae)));
extern volatile uint8_t io_byte_0x1af  __attribute__((nodp, addr(0x1af)));
extern volatile uint8_t io_byte_0x1b0  __attribute__((nodp, addr(0x1b0)));
extern volatile uint8_t io_byte_0x1b1  __attribute__((nodp, addr(0x1b1)));
extern volatile uint8_t error_IO_EEPROM_FLASH__FL_COMMAND_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1b2  __attribute__((nodp, addr(0x1b2)));
extern volatile uint8_t io_byte_0x1b4  __attribute__((nodp, addr(0x1b4)));
extern volatile uint8_t io_byte_0x1b5  __attribute__((nodp, addr(0x1b5)));
extern volatile uint8_t io_byte_0x1b6  __attribute__((nodp, addr(0x1b6)));
extern volatile uint8_t io_byte_0x1b7  __attribute__((nodp, addr(0x1b7)));
extern volatile uint8_t io_byte_0x1b8  __attribute__((nodp, addr(0x1b8)));
extern volatile uint8_t io_byte_0x1b9  __attribute__((nodp, addr(0x1b9)));
extern volatile uint8_t error_IO_EEPROM_FLASH__FL_PAGE_NUMBER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_EEPROM_FLASH__FL_SECTOR_NUMBER_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_EEPROM_FLASH__FL_CS_AREA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1bc  __attribute__((nodp, addr(0x1bc)));
extern volatile uint8_t io_byte_0x1bd  __attribute__((nodp, addr(0x1bd)));
extern volatile uint8_t io_byte_0x1be  __attribute__((nodp, addr(0x1be)));
extern volatile uint8_t io_byte_0x1c0  __attribute__((nodp, addr(0x1c0)));
extern volatile uint8_t io_byte_0x1c1  __attribute__((nodp, addr(0x1c1)));
extern volatile uint8_t io_byte_0x1c2  __attribute__((nodp, addr(0x1c2)));
extern volatile uint8_t io_byte_0x1c3  __attribute__((nodp, addr(0x1c3)));
extern volatile uint8_t io_byte_0x1c4  __attribute__((nodp, addr(0x1c4)));
extern volatile uint8_t io_byte_0x1c5  __attribute__((nodp, addr(0x1c5)));
extern volatile uint8_t io_byte_0x1c6  __attribute__((nodp, addr(0x1c6)));
extern volatile uint8_t io_byte_0x1c7  __attribute__((nodp, addr(0x1c7)));
extern volatile uint8_t io_byte_0x1c8  __attribute__((nodp, addr(0x1c8)));
extern volatile uint8_t io_byte_0x1d2  __attribute__((nodp, addr(0x1d2)));
extern volatile uint8_t io_byte_0x1d3  __attribute__((nodp, addr(0x1d3)));
extern volatile uint8_t error_IO_EEPROM_FLASH__FL_BITS_ERRORS_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_EEPROM_FLASH__FL_SIG_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_EEPROM_FLASH__FL_SIG_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1da  __attribute__((nodp, addr(0x1da)));
extern volatile uint8_t io_byte_0x1db  __attribute__((nodp, addr(0x1db)));
extern volatile uint8_t error_IO_EEPROM_FLASH__EE_BITS_ERRORS_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_EEPROM_FLASH__EE_SIG_L_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_EEPROM_FLASH__EE_SIG_H_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_LIN_XKEY__LIN_XKEY_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1e4  __attribute__((nodp, addr(0x1e4)));
extern volatile uint8_t error_IO_TRIM_BG_BIAS__PRE_TR_BIAS_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_TRIM_BG_BIAS__TR_UNUSED_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1e5  __attribute__((nodp, addr(0x1e5)));
extern volatile uint8_t io_byte_0x1e6  __attribute__((nodp, addr(0x1e6)));
extern volatile uint8_t error_IO_TRIM_VDD__PRE_TR_SUP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_TRIM_VDD__TR_UNUSED_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1e7  __attribute__((nodp, addr(0x1e7)));
extern volatile uint8_t error_IO_TRIM_RCO32M__TR_RCO32M_IN_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_TRIM_RCO32M__TR_UNUSED_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1e9  __attribute__((nodp, addr(0x1e9)));
extern volatile uint8_t io_byte_0x1ea  __attribute__((nodp, addr(0x1ea)));
extern volatile uint8_t io_byte_0x1eb  __attribute__((nodp, addr(0x1eb)));
extern volatile uint8_t io_byte_0x1ec  __attribute__((nodp, addr(0x1ec)));
extern volatile uint8_t io_byte_0x1ee  __attribute__((nodp, addr(0x1ee)));
extern volatile uint8_t error_IO_PORT_STEP_CONF__STEP_CNT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1f0  __attribute__((nodp, addr(0x1f0)));
extern volatile uint8_t error_IO_PORT_SUPP_TEST__SWITCHOFFREG_VDDD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP_TEST__SWITCHOFFUV_VDDD_RES_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP_TEST__SBY_BIAS_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP_TEST__LOW_VDDD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP_TEST__SHOVE_VDDD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP_TEST__SHOVE_VDDA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP_TEST__SHOVE_VAUX_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP_TEST__DISABLE_RAM_ROM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1f2  __attribute__((nodp, addr(0x1f2)));
extern volatile uint8_t error_IO_PORT_SUPP2_TEST__TEST_MEM_ANA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP2_TEST__TM_35DEGC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP2_TEST__TM_TSDCMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP2_TEST__TM_VGB_O_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP2_TEST__IDDQ_TEMPSENSE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP2_TEST__IDDQ_CLK10K_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP2_TEST__FGTSM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SUPP2_TEST__FSTOP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1f4  __attribute__((nodp, addr(0x1f4)));
extern volatile uint8_t io_byte_0x1f6  __attribute__((nodp, addr(0x1f6)));
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__UV_VDDA_IT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__UV_VDDA_SYNC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__UV_VS_IT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__UV_VS_SYNC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__OV_VS_IT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__OV_VS_SYNC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__OVT_IT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__OVT_SYNC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__OVT_WARN_IT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__OVT_WARN_SYNC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__UV_VDD5_IT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_SUPP_IN__UV_VDD5_SYNC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MISC_IN__AIN_SUP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MISC_IN__STOP_MODE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MISC_IN__RSTAT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MISC_IN__INTERNAL_WU_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MISC_IN__LIN_WU_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MISC_IN__PHYSTAT1_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MISC_IN__AAOUT_IN_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x1fc  __attribute__((nodp, addr(0x1fc)));
extern volatile uint8_t io_byte_0x1fe  __attribute__((nodp, addr(0x1fe)));
extern volatile uint8_t error_IO_PORT_TIMER_CFG__TIMER1_CHA_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_TIMER_CFG__TIMER1_CHB_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x200  __attribute__((nodp, addr(0x200)));
extern volatile uint8_t error_IO_PORT_MISC_OUT__PRUV_VS_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_MISC_OUT__PROV_VS_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_MISC_OUT__PRUV_VDDA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_MISC_OUT__SEL_TEMP_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x202  __attribute__((nodp, addr(0x202)));
extern volatile uint8_t error_IO_PORT_MISC2_OUT__OVT_DISABLE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_MISC2_OUT__SWITCHOFFUV_VDD5_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_MISC2_OUT__ENABLE_TX_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_MISC2_OUT__EN_IN_AAIN_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x204  __attribute__((nodp, addr(0x204)));
extern volatile uint8_t io_byte_0x206  __attribute__((nodp, addr(0x206)));
extern volatile uint8_t error_IO_PORT_STOPMD_CFG__STOPMD_CFG_ANA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x208  __attribute__((nodp, addr(0x208)));
extern volatile uint8_t io_byte_0x20a  __attribute__((nodp, addr(0x20a)));
extern volatile uint8_t error_IO_UART__BRRD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__TRD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__RRD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__LFCD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__LFDD_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__TRE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__REE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__ISB_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__LBE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__LSE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__LTE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__MLS_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__BSC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__LBC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__LSC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_UART__LDC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__TRO_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__TRB_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__TSB_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__RSO_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__RSB_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__RRF_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__NBR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_UART__SBE_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x218  __attribute__((nodp, addr(0x218)));
extern volatile uint8_t error_IO_PORT_ID__IDMASK_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_DMA_RDA__DMA_RDA_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_DMA_RDB__DMA_RDB_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_DMA_TX__DMA_TX_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x220  __attribute__((nodp, addr(0x220)));
extern IO_CONST volatile uint8_t error_IO_PORT_MELIBUS_STAT__RD_BUFFER_VALID_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MELIBUS_STAT__FRAME_RECEIVED_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MELIBUS_STAT__FRAME_TRANSMIT_RDY_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MELIBUS_STAT__PARITY_ERROR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MELIBUS_STAT__CRC_ERROR_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MELIBUS_STAT__WRONG_BREAK_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MELIBUS_STAT__BREAK_IN_FRAME_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern IO_CONST volatile uint8_t error_IO_PORT_MELIBUS_STAT__TX_TIMEOUT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x224  __attribute__((nodp, addr(0x224)));
extern volatile uint8_t error_IO_PORT_LC_SEL1__LC2_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_LC_SEL1__LC3_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x226  __attribute__((nodp, addr(0x226)));
extern volatile uint8_t error_IO_PORT_LC_SEL2__LC6_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_LC_SEL2__LC7_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x228  __attribute__((nodp, addr(0x228)));
extern volatile uint8_t error_IO_PORT_LC_SEL3__LC10_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_LC_SEL3__LC11_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x22a  __attribute__((nodp, addr(0x22a)));
extern volatile uint8_t error_IO_TRIM1_LC__LC1_TRIM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x22b  __attribute__((nodp, addr(0x22b)));
extern volatile uint8_t io_byte_0x22c  __attribute__((nodp, addr(0x22c)));
extern volatile uint8_t error_IO_TRIM2_LC__LC3_TRIM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x22d  __attribute__((nodp, addr(0x22d)));
extern volatile uint8_t io_byte_0x22e  __attribute__((nodp, addr(0x22e)));
extern volatile uint8_t error_IO_TRIM3_LC__LC5_TRIM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x22f  __attribute__((nodp, addr(0x22f)));
extern volatile uint8_t io_byte_0x230  __attribute__((nodp, addr(0x230)));
extern volatile uint8_t error_IO_TRIM4_LC__LC7_TRIM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x231  __attribute__((nodp, addr(0x231)));
extern volatile uint8_t io_byte_0x232  __attribute__((nodp, addr(0x232)));
extern volatile uint8_t error_IO_TRIM5_LC__LC9_TRIM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x233  __attribute__((nodp, addr(0x233)));
extern volatile uint8_t io_byte_0x234  __attribute__((nodp, addr(0x234)));
extern volatile uint8_t error_IO_TRIM6_LC__LC11_TRIM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x235  __attribute__((nodp, addr(0x235)));
extern volatile uint8_t error_IO_PORT_LC_DISPU_DIAG__LC_DISPU_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_LC_DISPU_DIAG__LCDIAG_SEL_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_LC_OUT__LC_OUT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_PORT_SELP_LC__SELP_LC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x23c  __attribute__((nodp, addr(0x23c)));
extern volatile uint8_t io_byte_0x23e  __attribute__((nodp, addr(0x23e)));
extern volatile uint8_t io_byte_0x240  __attribute__((nodp, addr(0x240)));
extern volatile uint8_t error_IO_TRIM_MISC__LCDIAG_TRIM_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t error_IO_TRIM_MISC__TR_OVT_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x241  __attribute__((nodp, addr(0x241)));
extern volatile uint8_t io_byte_0x242  __attribute__((nodp, addr(0x242)));
extern volatile uint8_t io_byte_0x244  __attribute__((nodp, addr(0x244)));
extern volatile uint8_t io_byte_0x246  __attribute__((nodp, addr(0x246)));
extern volatile uint8_t io_byte_0x248  __attribute__((nodp, addr(0x248)));
extern IO_CONST volatile uint8_t error_IO_PORT_AA_IN__AA_IN_SYNC_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x24c  __attribute__((nodp, addr(0x24c)));
extern IO_CONST volatile uint8_t error_IO_PORT_LIN_XCFG_VALID__LIN_XCFG_VALID_has_no_byte_access; /* to catch attempts of byte access when not supported */
extern volatile uint8_t io_byte_0x250  __attribute__((nodp, addr(0x250)));
extern volatile uint8_t io_byte_0x251  __attribute__((nodp, addr(0x251)));
extern volatile uint8_t io_byte_0x252  __attribute__((nodp, addr(0x252)));


/*****************************************************************************/
/* 3.2 Get macros. */

#define IO_RST_CTRL__AWD_WBOOT_GET() \
	((io0x2 & 0x0001u) >> 0U)
#define IO_RST_CTRL__SOFT_WBOOT_GET() \
	((io0x2 & 0x0002u) >> 1U)
#define IO_RST_CTRL__HVDIG_WBOOT_GET() \
	((io0x2 & 0x0004u) >> 2U)
#define IO_RST_CTRL__DBG_WBOOT_GET() \
	((io0x2 & 0x0008u) >> 3U)
#define IO_RST_CTRL__IWD_WBOOT_GET() \
	((io0x2 & 0x0010u) >> 4U)
#define IO_RST_CTRL__SOFT_RESET_GET() \
	((io0x2 & 0x2000u) >> 13U)
#define IO_RST_CTRL__HVDIG_USED_GET() \
	((io0x2 & 0x4000u) >> 14U)
#define IO_RST_CTRL__HVDIG_OK_GET() \
	((io0x2 & 0x8000u) >> 15U)
#define IO_FUNC_TEST__DISABLE_RESET_GET() \
	((io0x6 & 0x0001u) >> 0U)
#define IO_FUNC_TEST__DISABLE_SUSPEND_GET() \
	((io0x6 & 0x0002u) >> 1U)
#define IO_FUNC_TEST__DISABLE_IRQ_GET() \
	((io0x6 & 0x0004u) >> 2U)
#define IO_FUNC_TEST__FTC_SEL_GET() \
	((io0x6 & 0x0300u) >> 8U)
#define IO_FUNC_TEST__FTC_DIV_GET() \
	((io0x6 & 0x3c00u) >> 10U)
#define IO_FUNC_TEST__TEST_UNPROT_GET() \
	(io0x8 >> 0U)
#define IO_MUPET__PTC_RECEIVE_GET() \
	((io0xa & 0xffffu) >> 0U)
#define IO_MUPET__PTC_SEND_GET() \
	((io0xa & 0xffffu) >> 0U)
#define IO_MUPET__PTCA_PEND_GET() \
	((io0xc & 0x0001u) >> 0U)
#define IO_MUPET__PTCS_PEND_GET() \
	((io0xc & 0x0002u) >> 1U)
#define IO_MUPET__PTCR_PEND_GET() \
	((io0xc & 0x0004u) >> 2U)
#define IO_MUPET__PTC_KEY_GET() \
	((io0xc & 0x0080u) >> 7U)
#define IO_MUPET__WARM_TRIGGER_GET() \
	((io0xc & 0x1f00u) >> 8U)
#define IO_MUPET__CONNECTION_GET() \
	((io0xc & 0x6000u) >> 13U)
#define IO_MUPET__IN_APPLICATION_GET() \
	((io0xc & 0x8000u) >> 15U)
static __attribute__((always_inline)) inline uint16_t IO_AWD__ACK_GET (void) {
	register uint16_t result = 0u;
	asm volatile (
		"mov c, io:0xe.6\n"
		"adc %[acc], #0" : [acc]"=r"(result) : "0"(result));
	return result;
}
static __attribute__((always_inline)) inline uint16_t IO_AWD__WIN_OPEN_GET (void) {
	register uint16_t result = 0u;
	asm volatile (
		"mov c, io:0xf.7\n"
		"adc %[acc], #0" : [acc]"=r"(result) : "0"(result));
	return result;
}
#define IO_ROM_SHELL__READY_GET() \
	(io0x10 >> 0U)
#define IO_ROM_BIST__ADD_START_L_GET() \
	(io0x12 >> 0U)
#define IO_ROM_BIST__ADD_START_H_GET() \
	(io0x14 >> 0U)
#define IO_ROM_BIST__ADD_STOP_L_GET() \
	(io0x16 >> 0U)
#define IO_ROM_BIST__ADD_STOP_H_GET() \
	(io0x18 >> 0U)
#define IO_ROM_BIST__SIG_EXPECTED_L_GET() \
	(io0x1a >> 0U)
#define IO_ROM_BIST__SIG_EXPECTED_H_GET() \
	(io0x1c >> 0U)
#define IO_ROM_BIST__ECC_POSITION_GET() \
	((io0x1e & 0x0003u) >> 0U)
#define IO_ROM_BIST__BIST_GET() \
	((io0x1e & 0x0010u) >> 4U)
#define IO_ROM_BIST__SINGLE_RAMP_GET() \
	((io0x1e & 0x0020u) >> 5U)
#define IO_ROM_BIST__MASK_SIG_ERR_GET() \
	((io0x1e & 0x0040u) >> 6U)
#define IO_ROM_BIST__BIST_REQUEST_GET() \
	((io0x1e & 0x0200u) >> 9U)
#define IO_ROM_BIST__VALID_CLOCK_GET() \
	((io0x1e & 0x0800u) >> 11U)
#define IO_ROM_BIST__COMPLETED_GET() \
	((io0x1e & 0x8000u) >> 15U)
#define IO_ROM_BIST__START_BIST_GET() \
	(io0x20 >> 0U)
#define IO_ROM_BIST__SIG_RECEIVED_L_GET() \
	(io0x22 >> 0U)
#define IO_ROM_BIST__SIG_RECEIVED_H_GET() \
	(io0x24 >> 0U)
#define IO_IWD__WDT_GET() \
	((io0x28 & 0x00ffu) >> 0U)
#define IO_IWD__DIV_GET() \
	((io0x28 & 0x0700u) >> 8U)
#define IO_IWD__WTG_GET() \
	((io0x2a & 0x00ffu) >> 0U)
#define IO_IWD__WIN_ENABLE_GET() \
	((io0x2a & 0x1000u) >> 12U)
#define IO_IWD__WIN_DISABLE_GET() \
	((io0x2a & 0x2000u) >> 13U)
#define IO_IWD__ATT_INT_GET() \
	((io0x2a & 0x4000u) >> 14U)
#define IO_IWD__WIN_OPEN_GET() \
	((io0x2a & 0x8000u) >> 15U)
#define IO_STIMER__CURRENT_GET() \
	(io0x2c >> 0U)
#define IO_STIMER__VALUE_GET() \
	((io0x2e & 0x3fffu) >> 0U)
#define IO_STIMER__MODE_GET() \
	((io0x2e & 0xc000u) >> 14U)
#define IO_CTIMER0__TREGB_GET() \
	(io0x30 >> 0U)
#define IO_CTIMER0__TREGA_GET() \
	(io0x32 >> 0U)
#define IO_CTIMER0__TCNT_GET() \
	(io0x34 >> 0U)
#define IO_CTIMER0__START_GET() \
	((io0x36 & 0x0001u) >> 0U)
#define IO_CTIMER0__STOP_GET() \
	((io0x36 & 0x0002u) >> 1U)
#define IO_CTIMER0__EDGA_GET() \
	((io0x36 & 0x000cu) >> 2U)
#define IO_CTIMER0__EDGB_GET() \
	((io0x36 & 0x0030u) >> 4U)
#define IO_CTIMER0__PWMI_GET() \
	((io0x36 & 0x0040u) >> 6U)
#define IO_CTIMER0__POL_GET() \
	((io0x36 & 0x0080u) >> 7U)
#define IO_CTIMER0__OVRA_GET() \
	((io0x36 & 0x0100u) >> 8U)
#define IO_CTIMER0__OVRB_GET() \
	((io0x36 & 0x0200u) >> 9U)
#define IO_CTIMER0__ENCMP_GET() \
	((io0x36 & 0x0400u) >> 10U)
#define IO_CTIMER0__MODE_GET() \
	((io0x36 & 0x3800u) >> 11U)
#define IO_CTIMER0__DIV_GET() \
	((io0x36 & 0xc000u) >> 14U)
#define IO_MLX16__DBG_DATA0_GET() \
	(io0x38 >> 0U)
#define IO_MLX16__DBG_DATA1_GET() \
	(io0x3a >> 0U)
#define IO_MLX16__DBG_DATA2_GET() \
	(io0x3c >> 0U)
#define IO_MLX16__DBG_DATA3_GET() \
	(io0x3e >> 0U)
#define IO_MLX16__DBG_ADDRESS0_GET() \
	(io0x40 >> 0U)
#define IO_MLX16__DBG_ADREXT0_GET() \
	((io0x42 & 0x000fu) >> 0U)
#define IO_MLX16__DBG_CONTROL0_GET() \
	((io0x42 & 0xffffu) >> 0U)
#define IO_MLX16__DBG_COND0_GET() \
	((io0x42 & 0x00f0u) >> 4U)
#define IO_MLX16__DBG_RANGE0_GET() \
	((io0x42 & 0x0100u) >> 8U)
#define IO_MLX16__DBG_MODE0_GET() \
	((io0x42 & 0x0600u) >> 9U)
#define IO_MLX16__DBG_LOCK0_GET() \
	((io0x42 & 0x0800u) >> 11U)
#define IO_MLX16__DBG_MARKEXT0_GET() \
	((io0x42 & 0x1000u) >> 12U)
#define IO_MLX16__DBG_OVR0_GET() \
	((io0x42 & 0x4000u) >> 14U)
#define IO_MLX16__DBG_READY0_GET() \
	((io0x42 & 0x8000u) >> 15U)
#define IO_MLX16__DBG_ADDRESS1_GET() \
	(io0x44 >> 0U)
#define IO_MLX16__DBG_ADREXT1_GET() \
	((io0x46 & 0x000fu) >> 0U)
#define IO_MLX16__DBG_CONTROL1_GET() \
	((io0x46 & 0xffffu) >> 0U)
#define IO_MLX16__DBG_COND1_GET() \
	((io0x46 & 0x00f0u) >> 4U)
#define IO_MLX16__DBG_RANGE1_GET() \
	((io0x46 & 0x0100u) >> 8U)
#define IO_MLX16__DBG_MODE1_GET() \
	((io0x46 & 0x0600u) >> 9U)
#define IO_MLX16__DBG_LOCK1_GET() \
	((io0x46 & 0x0800u) >> 11U)
#define IO_MLX16__DBG_MARKEXT1_GET() \
	((io0x46 & 0x1000u) >> 12U)
#define IO_MLX16__DBG_OVR1_GET() \
	((io0x46 & 0x4000u) >> 14U)
#define IO_MLX16__DBG_READY1_GET() \
	((io0x46 & 0x8000u) >> 15U)
#define IO_MLX16__DBG_ADDRESS2_GET() \
	(io0x48 >> 0U)
#define IO_MLX16__DBG_ADREXT2_GET() \
	((io0x4a & 0x000fu) >> 0U)
#define IO_MLX16__DBG_CONTROL2_GET() \
	((io0x4a & 0xffffu) >> 0U)
#define IO_MLX16__DBG_COND2_GET() \
	((io0x4a & 0x00f0u) >> 4U)
#define IO_MLX16__DBG_RANGE2_GET() \
	((io0x4a & 0x0100u) >> 8U)
#define IO_MLX16__DBG_MODE2_GET() \
	((io0x4a & 0x0600u) >> 9U)
#define IO_MLX16__DBG_LOCK2_GET() \
	((io0x4a & 0x0800u) >> 11U)
#define IO_MLX16__DBG_MARKEXT2_GET() \
	((io0x4a & 0x1000u) >> 12U)
#define IO_MLX16__DBG_OVR2_GET() \
	((io0x4a & 0x4000u) >> 14U)
#define IO_MLX16__DBG_READY2_GET() \
	((io0x4a & 0x8000u) >> 15U)
#define IO_MLX16__DBG_ADDRESS3_GET() \
	(io0x4c >> 0U)
#define IO_MLX16__DBG_ADREXT3_GET() \
	((io0x4e & 0x000fu) >> 0U)
#define IO_MLX16__DBG_CONTROL3_GET() \
	((io0x4e & 0xffffu) >> 0U)
#define IO_MLX16__DBG_COND3_GET() \
	((io0x4e & 0x00f0u) >> 4U)
#define IO_MLX16__DBG_RANGE3_GET() \
	((io0x4e & 0x0100u) >> 8U)
#define IO_MLX16__DBG_MODE3_GET() \
	((io0x4e & 0x0600u) >> 9U)
#define IO_MLX16__DBG_LOCK3_GET() \
	((io0x4e & 0x0800u) >> 11U)
#define IO_MLX16__DBG_MARKEXT3_GET() \
	((io0x4e & 0x1000u) >> 12U)
#define IO_MLX16__DBG_OVR3_GET() \
	((io0x4e & 0x4000u) >> 14U)
#define IO_MLX16__DBG_READY3_GET() \
	((io0x4e & 0x8000u) >> 15U)
#define IO_MLX16__ITC_PEND0_GET() \
	((io0x50 & 0xffffu) >> 0U)
#define IO_MLX16__MLX16_EXCHG_PEND_GET() \
	((io0x50 & 0x0001u) >> 0U)
#define IO_MLX16__MLX16_DMAERR_PEND_GET() \
	((io0x50 & 0x0002u) >> 1U)
#define IO_MLX16__AWD_ATT_PEND_GET() \
	((io0x50 & 0x0004u) >> 2U)
#define IO_MLX16__IWD_ATT_PEND_GET() \
	((io0x50 & 0x0008u) >> 3U)
#define IO_MLX16__EE_ECC_PEND_GET() \
	((io0x50 & 0x0010u) >> 4U)
#define IO_MLX16__FL_ECC_PEND_GET() \
	((io0x50 & 0x0020u) >> 5U)
#define IO_MLX16__UV_VDDA_PEND_GET() \
	((io0x50 & 0x0040u) >> 6U)
#define IO_MLX16__UV_VS_PEND_GET() \
	((io0x50 & 0x0080u) >> 7U)
#define IO_MLX16__OVT_PEND_GET() \
	((io0x50 & 0x0100u) >> 8U)
#define IO_MLX16__STIMER_PEND_GET() \
	((io0x50 & 0x0200u) >> 9U)
#define IO_MLX16__CTIMER0_1_PEND_GET() \
	((io0x50 & 0x0400u) >> 10U)
#define IO_MLX16__CTIMER0_2_PEND_GET() \
	((io0x50 & 0x0800u) >> 11U)
#define IO_MLX16__CTIMER0_3_PEND_GET() \
	((io0x50 & 0x1000u) >> 12U)
#define IO_MLX16__CTIMER1_1_PEND_GET() \
	((io0x50 & 0x2000u) >> 13U)
#define IO_MLX16__CTIMER1_2_PEND_GET() \
	((io0x50 & 0x4000u) >> 14U)
#define IO_MLX16__CTIMER1_3_PEND_GET() \
	((io0x50 & 0x8000u) >> 15U)
#define IO_MLX16__ITC_PEND1_GET() \
	((io0x52 & 0xffffu) >> 0U)
#define IO_MLX16__PWM_MASTER1_CMP_PEND_GET() \
	((io0x52 & 0x0001u) >> 0U)
#define IO_MLX16__PWM_MASTER1_END_PEND_GET() \
	((io0x52 & 0x0002u) >> 1U)
#define IO_MLX16__PWM_SLAVE1_CMP_PEND_GET() \
	((io0x52 & 0x0004u) >> 2U)
#define IO_MLX16__PWM_SLAVE2_CMP_PEND_GET() \
	((io0x52 & 0x0008u) >> 3U)
#define IO_MLX16__PWM_SLAVE3_CMP_PEND_GET() \
	((io0x52 & 0x0010u) >> 4U)
#define IO_MLX16__PWM_SLAVE4_CMP_PEND_GET() \
	((io0x52 & 0x0020u) >> 5U)
#define IO_MLX16__PWM_SLAVE5_CMP_PEND_GET() \
	((io0x52 & 0x0040u) >> 6U)
#define IO_MLX16__PWM_SLAVE6_CMP_PEND_GET() \
	((io0x52 & 0x0080u) >> 7U)
#define IO_MLX16__PWM_SLAVE7_CMP_PEND_GET() \
	((io0x52 & 0x0100u) >> 8U)
#define IO_MLX16__PWM_SLAVE8_CMP_PEND_GET() \
	((io0x52 & 0x0200u) >> 9U)
#define IO_MLX16__PWM_SLAVE9_CMP_PEND_GET() \
	((io0x52 & 0x0400u) >> 10U)
#define IO_MLX16__PWM_SLAVE10_CMP_PEND_GET() \
	((io0x52 & 0x0800u) >> 11U)
#define IO_MLX16__PWM_SLAVE11_CMP_PEND_GET() \
	((io0x52 & 0x1000u) >> 12U)
#define IO_MLX16__ADC_SAR_PEND_GET() \
	((io0x52 & 0x2000u) >> 13U)
#define IO_MLX16__EE_COMPLETE_PEND_GET() \
	((io0x52 & 0x4000u) >> 14U)
#define IO_MLX16__FL_COMPLETE_PEND_GET() \
	((io0x52 & 0x8000u) >> 15U)
#define IO_MLX16__ITC_PEND2_GET() \
	((io0x54 & 0xffffu) >> 0U)
#define IO_MLX16__OV_VS_PEND_GET() \
	((io0x54 & 0x0001u) >> 0U)
#define IO_MLX16__OVT_WARN_PEND_GET() \
	((io0x54 & 0x0002u) >> 1U)
#define IO_MLX16__UV_VDD5_PEND_GET() \
	((io0x54 & 0x0004u) >> 2U)
#define IO_MLX16__AA_IN_FE_PEND_GET() \
	((io0x54 & 0x0008u) >> 3U)
#define IO_MLX16__UART_SB_PEND_GET() \
	((io0x54 & 0x0010u) >> 4U)
#define IO_MLX16__UART_RS_PEND_GET() \
	((io0x54 & 0x0020u) >> 5U)
#define IO_MLX16__UART_RR_PEND_GET() \
	((io0x54 & 0x0040u) >> 6U)
#define IO_MLX16__UART_TS_PEND_GET() \
	((io0x54 & 0x0080u) >> 7U)
#define IO_MLX16__UART_TR_PEND_GET() \
	((io0x54 & 0x0100u) >> 8U)
#define IO_MLX16__UART_TE_PEND_GET() \
	((io0x54 & 0x0200u) >> 9U)
#define IO_MLX16__MFT_PEND_GET() \
	((io0x54 & 0x0400u) >> 10U)
#define IO_MLX16__MFR_PEND_GET() \
	((io0x54 & 0x0800u) >> 11U)
#define IO_MLX16__MHR_PEND_GET() \
	((io0x54 & 0x1000u) >> 12U)
#define IO_MLX16__MER_PEND_GET() \
	((io0x54 & 0x2000u) >> 13U)
#define IO_MLX16__TX_TIMEOUT_PEND_GET() \
	((io0x54 & 0x4000u) >> 14U)
#define IO_MLX16__MLX16_SOFT_PEND_GET() \
	((io0x54 & 0x8000u) >> 15U)
#define IO_MLX16__SWI_GET() \
	(io0x6a >> 15U)
#define IO_MLX16__ITC_MASK0_GET() \
	((io0x6c & 0xffffu) >> 0U)
#define IO_MLX16__MLX16_EXCHG_ITC_GET() \
	((io0x6c & 0x0001u) >> 0U)
#define IO_MLX16__MLX16_DMAERR_ITC_GET() \
	((io0x6c & 0x0002u) >> 1U)
#define IO_MLX16__AWD_ATT_ITC_GET() \
	((io0x6c & 0x0004u) >> 2U)
#define IO_MLX16__IWD_ATT_ITC_GET() \
	((io0x6c & 0x0008u) >> 3U)
#define IO_MLX16__EE_ECC_ITC_GET() \
	((io0x6c & 0x0010u) >> 4U)
#define IO_MLX16__FL_ECC_ITC_GET() \
	((io0x6c & 0x0020u) >> 5U)
#define IO_MLX16__UV_VDDA_ITC_GET() \
	((io0x6c & 0x0040u) >> 6U)
#define IO_MLX16__UV_VS_ITC_GET() \
	((io0x6c & 0x0080u) >> 7U)
#define IO_MLX16__OVT_ITC_GET() \
	((io0x6c & 0x0100u) >> 8U)
#define IO_MLX16__STIMER_ITC_GET() \
	((io0x6c & 0x0200u) >> 9U)
#define IO_MLX16__CTIMER0_1_ITC_GET() \
	((io0x6c & 0x0400u) >> 10U)
#define IO_MLX16__CTIMER0_2_ITC_GET() \
	((io0x6c & 0x0800u) >> 11U)
#define IO_MLX16__CTIMER0_3_ITC_GET() \
	((io0x6c & 0x1000u) >> 12U)
#define IO_MLX16__CTIMER1_1_ITC_GET() \
	((io0x6c & 0x2000u) >> 13U)
#define IO_MLX16__CTIMER1_2_ITC_GET() \
	((io0x6c & 0x4000u) >> 14U)
#define IO_MLX16__CTIMER1_3_ITC_GET() \
	((io0x6c & 0x8000u) >> 15U)
#define IO_MLX16__ITC_MASK1_GET() \
	((io0x6e & 0xffffu) >> 0U)
#define IO_MLX16__PWM_MASTER1_CMP_ITC_GET() \
	((io0x6e & 0x0001u) >> 0U)
#define IO_MLX16__PWM_MASTER1_END_ITC_GET() \
	((io0x6e & 0x0002u) >> 1U)
#define IO_MLX16__PWM_SLAVE1_CMP_ITC_GET() \
	((io0x6e & 0x0004u) >> 2U)
#define IO_MLX16__PWM_SLAVE2_CMP_ITC_GET() \
	((io0x6e & 0x0008u) >> 3U)
#define IO_MLX16__PWM_SLAVE3_CMP_ITC_GET() \
	((io0x6e & 0x0010u) >> 4U)
#define IO_MLX16__PWM_SLAVE4_CMP_ITC_GET() \
	((io0x6e & 0x0020u) >> 5U)
#define IO_MLX16__PWM_SLAVE5_CMP_ITC_GET() \
	((io0x6e & 0x0040u) >> 6U)
#define IO_MLX16__PWM_SLAVE6_CMP_ITC_GET() \
	((io0x6e & 0x0080u) >> 7U)
#define IO_MLX16__PWM_SLAVE7_CMP_ITC_GET() \
	((io0x6e & 0x0100u) >> 8U)
#define IO_MLX16__PWM_SLAVE8_CMP_ITC_GET() \
	((io0x6e & 0x0200u) >> 9U)
#define IO_MLX16__PWM_SLAVE9_CMP_ITC_GET() \
	((io0x6e & 0x0400u) >> 10U)
#define IO_MLX16__PWM_SLAVE10_CMP_ITC_GET() \
	((io0x6e & 0x0800u) >> 11U)
#define IO_MLX16__PWM_SLAVE11_CMP_ITC_GET() \
	((io0x6e & 0x1000u) >> 12U)
#define IO_MLX16__ADC_SAR_ITC_GET() \
	((io0x6e & 0x2000u) >> 13U)
#define IO_MLX16__EE_COMPLETE_ITC_GET() \
	((io0x6e & 0x4000u) >> 14U)
#define IO_MLX16__FL_COMPLETE_ITC_GET() \
	((io0x6e & 0x8000u) >> 15U)
#define IO_MLX16__ITC_MASK2_GET() \
	((io0x70 & 0xffffu) >> 0U)
#define IO_MLX16__OV_VS_ITC_GET() \
	((io0x70 & 0x0001u) >> 0U)
#define IO_MLX16__OVT_WARN_ITC_GET() \
	((io0x70 & 0x0002u) >> 1U)
#define IO_MLX16__UV_VDD5_ITC_GET() \
	((io0x70 & 0x0004u) >> 2U)
#define IO_MLX16__AA_IN_FE_ITC_GET() \
	((io0x70 & 0x0008u) >> 3U)
#define IO_MLX16__UART_SB_ITC_GET() \
	((io0x70 & 0x0010u) >> 4U)
#define IO_MLX16__UART_RS_ITC_GET() \
	((io0x70 & 0x0020u) >> 5U)
#define IO_MLX16__UART_RR_ITC_GET() \
	((io0x70 & 0x0040u) >> 6U)
#define IO_MLX16__UART_TS_ITC_GET() \
	((io0x70 & 0x0080u) >> 7U)
#define IO_MLX16__UART_TR_ITC_GET() \
	((io0x70 & 0x0100u) >> 8U)
#define IO_MLX16__UART_TE_ITC_GET() \
	((io0x70 & 0x0200u) >> 9U)
#define IO_MLX16__MFT_ITC_GET() \
	((io0x70 & 0x0400u) >> 10U)
#define IO_MLX16__MFR_ITC_GET() \
	((io0x70 & 0x0800u) >> 11U)
#define IO_MLX16__MHR_ITC_GET() \
	((io0x70 & 0x1000u) >> 12U)
#define IO_MLX16__MER_ITC_GET() \
	((io0x70 & 0x2000u) >> 13U)
#define IO_MLX16__TX_TIMEOUT_ITC_GET() \
	((io0x70 & 0x4000u) >> 14U)
#define IO_MLX16__MLX16_SOFT_ITC_GET() \
	((io0x70 & 0x8000u) >> 15U)
#define IO_MLX16__ITC_PRIO0_GET() \
	((io0x88 & 0xffffu) >> 0U)
#define IO_MLX16__STIMER_PRIO_GET() \
	((io0x88 & 0x0003u) >> 0U)
#define IO_MLX16__CTIMER0_1_PRIO_GET() \
	((io0x88 & 0x000cu) >> 2U)
#define IO_MLX16__CTIMER0_2_PRIO_GET() \
	((io0x88 & 0x0030u) >> 4U)
#define IO_MLX16__CTIMER0_3_PRIO_GET() \
	((io0x88 & 0x00c0u) >> 6U)
#define IO_MLX16__CTIMER1_1_PRIO_GET() \
	((io0x88 & 0x0300u) >> 8U)
#define IO_MLX16__CTIMER1_2_PRIO_GET() \
	((io0x88 & 0x0c00u) >> 10U)
#define IO_MLX16__CTIMER1_3_PRIO_GET() \
	((io0x88 & 0x3000u) >> 12U)
#define IO_MLX16__PWM_MASTER1_CMP_PRIO_GET() \
	((io0x88 & 0xc000u) >> 14U)
#define IO_MLX16__ITC_PRIO1_GET() \
	((io0x8a & 0xffffu) >> 0U)
#define IO_MLX16__PWM_MASTER1_END_PRIO_GET() \
	((io0x8a & 0x0003u) >> 0U)
#define IO_MLX16__PWM_SLAVE1_CMP_PRIO_GET() \
	((io0x8a & 0x000cu) >> 2U)
#define IO_MLX16__PWM_SLAVE2_CMP_PRIO_GET() \
	((io0x8a & 0x0030u) >> 4U)
#define IO_MLX16__PWM_SLAVE3_CMP_PRIO_GET() \
	((io0x8a & 0x00c0u) >> 6U)
#define IO_MLX16__PWM_SLAVE4_CMP_PRIO_GET() \
	((io0x8a & 0x0300u) >> 8U)
#define IO_MLX16__PWM_SLAVE5_CMP_PRIO_GET() \
	((io0x8a & 0x0c00u) >> 10U)
#define IO_MLX16__PWM_SLAVE6_CMP_PRIO_GET() \
	((io0x8a & 0x3000u) >> 12U)
#define IO_MLX16__PWM_SLAVE7_CMP_PRIO_GET() \
	((io0x8a & 0xc000u) >> 14U)
#define IO_MLX16__ITC_PRIO2_GET() \
	((io0x8c & 0xffffu) >> 0U)
#define IO_MLX16__PWM_SLAVE8_CMP_PRIO_GET() \
	((io0x8c & 0x0003u) >> 0U)
#define IO_MLX16__PWM_SLAVE9_CMP_PRIO_GET() \
	((io0x8c & 0x000cu) >> 2U)
#define IO_MLX16__PWM_SLAVE10_CMP_PRIO_GET() \
	((io0x8c & 0x0030u) >> 4U)
#define IO_MLX16__PWM_SLAVE11_CMP_PRIO_GET() \
	((io0x8c & 0x00c0u) >> 6U)
#define IO_MLX16__ADC_SAR_PRIO_GET() \
	((io0x8c & 0x0300u) >> 8U)
#define IO_MLX16__EE_COMPLETE_PRIO_GET() \
	((io0x8c & 0x0c00u) >> 10U)
#define IO_MLX16__FL_COMPLETE_PRIO_GET() \
	((io0x8c & 0x3000u) >> 12U)
#define IO_MLX16__OV_VS_PRIO_GET() \
	((io0x8c & 0xc000u) >> 14U)
#define IO_MLX16__ITC_PRIO3_GET() \
	((io0x8e & 0xffffu) >> 0U)
#define IO_MLX16__OVT_WARN_PRIO_GET() \
	((io0x8e & 0x0003u) >> 0U)
#define IO_MLX16__UV_VDD5_PRIO_GET() \
	((io0x8e & 0x000cu) >> 2U)
#define IO_MLX16__AA_IN_FE_PRIO_GET() \
	((io0x8e & 0x0030u) >> 4U)
#define IO_MLX16__UART_SB_PRIO_GET() \
	((io0x8e & 0x00c0u) >> 6U)
#define IO_MLX16__UART_RS_PRIO_GET() \
	((io0x8e & 0x0300u) >> 8U)
#define IO_MLX16__UART_RR_PRIO_GET() \
	((io0x8e & 0x0c00u) >> 10U)
#define IO_MLX16__UART_TS_PRIO_GET() \
	((io0x8e & 0x3000u) >> 12U)
#define IO_MLX16__UART_TR_PRIO_GET() \
	((io0x8e & 0xc000u) >> 14U)
#define IO_MLX16__ITC_PRIO4_GET() \
	((io0x90 & 0x0fffu) >> 0U)
#define IO_MLX16__UART_TE_PRIO_GET() \
	((io0x90 & 0x0003u) >> 0U)
#define IO_MLX16__MFT_PRIO_GET() \
	((io0x90 & 0x000cu) >> 2U)
#define IO_MLX16__MFR_PRIO_GET() \
	((io0x90 & 0x0030u) >> 4U)
#define IO_MLX16__MHR_PRIO_GET() \
	((io0x90 & 0x00c0u) >> 6U)
#define IO_MLX16__MER_PRIO_GET() \
	((io0x90 & 0x0300u) >> 8U)
#define IO_MLX16__TX_TIMEOUT_PRIO_GET() \
	((io0x90 & 0x0c00u) >> 10U)
#define IO_MLX16__SHELL_VERSION_GET() \
	(io0xbe >> 0U)
#define IO_MLX16__CPU_FP0ADR_GET() \
	(io0xc0 >> 0U)
#define IO_VERSION__VERSION_L_GET() \
	(io0x100 >> 0U)
#define IO_VERSION__VERSION_H_GET() \
	(io0x102 >> 0U)
#define IO_RAM_BIST__KEY_GET() \
	((io0x104 & 0xffffu) >> 0U)
#define IO_RAM_BIST__PHASE_GET() \
	((io0x104 & 0x0007u) >> 0U)
#define IO_RAM_BIST__TRANSPARENT_GET() \
	((io0x104 & 0x0100u) >> 8U)
#define IO_RAM_BIST__REGULAR_GET() \
	((io0x104 & 0x0200u) >> 9U)
#define IO_RAM_BIST__RUNNING_GET() \
	((io0x104 & 0x0400u) >> 10U)
#define IO_RAM_BIST__VALID_CLOCK_GET() \
	((io0x104 & 0x0800u) >> 11U)
#define IO_RAM_BIST__REGULAR_BIST_ERROR_GET() \
	((io0x104 & 0x1000u) >> 12U)
#define IO_RAM_BIST__COMPLETED_GET() \
	((io0x104 & 0x8000u) >> 15U)
#define IO_RAM_BIST__LFSR_GET() \
	((io0x106 & 0xffffu) >> 0U)
#define IO_RAM_BIST__LSFR_GOT_GET() \
	((io0x106 & 0xffffu) >> 0U)
#define IO_RAM_BIST__LFSR_EXPECTED_GET() \
	(io0x108 >> 0U)
#define IO_RAM_BIST__ADL_GET() \
	(io0x10a >> 0U)
#define IO_RAM_BIST__ADH_GET() \
	(io0x10c >> 0U)
#define IO_RAM_BIST__ADD_START_L_GET() \
	(io0x10e >> 0U)
#define IO_RAM_BIST__ADD_START_H_GET() \
	(io0x110 >> 0U)
#define IO_RAM_BIST__ADD_STOP_L_GET() \
	(io0x112 >> 0U)
#define IO_RAM_BIST__ADD_STOP_H_GET() \
	(io0x114 >> 0U)
#define IO_RAM_BIST__NB_ECC_BITS_GET() \
	((io0x116 & 0x001fu) >> 0U)
#define IO_RAM_BIST__WORD_BIST_GET() \
	((io0x116 & 0x0020u) >> 5U)
#define IO_RAM_BIST__FUNCTIONAL_BIST_GET() \
	((io0x116 & 0x0040u) >> 6U)
#define IO_RAM_BIST__ADD_SCRAMBLE_GET() \
	((io0x116 & 0x0700u) >> 8U)
#define IO_RAM_BIST__SIGNATURE_INIT_GET() \
	((io0x116 & 0x1000u) >> 12U)
#define IO_CTIMER1__TREGB_GET() \
	(io0x118 >> 0U)
#define IO_CTIMER1__TREGA_GET() \
	(io0x11a >> 0U)
#define IO_CTIMER1__TCNT_GET() \
	(io0x11c >> 0U)
#define IO_CTIMER1__START_GET() \
	((io0x11e & 0x0001u) >> 0U)
#define IO_CTIMER1__STOP_GET() \
	((io0x11e & 0x0002u) >> 1U)
#define IO_CTIMER1__EDGA_GET() \
	((io0x11e & 0x000cu) >> 2U)
#define IO_CTIMER1__EDGB_GET() \
	((io0x11e & 0x0030u) >> 4U)
#define IO_CTIMER1__PWMI_GET() \
	((io0x11e & 0x0040u) >> 6U)
#define IO_CTIMER1__POL_GET() \
	((io0x11e & 0x0080u) >> 7U)
#define IO_CTIMER1__OVRA_GET() \
	((io0x11e & 0x0100u) >> 8U)
#define IO_CTIMER1__OVRB_GET() \
	((io0x11e & 0x0200u) >> 9U)
#define IO_CTIMER1__ENCMP_GET() \
	((io0x11e & 0x0400u) >> 10U)
#define IO_CTIMER1__MODE_GET() \
	((io0x11e & 0x3800u) >> 11U)
#define IO_CTIMER1__DIV_GET() \
	((io0x11e & 0xc000u) >> 14U)
#define IO_PWM_MASTER1__CMP_GET() \
	(io0x120 >> 0U)
#define IO_PWM_MASTER1__HT_GET() \
	(io0x122 >> 0U)
#define IO_PWM_MASTER1__LT_GET() \
	(io0x124 >> 0U)
#define IO_PWM_MASTER1__PER_GET() \
	(io0x126 >> 0U)
#define IO_PWM_MASTER1__START_GET() \
	((io0x128 & 0x0001u) >> 0U)
#define IO_PWM_MASTER1__STOP_GET() \
	((io0x128 & 0x0002u) >> 1U)
#define IO_PWM_MASTER1__SLAVE_GET() \
	((io0x128 & 0x0004u) >> 2U)
#define IO_PWM_MASTER1__MODE_GET() \
	((io0x128 & 0x0018u) >> 3U)
#define IO_PWM_MASTER1__POL_GET() \
	((io0x128 & 0x0020u) >> 5U)
#define IO_PWM_MASTER1__IDLE_GET() \
	((io0x128 & 0x0040u) >> 6U)
#define IO_PWM_MASTER1__PWM_IN_GET() \
	((io0x128 & 0x0080u) >> 7U)
#define IO_PWM_MASTER1__PSCLN_GET() \
	((io0x128 & 0x0f00u) >> 8U)
#define IO_PWM_MASTER1__PSCLM_GET() \
	((io0x128 & 0xf000u) >> 12U)
#define IO_PWM_SLAVE1__CMP_GET() \
	(io0x12a >> 0U)
#define IO_PWM_SLAVE1__HT_GET() \
	(io0x12c >> 0U)
#define IO_PWM_SLAVE1__LT_GET() \
	(io0x12e >> 0U)
#define IO_PWM_SLAVE1__PER_GET() \
	(io0x130 >> 0U)
#define IO_PWM_SLAVE1__START_GET() \
	((io0x132 & 0x0001u) >> 0U)
#define IO_PWM_SLAVE1__STOP_GET() \
	((io0x132 & 0x0002u) >> 1U)
#define IO_PWM_SLAVE1__SLAVE_GET() \
	((io0x132 & 0x0004u) >> 2U)
#define IO_PWM_SLAVE1__MODE_GET() \
	((io0x132 & 0x0018u) >> 3U)
#define IO_PWM_SLAVE1__POL_GET() \
	((io0x132 & 0x0020u) >> 5U)
#define IO_PWM_SLAVE1__IDLE_GET() \
	((io0x132 & 0x0040u) >> 6U)
#define IO_PWM_SLAVE1__PWM_IN_GET() \
	((io0x132 & 0x0080u) >> 7U)
#define IO_PWM_SLAVE1__PSCLN_GET() \
	((io0x132 & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE1__PSCLM_GET() \
	((io0x132 & 0xf000u) >> 12U)
#define IO_PWM_SLAVE2__CMP_GET() \
	(io0x134 >> 0U)
#define IO_PWM_SLAVE2__HT_GET() \
	(io0x136 >> 0U)
#define IO_PWM_SLAVE2__LT_GET() \
	(io0x138 >> 0U)
#define IO_PWM_SLAVE2__PER_GET() \
	(io0x13a >> 0U)
#define IO_PWM_SLAVE2__START_GET() \
	((io0x13c & 0x0001u) >> 0U)
#define IO_PWM_SLAVE2__STOP_GET() \
	((io0x13c & 0x0002u) >> 1U)
#define IO_PWM_SLAVE2__SLAVE_GET() \
	((io0x13c & 0x0004u) >> 2U)
#define IO_PWM_SLAVE2__MODE_GET() \
	((io0x13c & 0x0018u) >> 3U)
#define IO_PWM_SLAVE2__POL_GET() \
	((io0x13c & 0x0020u) >> 5U)
#define IO_PWM_SLAVE2__IDLE_GET() \
	((io0x13c & 0x0040u) >> 6U)
#define IO_PWM_SLAVE2__PWM_IN_GET() \
	((io0x13c & 0x0080u) >> 7U)
#define IO_PWM_SLAVE2__PSCLN_GET() \
	((io0x13c & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE2__PSCLM_GET() \
	((io0x13c & 0xf000u) >> 12U)
#define IO_PWM_SLAVE3__CMP_GET() \
	(io0x13e >> 0U)
#define IO_PWM_SLAVE3__HT_GET() \
	(io0x140 >> 0U)
#define IO_PWM_SLAVE3__LT_GET() \
	(io0x142 >> 0U)
#define IO_PWM_SLAVE3__PER_GET() \
	(io0x144 >> 0U)
#define IO_PWM_SLAVE3__START_GET() \
	((io0x146 & 0x0001u) >> 0U)
#define IO_PWM_SLAVE3__STOP_GET() \
	((io0x146 & 0x0002u) >> 1U)
#define IO_PWM_SLAVE3__SLAVE_GET() \
	((io0x146 & 0x0004u) >> 2U)
#define IO_PWM_SLAVE3__MODE_GET() \
	((io0x146 & 0x0018u) >> 3U)
#define IO_PWM_SLAVE3__POL_GET() \
	((io0x146 & 0x0020u) >> 5U)
#define IO_PWM_SLAVE3__IDLE_GET() \
	((io0x146 & 0x0040u) >> 6U)
#define IO_PWM_SLAVE3__PWM_IN_GET() \
	((io0x146 & 0x0080u) >> 7U)
#define IO_PWM_SLAVE3__PSCLN_GET() \
	((io0x146 & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE3__PSCLM_GET() \
	((io0x146 & 0xf000u) >> 12U)
#define IO_PWM_SLAVE4__CMP_GET() \
	(io0x148 >> 0U)
#define IO_PWM_SLAVE4__HT_GET() \
	(io0x14a >> 0U)
#define IO_PWM_SLAVE4__LT_GET() \
	(io0x14c >> 0U)
#define IO_PWM_SLAVE4__PER_GET() \
	(io0x14e >> 0U)
#define IO_PWM_SLAVE4__START_GET() \
	((io0x150 & 0x0001u) >> 0U)
#define IO_PWM_SLAVE4__STOP_GET() \
	((io0x150 & 0x0002u) >> 1U)
#define IO_PWM_SLAVE4__SLAVE_GET() \
	((io0x150 & 0x0004u) >> 2U)
#define IO_PWM_SLAVE4__MODE_GET() \
	((io0x150 & 0x0018u) >> 3U)
#define IO_PWM_SLAVE4__POL_GET() \
	((io0x150 & 0x0020u) >> 5U)
#define IO_PWM_SLAVE4__IDLE_GET() \
	((io0x150 & 0x0040u) >> 6U)
#define IO_PWM_SLAVE4__PWM_IN_GET() \
	((io0x150 & 0x0080u) >> 7U)
#define IO_PWM_SLAVE4__PSCLN_GET() \
	((io0x150 & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE4__PSCLM_GET() \
	((io0x150 & 0xf000u) >> 12U)
#define IO_PWM_SLAVE5__CMP_GET() \
	(io0x152 >> 0U)
#define IO_PWM_SLAVE5__HT_GET() \
	(io0x154 >> 0U)
#define IO_PWM_SLAVE5__LT_GET() \
	(io0x156 >> 0U)
#define IO_PWM_SLAVE5__PER_GET() \
	(io0x158 >> 0U)
#define IO_PWM_SLAVE5__START_GET() \
	((io0x15a & 0x0001u) >> 0U)
#define IO_PWM_SLAVE5__STOP_GET() \
	((io0x15a & 0x0002u) >> 1U)
#define IO_PWM_SLAVE5__SLAVE_GET() \
	((io0x15a & 0x0004u) >> 2U)
#define IO_PWM_SLAVE5__MODE_GET() \
	((io0x15a & 0x0018u) >> 3U)
#define IO_PWM_SLAVE5__POL_GET() \
	((io0x15a & 0x0020u) >> 5U)
#define IO_PWM_SLAVE5__IDLE_GET() \
	((io0x15a & 0x0040u) >> 6U)
#define IO_PWM_SLAVE5__PWM_IN_GET() \
	((io0x15a & 0x0080u) >> 7U)
#define IO_PWM_SLAVE5__PSCLN_GET() \
	((io0x15a & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE5__PSCLM_GET() \
	((io0x15a & 0xf000u) >> 12U)
#define IO_PWM_SLAVE6__CMP_GET() \
	(io0x15c >> 0U)
#define IO_PWM_SLAVE6__HT_GET() \
	(io0x15e >> 0U)
#define IO_PWM_SLAVE6__LT_GET() \
	(io0x160 >> 0U)
#define IO_PWM_SLAVE6__PER_GET() \
	(io0x162 >> 0U)
#define IO_PWM_SLAVE6__START_GET() \
	((io0x164 & 0x0001u) >> 0U)
#define IO_PWM_SLAVE6__STOP_GET() \
	((io0x164 & 0x0002u) >> 1U)
#define IO_PWM_SLAVE6__SLAVE_GET() \
	((io0x164 & 0x0004u) >> 2U)
#define IO_PWM_SLAVE6__MODE_GET() \
	((io0x164 & 0x0018u) >> 3U)
#define IO_PWM_SLAVE6__POL_GET() \
	((io0x164 & 0x0020u) >> 5U)
#define IO_PWM_SLAVE6__IDLE_GET() \
	((io0x164 & 0x0040u) >> 6U)
#define IO_PWM_SLAVE6__PWM_IN_GET() \
	((io0x164 & 0x0080u) >> 7U)
#define IO_PWM_SLAVE6__PSCLN_GET() \
	((io0x164 & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE6__PSCLM_GET() \
	((io0x164 & 0xf000u) >> 12U)
#define IO_PWM_SLAVE7__CMP_GET() \
	(io0x166 >> 0U)
#define IO_PWM_SLAVE7__HT_GET() \
	(io0x168 >> 0U)
#define IO_PWM_SLAVE7__LT_GET() \
	(io0x16a >> 0U)
#define IO_PWM_SLAVE7__PER_GET() \
	(io0x16c >> 0U)
#define IO_PWM_SLAVE7__START_GET() \
	((io0x16e & 0x0001u) >> 0U)
#define IO_PWM_SLAVE7__STOP_GET() \
	((io0x16e & 0x0002u) >> 1U)
#define IO_PWM_SLAVE7__SLAVE_GET() \
	((io0x16e & 0x0004u) >> 2U)
#define IO_PWM_SLAVE7__MODE_GET() \
	((io0x16e & 0x0018u) >> 3U)
#define IO_PWM_SLAVE7__POL_GET() \
	((io0x16e & 0x0020u) >> 5U)
#define IO_PWM_SLAVE7__IDLE_GET() \
	((io0x16e & 0x0040u) >> 6U)
#define IO_PWM_SLAVE7__PWM_IN_GET() \
	((io0x16e & 0x0080u) >> 7U)
#define IO_PWM_SLAVE7__PSCLN_GET() \
	((io0x16e & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE7__PSCLM_GET() \
	((io0x16e & 0xf000u) >> 12U)
#define IO_PWM_SLAVE8__CMP_GET() \
	(io0x170 >> 0U)
#define IO_PWM_SLAVE8__HT_GET() \
	(io0x172 >> 0U)
#define IO_PWM_SLAVE8__LT_GET() \
	(io0x174 >> 0U)
#define IO_PWM_SLAVE8__PER_GET() \
	(io0x176 >> 0U)
#define IO_PWM_SLAVE8__START_GET() \
	((io0x178 & 0x0001u) >> 0U)
#define IO_PWM_SLAVE8__STOP_GET() \
	((io0x178 & 0x0002u) >> 1U)
#define IO_PWM_SLAVE8__SLAVE_GET() \
	((io0x178 & 0x0004u) >> 2U)
#define IO_PWM_SLAVE8__MODE_GET() \
	((io0x178 & 0x0018u) >> 3U)
#define IO_PWM_SLAVE8__POL_GET() \
	((io0x178 & 0x0020u) >> 5U)
#define IO_PWM_SLAVE8__IDLE_GET() \
	((io0x178 & 0x0040u) >> 6U)
#define IO_PWM_SLAVE8__PWM_IN_GET() \
	((io0x178 & 0x0080u) >> 7U)
#define IO_PWM_SLAVE8__PSCLN_GET() \
	((io0x178 & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE8__PSCLM_GET() \
	((io0x178 & 0xf000u) >> 12U)
#define IO_PWM_SLAVE9__CMP_GET() \
	(io0x17a >> 0U)
#define IO_PWM_SLAVE9__HT_GET() \
	(io0x17c >> 0U)
#define IO_PWM_SLAVE9__LT_GET() \
	(io0x17e >> 0U)
#define IO_PWM_SLAVE9__PER_GET() \
	(io0x180 >> 0U)
#define IO_PWM_SLAVE9__START_GET() \
	((io0x182 & 0x0001u) >> 0U)
#define IO_PWM_SLAVE9__STOP_GET() \
	((io0x182 & 0x0002u) >> 1U)
#define IO_PWM_SLAVE9__SLAVE_GET() \
	((io0x182 & 0x0004u) >> 2U)
#define IO_PWM_SLAVE9__MODE_GET() \
	((io0x182 & 0x0018u) >> 3U)
#define IO_PWM_SLAVE9__POL_GET() \
	((io0x182 & 0x0020u) >> 5U)
#define IO_PWM_SLAVE9__IDLE_GET() \
	((io0x182 & 0x0040u) >> 6U)
#define IO_PWM_SLAVE9__PWM_IN_GET() \
	((io0x182 & 0x0080u) >> 7U)
#define IO_PWM_SLAVE9__PSCLN_GET() \
	((io0x182 & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE9__PSCLM_GET() \
	((io0x182 & 0xf000u) >> 12U)
#define IO_PWM_SLAVE10__CMP_GET() \
	(io0x184 >> 0U)
#define IO_PWM_SLAVE10__HT_GET() \
	(io0x186 >> 0U)
#define IO_PWM_SLAVE10__LT_GET() \
	(io0x188 >> 0U)
#define IO_PWM_SLAVE10__PER_GET() \
	(io0x18a >> 0U)
#define IO_PWM_SLAVE10__START_GET() \
	((io0x18c & 0x0001u) >> 0U)
#define IO_PWM_SLAVE10__STOP_GET() \
	((io0x18c & 0x0002u) >> 1U)
#define IO_PWM_SLAVE10__SLAVE_GET() \
	((io0x18c & 0x0004u) >> 2U)
#define IO_PWM_SLAVE10__MODE_GET() \
	((io0x18c & 0x0018u) >> 3U)
#define IO_PWM_SLAVE10__POL_GET() \
	((io0x18c & 0x0020u) >> 5U)
#define IO_PWM_SLAVE10__IDLE_GET() \
	((io0x18c & 0x0040u) >> 6U)
#define IO_PWM_SLAVE10__PWM_IN_GET() \
	((io0x18c & 0x0080u) >> 7U)
#define IO_PWM_SLAVE10__PSCLN_GET() \
	((io0x18c & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE10__PSCLM_GET() \
	((io0x18c & 0xf000u) >> 12U)
#define IO_PWM_SLAVE11__CMP_GET() \
	(io0x18e >> 0U)
#define IO_PWM_SLAVE11__HT_GET() \
	(io0x190 >> 0U)
#define IO_PWM_SLAVE11__LT_GET() \
	(io0x192 >> 0U)
#define IO_PWM_SLAVE11__PER_GET() \
	(io0x194 >> 0U)
#define IO_PWM_SLAVE11__START_GET() \
	((io0x196 & 0x0001u) >> 0U)
#define IO_PWM_SLAVE11__STOP_GET() \
	((io0x196 & 0x0002u) >> 1U)
#define IO_PWM_SLAVE11__SLAVE_GET() \
	((io0x196 & 0x0004u) >> 2U)
#define IO_PWM_SLAVE11__MODE_GET() \
	((io0x196 & 0x0018u) >> 3U)
#define IO_PWM_SLAVE11__POL_GET() \
	((io0x196 & 0x0020u) >> 5U)
#define IO_PWM_SLAVE11__IDLE_GET() \
	((io0x196 & 0x0040u) >> 6U)
#define IO_PWM_SLAVE11__PWM_IN_GET() \
	((io0x196 & 0x0080u) >> 7U)
#define IO_PWM_SLAVE11__PSCLN_GET() \
	((io0x196 & 0x0f00u) >> 8U)
#define IO_PWM_SLAVE11__PSCLM_GET() \
	((io0x196 & 0xf000u) >> 12U)
#define IO_ADC_SAR__START_GET() \
	((io0x198 & 0x0001u) >> 0U)
#define IO_ADC_SAR__STOP_GET() \
	((io0x198 & 0x0002u) >> 1U)
#define IO_ADC_SAR__SOS_SOURCE_GET() \
	((io0x198 & 0x000cu) >> 2U)
#define IO_ADC_SAR__SOC_SOURCE_GET() \
	((io0x198 & 0x0030u) >> 4U)
#define IO_ADC_SAR__NO_INTERLEAVE_GET() \
	((io0x198 & 0x0040u) >> 6U)
#define IO_ADC_SAR__SATURATE_GET() \
	((io0x198 & 0x0080u) >> 7U)
#define IO_ADC_SAR__INT_SCHEME_GET() \
	((io0x198 & 0x0300u) >> 8U)
#define IO_ADC_SAR__ASB_GET() \
	((io0x198 & 0x0c00u) >> 10U)
#define IO_ADC_SAR__ADC_WIDTH_GET() \
	((io0x198 & 0x1000u) >> 12U)
#define IO_ADC_SAR__SBASE_0_GET() \
	(io0x19a >> 0U)
#define IO_ADC_SAR__PAUSE_GET() \
	((io0x19c & 0x0001u) >> 0U)
#define IO_ADC_SAR__RESUME_GET() \
	((io0x19c & 0x0002u) >> 1U)
#define IO_ADC_SAR__SW_TRIG_GET() \
	((io0x19c & 0x0004u) >> 2U)
#define IO_ADC_SAR__READY_GET() \
	((io0x19c & 0x0008u) >> 3U)
#define IO_ADC_SAR__LAST_INT_SRC_GET() \
	((io0x19c & 0x0030u) >> 4U)
#define IO_ADC_SAR__STATE_GET() \
	((io0x19c & 0x00c0u) >> 6U)
#define IO_ADC_SAR__ADC_OVF_GET() \
	((io0x19c & 0x0100u) >> 8U)
#define IO_ADC_SAR__ADC_ERR_GET() \
	((io0x19c & 0x0200u) >> 9U)
#define IO_ADC_SAR__MEM_ERR_GET() \
	((io0x19c & 0x0400u) >> 10U)
#define IO_ADC_SAR__FRAME_ERR_GET() \
	((io0x19c & 0x0800u) >> 11U)
#define IO_ADC_SAR__ABORTED_GET() \
	((io0x19c & 0x1000u) >> 12U)
#define IO_ADC_SAR__ADC_CLK_DIV_GET() \
	(io0x19e >> 0U)
#define IO_ADC_SAR__TEST_CONV2_GET() \
	((io0x1a0 & 0x0001u) >> 0U)
#define IO_ADC_SAR__TEST_CONV3_GET() \
	((io0x1a0 & 0x0002u) >> 1U)
#define IO_ADC_SAR__TEST_CONV4_GET() \
	((io0x1a0 & 0x0004u) >> 2U)
#define IO_ADC_SAR__TEST_TRIM_GET() \
	((io0x1a0 & 0x0008u) >> 3U)
#define IO_ADC_SAR__ADC_DATA_GET() \
	((io0x1a2 & 0x03ffu) >> 0U)
#define IO_ADC_SAR__SEL_TR_ADCREF_GET() \
	((io0x1a2 & 0x0400u) >> 10U)
#define IO_ADC_SAR__TR_ADCREF1_GET() \
	((io0x1a4 & 0x007fu) >> 0U)
#define IO_ADC_SAR__TR_ADCREF2_GET() \
	((io0x1a4 & 0x3f80u) >> 7U)
#define IO_ADC_SAR__TR_ADCREF3_GET() \
	((io0x1a6 & 0x007fu) >> 0U)
#define IO_ADC_SAR__IDDQ_ADC_GET() \
	((io0x1a6 & 0x0080u) >> 7U)
#define IO_PORT_ADC_CTRL__ADC_INV_COMPOUT_GET() \
	((io0x1a8 & 0x0001u) >> 0U)
#define IO_PORT_ADC_CTRL__ADC_EN_GET() \
	((io0x1a8 & 0x0002u) >> 1U)
#define IO_PORT_TEST_ADC__TEST_DAC_GET() \
	((io0x1aa & 0x0001u) >> 0U)
#define IO_PORT_TEST_ADC__TEST_DAC_BUF_GET() \
	((io0x1aa & 0x0002u) >> 1U)
#define IO_PORT_TEST_ADC__TEST_LDAC_GET() \
	((io0x1aa & 0x0004u) >> 2U)
#define IO_PORT_TEST_ADC__TEST_HDAC_GET() \
	((io0x1aa & 0x0008u) >> 3U)
#define IO_PORT_TEST_ADC__TEST_ADC_CURSRC_GET() \
	((io0x1aa & 0x0010u) >> 4U)
#define IO_PORT_TEST_ADC__TEST_ADC_REF_GET() \
	((io0x1aa & 0x0060u) >> 5U)
#define IO_PORT_TEST_ADC__TEST_ADC_IN_GET() \
	((io0x1aa & 0x0180u) >> 7U)
#define IO_PORT_TEST_ADC__TEST_ADC_SPARE_GET() \
	((io0x1aa & 0x0e00u) >> 9U)
#define IO_EEPROM_FLASH__T_10NS_GET() \
	((io0x1ac & 0x0001u) >> 0U)
#define IO_EEPROM_FLASH__T_50NS_GET() \
	((io0x1ac & 0x001cu) >> 2U)
#define IO_EEPROM_FLASH__T_10US_GET() \
	((io0x1ac & 0x03e0u) >> 5U)
#define IO_EEPROM_FLASH__T_50US_GET() \
	((io0x1ac & 0x7c00u) >> 10U)
#define IO_EEPROM_FLASH__LOCK_T_GET() \
	((io0x1ac & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__T_1US_GET() \
	((io0x1ae & 0x001fu) >> 0U)
#define IO_EEPROM_FLASH__LOCK_1US_GET() \
	((io0x1ae & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__FL_DMA_GET() \
	((io0x1b0 & 0x0001u) >> 0U)
#define IO_EEPROM_FLASH__EE_FL_VERSION_GET() \
	((io0x1b0 & 0x00f0u) >> 4U)
#define IO_EEPROM_FLASH__EE_DMA_GET() \
	((io0x1b0 & 0x0100u) >> 8U)
#define IO_EEPROM_FLASH__EE_EXTEND_DATA_GET() \
	((io0x1b0 & 0x0200u) >> 9U)
#define IO_EEPROM_FLASH__EE_FL_PDN_LOW_GET() \
	((io0x1b0 & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__FL_COMMAND_GET() \
	((io0x1b2 & 0xffffu) >> 0U)
#define IO_EEPROM_FLASH__FL_STATUS_GET() \
	((io0x1b2 & 0x000fu) >> 0U)
#define IO_EEPROM_FLASH__FL_ER_TIME_GET() \
	((io0x1b4 & 0x00ffu) >> 0U)
#define IO_EEPROM_FLASH__FL_WR_TIME_GET() \
	((io0x1b4 & 0x7f00u) >> 8U)
#define IO_EEPROM_FLASH__FL_LOCK_ER_WR_GET() \
	((io0x1b4 & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__FL_WAIT_STATES_GET() \
	((io0x1b6 & 0x0007u) >> 0U)
#define IO_EEPROM_FLASH__FL_BYPASS_QUEUE_GET() \
	((io0x1b6 & 0x0010u) >> 4U)
#define IO_EEPROM_FLASH__FL_PREDICTION_BEHAVIOR_GET() \
	((io0x1b6 & 0x00c0u) >> 6U)
#define IO_EEPROM_FLASH__FL_DED_RETRY_GET() \
	((io0x1b6 & 0x0700u) >> 8U)
#define IO_EEPROM_FLASH__FL_HALT_BEHAVIOR_GET() \
	((io0x1b6 & 0x3000u) >> 12U)
#define IO_EEPROM_FLASH__FL_LOCK_RDY_GET() \
	((io0x1b6 & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__FL_SBE_GET() \
	((io0x1b8 & 0x0001u) >> 0U)
#define IO_EEPROM_FLASH__FL_DATA_CORRUPTED_GET() \
	((io0x1b8 & 0x0002u) >> 1U)
#define IO_EEPROM_FLASH__FL_EXTENDED_DATA_GET() \
	((io0x1b8 & 0x0100u) >> 8U)
#define IO_EEPROM_FLASH__FL_PAGE_NUMBER_GET() \
	((io0x1ba & 0x000fu) >> 0U)
#define IO_EEPROM_FLASH__FL_SECTOR_NUMBER_GET() \
	((io0x1ba & 0x00f0u) >> 4U)
#define IO_EEPROM_FLASH__FL_CS_AREA_GET() \
	((io0x1ba & 0x0100u) >> 8U)
#define IO_EEPROM_FLASH__EE_ACTIVE_GET() \
	((io0x1bc & 0x0001u) >> 0U)
#define IO_EEPROM_FLASH__EE_CONFIGURED_GET() \
	((io0x1bc & 0x0002u) >> 1U)
#define IO_EEPROM_FLASH__EE_WE_KEY_GET() \
	((io0x1bc & 0x00f0u) >> 4U)
#define IO_EEPROM_FLASH__EE_W_MODE_GET() \
	((io0x1bc & 0x0700u) >> 8U)
#define IO_EEPROM_FLASH__EE_BUSY_BUF_NOT_EMPTY_GET() \
	((io0x1bc & 0x1000u) >> 12U)
#define IO_EEPROM_FLASH__EE_BUSY_WR_GET() \
	((io0x1bc & 0x2000u) >> 13U)
#define IO_EEPROM_FLASH__EE_BUSY_STBY_GET() \
	((io0x1bc & 0x4000u) >> 14U)
#define IO_EEPROM_FLASH__EE_BUSY_GET() \
	((io0x1bc & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__EE_SBE_1_GET() \
	((io0x1be & 0x0001u) >> 0U)
#define IO_EEPROM_FLASH__EE_DATA_CORRUPTED_1_GET() \
	((io0x1be & 0x0002u) >> 1U)
#define IO_EEPROM_FLASH__EE_SBE_2_GET() \
	((io0x1be & 0x0004u) >> 2U)
#define IO_EEPROM_FLASH__EE_DATA_CORRUPTED_2_GET() \
	((io0x1be & 0x0008u) >> 3U)
#define IO_EEPROM_FLASH__EE_PRE_WR_TIME_GET() \
	((io0x1c0 & 0x007fu) >> 0U)
#define IO_EEPROM_FLASH__EE_WR_TIME_GET() \
	((io0x1c0 & 0x7f00u) >> 8U)
#define IO_EEPROM_FLASH__EE_LOCK_WR_GET() \
	((io0x1c0 & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__EE_ER_TIME_GET() \
	((io0x1c2 & 0x003fu) >> 0U)
#define IO_EEPROM_FLASH__EE_LOCK_ER_GET() \
	((io0x1c2 & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__EE_PROGRAM_CYCLE_GET() \
	((io0x1c4 & 0x001fu) >> 0U)
#define IO_EEPROM_FLASH__EE_PROGRAM_CYCLE_LOCK_GET() \
	((io0x1c4 & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__EE_HALT_BEHAVIOR_GET() \
	((io0x1c6 & 0x0003u) >> 0U)
#define IO_EEPROM_FLASH__EE_WAIT_STATES_GET() \
	((io0x1c6 & 0x0f00u) >> 8U)
#define IO_EEPROM_FLASH__EE_LOCK_RD_GET() \
	((io0x1c6 & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__EE_INT_INVALID_SRC_GET() \
	(io0x1c8 >> 0U)
#define IO_EEPROM_FLASH__FL_PATTERN_ID_GET() \
	((io0x1d2 & 0x0070u) >> 4U)
#define IO_EEPROM_FLASH__FL_ACCESS_ORDER_GET() \
	((io0x1d2 & 0x0300u) >> 8U)
#define IO_EEPROM_FLASH__FL_TEST_AREA_GET() \
	((io0x1d2 & 0x0c00u) >> 10U)
#define IO_EEPROM_FLASH__FL_ERROR_GET() \
	((io0x1d2 & 0x4000u) >> 14U)
#define IO_EEPROM_FLASH__FL_COMPLETED_GET() \
	((io0x1d2 & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__FL_BITS_ERRORS_GET() \
	(io0x1d4 >> 0U)
#define IO_EEPROM_FLASH__FL_SIG_L_GET() \
	(io0x1d6 >> 0U)
#define IO_EEPROM_FLASH__FL_SIG_H_GET() \
	(io0x1d8 >> 0U)
#define IO_EEPROM_FLASH__EE_CMD_GET() \
	((io0x1da & 0x0003u) >> 0U)
#define IO_EEPROM_FLASH__EE_PATTERN_ID_GET() \
	((io0x1da & 0x0070u) >> 4U)
#define IO_EEPROM_FLASH__EE_ACCESS_ORDER_GET() \
	((io0x1da & 0x0300u) >> 8U)
#define IO_EEPROM_FLASH__EE_TEST_AREA_GET() \
	((io0x1da & 0x0c00u) >> 10U)
#define IO_EEPROM_FLASH__EE_IN_PROGRESS_GET() \
	((io0x1da & 0x2000u) >> 13U)
#define IO_EEPROM_FLASH__EE_ERROR_GET() \
	((io0x1da & 0x4000u) >> 14U)
#define IO_EEPROM_FLASH__EE_COMPLETED_GET() \
	((io0x1da & 0x8000u) >> 15U)
#define IO_EEPROM_FLASH__EE_BITS_ERRORS_GET() \
	(io0x1dc >> 0U)
#define IO_EEPROM_FLASH__EE_SIG_L_GET() \
	(io0x1de >> 0U)
#define IO_EEPROM_FLASH__EE_SIG_H_GET() \
	(io0x1e0 >> 0U)
#define IO_PORT_LIN_XKEY__LIN_XKEY_GET() \
	(io0x1e2 >> 0U)
#define IO_TRIM_BG_BIAS__PRE_TR_BGA_GET() \
	((io0x1e4 & 0x000fu) >> 0U)
#define IO_TRIM_BG_BIAS__PRE_TR_BGD_GET() \
	((io0x1e4 & 0x00f0u) >> 4U)
#define IO_TRIM_BG_BIAS__PRE_TR_BIAS_GET() \
	((io0x1e4 & 0x3f00u) >> 8U)
#define IO_TRIM_BG_BIAS__TR_UNUSED_GET() \
	((io0x1e4 & 0x4000u) >> 14U)
#define IO_TRIM_BG_BIAS__LOCK_GET() \
	((io0x1e4 & 0x8000u) >> 15U)
#define IO_TRIM_VDD__PRE_TR_VDDA_GET() \
	((io0x1e6 & 0x0007u) >> 0U)
#define IO_TRIM_VDD__PRE_TR_VDDD_GET() \
	((io0x1e6 & 0x0038u) >> 3U)
#define IO_TRIM_VDD__PRE_TR_SUP_GET() \
	((io0x1e6 & 0x3fc0u) >> 6U)
#define IO_TRIM_VDD__TR_UNUSED_GET() \
	((io0x1e6 & 0x4000u) >> 14U)
#define IO_TRIM_VDD__LOCK_GET() \
	((io0x1e6 & 0x8000u) >> 15U)
#define IO_TRIM_RCO32M__TR_RCO32M_IN_GET() \
	((io0x1e8 & 0x03ffu) >> 0U)
#define IO_TRIM_RCO32M__TR_UNUSED_GET() \
	((io0x1e8 & 0x7c00u) >> 10U)
#define IO_TRIM_RCO32M__LOCK_GET() \
	((io0x1e8 & 0x8000u) >> 15U)
#define IO_TRIM_RCO1M__PRE_TR_RCO1M_GET() \
	((io0x1ea & 0x00ffu) >> 0U)
#define IO_TRIM_RCO1M__LOCK_GET() \
	((io0x1ea & 0x8000u) >> 15U)
#define IO_PORT_SSCM_CONF__SSCM_EN_GET() \
	((io0x1ec & 0x0001u) >> 0U)
#define IO_PORT_SSCM_CONF__SSCM_SINGLEBIT_GET() \
	((io0x1ec & 0x0002u) >> 1U)
#define IO_PORT_STEP_CONF__STEP_INC_GET() \
	((io0x1ee & 0x000fu) >> 0U)
#define IO_PORT_STEP_CONF__STEP_DUR_GET() \
	((io0x1ee & 0x00f0u) >> 4U)
#define IO_PORT_STEP_CONF__STEP_CNT_GET() \
	((io0x1ee & 0xff00u) >> 8U)
#define IO_PORT_SUPP_TEST__TEST_BGA_GET() \
	((io0x1f0 & 0x0001u) >> 0U)
#define IO_PORT_SUPP_TEST__TEST_VAUX_ADDCURRENT_GET() \
	((io0x1f0 & 0x0002u) >> 1U)
#define IO_PORT_SUPP_TEST__TEST_10U_BIAS_GET() \
	((io0x1f0 & 0x0004u) >> 2U)
#define IO_PORT_SUPP_TEST__TEST_5U_BIASAUX_GET() \
	((io0x1f0 & 0x0008u) >> 3U)
#define IO_PORT_SUPP_TEST__PORTEST_GET() \
	((io0x1f0 & 0x0010u) >> 4U)
#define IO_PORT_SUPP_TEST__SWITCHOFFREG_VDDA_GET() \
	((io0x1f0 & 0x0060u) >> 5U)
#define IO_PORT_SUPP_TEST__SWITCHOFFREG_VDDD_GET() \
	((io0x1f0 & 0x0180u) >> 7U)
#define IO_PORT_SUPP_TEST__SWITCHOFFUV_VDDD_RES_GET() \
	((io0x1f0 & 0x0200u) >> 9U)
#define IO_PORT_SUPP_TEST__SBY_BIAS_GET() \
	((io0x1f0 & 0x0400u) >> 10U)
#define IO_PORT_SUPP_TEST__LOW_VDDD_GET() \
	((io0x1f0 & 0x0800u) >> 11U)
#define IO_PORT_SUPP_TEST__SHOVE_VDDD_GET() \
	((io0x1f0 & 0x1000u) >> 12U)
#define IO_PORT_SUPP_TEST__SHOVE_VDDA_GET() \
	((io0x1f0 & 0x2000u) >> 13U)
#define IO_PORT_SUPP_TEST__SHOVE_VAUX_GET() \
	((io0x1f0 & 0x4000u) >> 14U)
#define IO_PORT_SUPP_TEST__DISABLE_RAM_ROM_GET() \
	((io0x1f0 & 0x8000u) >> 15U)
#define IO_PORT_SUPP2_TEST__IDDQ_REG_VDDA_GET() \
	((io0x1f2 & 0x0001u) >> 0U)
#define IO_PORT_SUPP2_TEST__IDDQ_REG_VDDD_GET() \
	((io0x1f2 & 0x0002u) >> 1U)
#define IO_PORT_SUPP2_TEST__IDDQSENS_REG_VDDD_GET() \
	((io0x1f2 & 0x0004u) >> 2U)
#define IO_PORT_SUPP2_TEST__INT_WU_TEST_GET() \
	((io0x1f2 & 0x0008u) >> 3U)
#define IO_PORT_SUPP2_TEST__IDDQX_GET() \
	((io0x1f2 & 0x0030u) >> 4U)
#define IO_PORT_SUPP2_TEST__TEST_TA0_EN_GET() \
	((io0x1f2 & 0x0040u) >> 6U)
#define IO_PORT_SUPP2_TEST__TEST_TA1_EN_GET() \
	((io0x1f2 & 0x0080u) >> 7U)
#define IO_PORT_SUPP2_TEST__TEST_MEM_ANA_GET() \
	((io0x1f2 & 0x0100u) >> 8U)
#define IO_PORT_SUPP2_TEST__TM_35DEGC_GET() \
	((io0x1f2 & 0x0200u) >> 9U)
#define IO_PORT_SUPP2_TEST__TM_TSDCMP_GET() \
	((io0x1f2 & 0x0400u) >> 10U)
#define IO_PORT_SUPP2_TEST__TM_VGB_O_GET() \
	((io0x1f2 & 0x0800u) >> 11U)
#define IO_PORT_SUPP2_TEST__IDDQ_TEMPSENSE_GET() \
	((io0x1f2 & 0x1000u) >> 12U)
#define IO_PORT_SUPP2_TEST__IDDQ_CLK10K_GET() \
	((io0x1f2 & 0x2000u) >> 13U)
#define IO_PORT_SUPP2_TEST__FGTSM_GET() \
	((io0x1f2 & 0x4000u) >> 14U)
#define IO_PORT_SUPP2_TEST__FSTOP_GET() \
	((io0x1f2 & 0x8000u) >> 15U)
#define IO_PORT_SPARE_TEST__TEST_SUP_GET() \
	(io0x1f4 >> 0U)
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_BGA_GET() \
	((io0x1f6 & 0x0001u) >> 0U)
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_BGD_GET() \
	((io0x1f6 & 0x0002u) >> 1U)
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_VDDA_GET() \
	((io0x1f6 & 0x0004u) >> 2U)
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_VDDD_GET() \
	((io0x1f6 & 0x0008u) >> 3U)
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_BIAS_GET() \
	((io0x1f6 & 0x0010u) >> 4U)
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_SUP_GET() \
	((io0x1f6 & 0x0020u) >> 5U)
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_RCO32M_GET() \
	((io0x1f6 & 0x0040u) >> 6U)
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_RCO1M_GET() \
	((io0x1f6 & 0x0080u) >> 7U)
#define IO_PORT_SUPP_IN__UV_VDDA_IT_GET() \
	((io0x1f8 & 0x0001u) >> 0U)
#define IO_PORT_SUPP_IN__UV_VDDA_SYNC_GET() \
	((io0x1f8 & 0x0002u) >> 1U)
#define IO_PORT_SUPP_IN__UV_VS_IT_GET() \
	((io0x1f8 & 0x0004u) >> 2U)
#define IO_PORT_SUPP_IN__UV_VS_SYNC_GET() \
	((io0x1f8 & 0x0008u) >> 3U)
#define IO_PORT_SUPP_IN__OV_VS_IT_GET() \
	((io0x1f8 & 0x0010u) >> 4U)
#define IO_PORT_SUPP_IN__OV_VS_SYNC_GET() \
	((io0x1f8 & 0x0020u) >> 5U)
#define IO_PORT_SUPP_IN__OVT_IT_GET() \
	((io0x1f8 & 0x0040u) >> 6U)
#define IO_PORT_SUPP_IN__OVT_SYNC_GET() \
	((io0x1f8 & 0x0080u) >> 7U)
#define IO_PORT_SUPP_IN__OVT_WARN_IT_GET() \
	((io0x1f8 & 0x0100u) >> 8U)
#define IO_PORT_SUPP_IN__OVT_WARN_SYNC_GET() \
	((io0x1f8 & 0x0200u) >> 9U)
#define IO_PORT_SUPP_IN__UV_VDD5_IT_GET() \
	((io0x1f8 & 0x0400u) >> 10U)
#define IO_PORT_SUPP_IN__UV_VDD5_SYNC_GET() \
	((io0x1f8 & 0x0800u) >> 11U)
#define IO_PORT_MISC_IN__AIN_SUP_GET() \
	((io0x1fa & 0x00ffu) >> 0U)
#define IO_PORT_MISC_IN__STOP_MODE_GET() \
	((io0x1fa & 0x0100u) >> 8U)
#define IO_PORT_MISC_IN__RSTAT_GET() \
	((io0x1fa & 0x0200u) >> 9U)
#define IO_PORT_MISC_IN__INTERNAL_WU_GET() \
	((io0x1fa & 0x0400u) >> 10U)
#define IO_PORT_MISC_IN__LIN_WU_GET() \
	((io0x1fa & 0x0800u) >> 11U)
#define IO_PORT_MISC_IN__PHYSTAT1_GET() \
	((io0x1fa & 0x1000u) >> 12U)
#define IO_PORT_MISC_IN__AAOUT_IN_GET() \
	((io0x1fa & 0x2000u) >> 13U)
#define IO_PORT_SUPP_CFG__UV_VDDA_FILT_SEL_GET() \
	((io0x1fc & 0x0001u) >> 0U)
#define IO_PORT_SUPP_CFG__UV_VS_FILT_SEL_GET() \
	((io0x1fc & 0x0002u) >> 1U)
#define IO_PORT_SUPP_CFG__OV_VS_FILT_SEL_GET() \
	((io0x1fc & 0x0004u) >> 2U)
#define IO_PORT_SUPP_CFG__OVT_FILT_SEL_GET() \
	((io0x1fc & 0x0008u) >> 3U)
#define IO_PORT_SUPP_CFG__OVT_WARN_FILT_SEL_GET() \
	((io0x1fc & 0x0010u) >> 4U)
#define IO_PORT_SUPP_CFG__UV_VDD5_FILT_SEL_GET() \
	((io0x1fc & 0x0020u) >> 5U)
#define IO_PORT_TIMER_CFG__TIMER0_CHA_SEL_GET() \
	((io0x1fe & 0x000fu) >> 0U)
#define IO_PORT_TIMER_CFG__TIMER0_CHB_SEL_GET() \
	((io0x1fe & 0x00f0u) >> 4U)
#define IO_PORT_TIMER_CFG__TIMER1_CHA_SEL_GET() \
	((io0x1fe & 0x0f00u) >> 8U)
#define IO_PORT_TIMER_CFG__TIMER1_CHB_SEL_GET() \
	((io0x1fe & 0xf000u) >> 12U)
#define IO_PORT_MISC_OUT__CLEAR_STOP_GET() \
	((io0x200 & 0x0001u) >> 0U)
#define IO_PORT_MISC_OUT__SET_RSTAT_GET() \
	((io0x200 & 0x0002u) >> 1U)
#define IO_PORT_MISC_OUT__CLEAR_RSTAT_GET() \
	((io0x200 & 0x0004u) >> 2U)
#define IO_PORT_MISC_OUT__WUI_GET() \
	((io0x200 & 0x0018u) >> 3U)
#define IO_PORT_MISC_OUT__SWITCH_VDDA_TO_5V_GET() \
	((io0x200 & 0x0020u) >> 5U)
#define IO_PORT_MISC_OUT__PRUV_VS_GET() \
	((io0x200 & 0x01c0u) >> 6U)
#define IO_PORT_MISC_OUT__PROV_VS_GET() \
	((io0x200 & 0x0600u) >> 9U)
#define IO_PORT_MISC_OUT__PRUV_VDDA_GET() \
	((io0x200 & 0x0800u) >> 11U)
#define IO_PORT_MISC_OUT__SEL_TEMP_GET() \
	((io0x200 & 0xf000u) >> 12U)
#define IO_PORT_MISC2_OUT__AOUT_SUP_GET() \
	((io0x202 & 0x00ffu) >> 0U)
#define IO_PORT_MISC2_OUT__OVT_DISABLE_GET() \
	((io0x202 & 0x0100u) >> 8U)
#define IO_PORT_MISC2_OUT__SWITCHOFFUV_VDD5_GET() \
	((io0x202 & 0x0200u) >> 9U)
#define IO_PORT_MISC2_OUT__ENABLE_TX_GET() \
	((io0x202 & 0x0400u) >> 10U)
#define IO_PORT_MISC2_OUT__EN_IN_AAIN_GET() \
	((io0x202 & 0x0800u) >> 11U)
#define IO_PORT_STOPMD_CTRL__SEL_STOP_MODE_GET() \
	(io0x204 >> 0U)
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFUV_VDDA_GET() \
	((io0x206 & 0x0001u) >> 0U)
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFOV_VDDA_GET() \
	((io0x206 & 0x0002u) >> 1U)
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFOV_VDDD_GET() \
	((io0x206 & 0x0004u) >> 2U)
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFUV_VS_GET() \
	((io0x206 & 0x0008u) >> 3U)
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFOV_VS_GET() \
	((io0x206 & 0x0010u) >> 4U)
#define IO_PORT_STOPMD_CFG__PRE_SBY_RCO32M_GET() \
	((io0x206 & 0x0020u) >> 5U)
#define IO_PORT_STOPMD_CFG__PRE_SBY_RCO1M_GET() \
	((io0x206 & 0x0040u) >> 6U)
#define IO_PORT_STOPMD_CFG__STOPMD_CFG_ANA_GET() \
	((io0x206 & 0x0780u) >> 7U)
#define IO_PORT_DIS_GTSM__DIS_GTSM_GET() \
	(io0x208 >> 0U)
#define IO_PORT_CLOCK_CTRL__AC_SEL_GET() \
	(io0x20a >> 0U)
#define IO_UART__BRRD_GET() \
	(io0x20c >> 0U)
#define IO_UART__TRD_GET() \
	(io0x20e >> 0U)
#define IO_UART__RRD_GET() \
	(io0x210 >> 0U)
#define IO_UART__LFCD_GET() \
	((io0x212 & 0x00ffu) >> 0U)
#define IO_UART__LFDD_GET() \
	((io0x212 & 0xff00u) >> 8U)
#define IO_UART__TRE_GET() \
	((io0x214 & 0x0002u) >> 1U)
#define IO_UART__REE_GET() \
	((io0x214 & 0x0004u) >> 2U)
#define IO_UART__ISB_GET() \
	((io0x214 & 0x0008u) >> 3U)
#define IO_UART__LBE_GET() \
	((io0x214 & 0x0020u) >> 5U)
#define IO_UART__LSE_GET() \
	((io0x214 & 0x0040u) >> 6U)
#define IO_UART__LTE_GET() \
	((io0x214 & 0x0080u) >> 7U)
#define IO_UART__MLS_GET() \
	((io0x214 & 0x0700u) >> 8U)
#define IO_UART__BSC_GET() \
	((io0x214 & 0x1800u) >> 11U)
#define IO_UART__LBC_GET() \
	((io0x214 & 0x2000u) >> 13U)
#define IO_UART__LSC_GET() \
	((io0x214 & 0x4000u) >> 14U)
#define IO_UART__LDC_GET() \
	((io0x214 & 0x8000u) >> 15U)
#define IO_UART__TRO_GET() \
	((io0x216 & 0x0100u) >> 8U)
#define IO_UART__TRB_GET() \
	((io0x216 & 0x0200u) >> 9U)
#define IO_UART__TSB_GET() \
	((io0x216 & 0x0400u) >> 10U)
#define IO_UART__RSO_GET() \
	((io0x216 & 0x0800u) >> 11U)
#define IO_UART__RSB_GET() \
	((io0x216 & 0x1000u) >> 12U)
#define IO_UART__RRF_GET() \
	((io0x216 & 0x2000u) >> 13U)
#define IO_UART__NBR_GET() \
	((io0x216 & 0x4000u) >> 14U)
#define IO_UART__SBE_GET() \
	((io0x216 & 0x8000u) >> 15U)
#define IO_PORT_ID__ID_GET() \
	((io0x218 & 0x003fu) >> 0U)
#define IO_PORT_ID__IDMASK_GET() \
	((io0x218 & 0x0fc0u) >> 6U)
#define IO_PORT_DMA_RDA__DMA_RDA_GET() \
	(io0x21a >> 0U)
#define IO_PORT_DMA_RDB__DMA_RDB_GET() \
	(io0x21c >> 0U)
#define IO_PORT_DMA_TX__DMA_TX_GET() \
	(io0x21e >> 0U)
#define IO_PORT_MELIBUS_CTRL__MELIBUS_EN_GET() \
	((io0x220 & 0x0001u) >> 0U)
#define IO_PORT_MELIBUS_CTRL__MELIBUS_ACK_EN_GET() \
	((io0x220 & 0x0002u) >> 1U)
#define IO_PORT_MELIBUS_CTRL__MELIBUS_DIS_PARITY_GET() \
	((io0x220 & 0x0004u) >> 2U)
#define IO_PORT_MELIBUS_CTRL__MELIBUS_DIS_CRC_GET() \
	((io0x220 & 0x0008u) >> 3U)
#define IO_PORT_MELIBUS_CTRL__SEL_TXD_GET() \
	((io0x220 & 0x0030u) >> 4U)
#define IO_PORT_MELIBUS_CTRL__SEL_RX_IN_GET() \
	((io0x220 & 0x00c0u) >> 6U)
#define IO_PORT_MELIBUS_STAT__RD_BUFFER_VALID_GET() \
	((io0x222 & 0x0001u) >> 0U)
#define IO_PORT_MELIBUS_STAT__FRAME_RECEIVED_GET() \
	((io0x222 & 0x0002u) >> 1U)
#define IO_PORT_MELIBUS_STAT__FRAME_TRANSMIT_RDY_GET() \
	((io0x222 & 0x0004u) >> 2U)
#define IO_PORT_MELIBUS_STAT__PARITY_ERROR_GET() \
	((io0x222 & 0x0008u) >> 3U)
#define IO_PORT_MELIBUS_STAT__CRC_ERROR_GET() \
	((io0x222 & 0x0010u) >> 4U)
#define IO_PORT_MELIBUS_STAT__WRONG_BREAK_GET() \
	((io0x222 & 0x0020u) >> 5U)
#define IO_PORT_MELIBUS_STAT__BREAK_IN_FRAME_GET() \
	((io0x222 & 0x0040u) >> 6U)
#define IO_PORT_MELIBUS_STAT__TX_TIMEOUT_GET() \
	((io0x222 & 0x0080u) >> 7U)
#define IO_PORT_LC_SEL1__LC0_SEL_GET() \
	((io0x224 & 0x000fu) >> 0U)
#define IO_PORT_LC_SEL1__LC1_SEL_GET() \
	((io0x224 & 0x00f0u) >> 4U)
#define IO_PORT_LC_SEL1__LC2_SEL_GET() \
	((io0x224 & 0x0f00u) >> 8U)
#define IO_PORT_LC_SEL1__LC3_SEL_GET() \
	((io0x224 & 0xf000u) >> 12U)
#define IO_PORT_LC_SEL2__LC4_SEL_GET() \
	((io0x226 & 0x000fu) >> 0U)
#define IO_PORT_LC_SEL2__LC5_SEL_GET() \
	((io0x226 & 0x00f0u) >> 4U)
#define IO_PORT_LC_SEL2__LC6_SEL_GET() \
	((io0x226 & 0x0f00u) >> 8U)
#define IO_PORT_LC_SEL2__LC7_SEL_GET() \
	((io0x226 & 0xf000u) >> 12U)
#define IO_PORT_LC_SEL3__LC8_SEL_GET() \
	((io0x228 & 0x000fu) >> 0U)
#define IO_PORT_LC_SEL3__LC9_SEL_GET() \
	((io0x228 & 0x00f0u) >> 4U)
#define IO_PORT_LC_SEL3__LC10_SEL_GET() \
	((io0x228 & 0x0f00u) >> 8U)
#define IO_PORT_LC_SEL3__LC11_SEL_GET() \
	((io0x228 & 0xf000u) >> 12U)
#define IO_TRIM1_LC__LC0_TRIM_GET() \
	((io0x22a & 0x003fu) >> 0U)
#define IO_TRIM1_LC__LC1_TRIM_GET() \
	((io0x22a & 0x0fc0u) >> 6U)
#define IO_TRIM1_LC__LOCK_GET() \
	((io0x22a & 0x8000u) >> 15U)
#define IO_TRIM2_LC__LC2_TRIM_GET() \
	((io0x22c & 0x003fu) >> 0U)
#define IO_TRIM2_LC__LC3_TRIM_GET() \
	((io0x22c & 0x0fc0u) >> 6U)
#define IO_TRIM2_LC__LOCK_GET() \
	((io0x22c & 0x8000u) >> 15U)
#define IO_TRIM3_LC__LC4_TRIM_GET() \
	((io0x22e & 0x003fu) >> 0U)
#define IO_TRIM3_LC__LC5_TRIM_GET() \
	((io0x22e & 0x0fc0u) >> 6U)
#define IO_TRIM3_LC__LOCK_GET() \
	((io0x22e & 0x8000u) >> 15U)
#define IO_TRIM4_LC__LC6_TRIM_GET() \
	((io0x230 & 0x003fu) >> 0U)
#define IO_TRIM4_LC__LC7_TRIM_GET() \
	((io0x230 & 0x0fc0u) >> 6U)
#define IO_TRIM4_LC__LOCK_GET() \
	((io0x230 & 0x8000u) >> 15U)
#define IO_TRIM5_LC__LC8_TRIM_GET() \
	((io0x232 & 0x003fu) >> 0U)
#define IO_TRIM5_LC__LC9_TRIM_GET() \
	((io0x232 & 0x0fc0u) >> 6U)
#define IO_TRIM5_LC__LOCK_GET() \
	((io0x232 & 0x8000u) >> 15U)
#define IO_TRIM6_LC__LC10_TRIM_GET() \
	((io0x234 & 0x003fu) >> 0U)
#define IO_TRIM6_LC__LC11_TRIM_GET() \
	((io0x234 & 0x0fc0u) >> 6U)
#define IO_TRIM6_LC__LOCK_GET() \
	((io0x234 & 0x8000u) >> 15U)
#define IO_PORT_LC_DISPU_DIAG__LC_DISPU_GET() \
	((io0x236 & 0x0fffu) >> 0U)
#define IO_PORT_LC_DISPU_DIAG__LCDIAG_SEL_GET() \
	((io0x236 & 0x7000u) >> 12U)
#define IO_PORT_LC_OUT__LC_OUT_GET() \
	(io0x238 >> 0U)
#define IO_PORT_SELP_LC__SELP_LC_GET() \
	(io0x23a >> 0U)
#define IO_PORT_HSW_OUT__HSW_OUT_GET() \
	(io0x23c >> 0U)
#define IO_PORT_HSW_ENPD__HSW_ENPD_GET() \
	(io0x23e >> 0U)
#define IO_TRIM_MISC__TR_TAU_GET() \
	((io0x240 & 0x0001u) >> 0U)
#define IO_TRIM_MISC__TR_HYS_GET() \
	((io0x240 & 0x000eu) >> 1U)
#define IO_TRIM_MISC__LCDIAG_TRIM_GET() \
	((io0x240 & 0x03f0u) >> 4U)
#define IO_TRIM_MISC__TR_OVT_GET() \
	((io0x240 & 0x1c00u) >> 10U)
#define IO_TRIM_MISC__LOCK_GET() \
	((io0x240 & 0x8000u) >> 15U)
#define IO_PORT_MISC_TEST__LC_DIS_GET() \
	((io0x242 & 0x0001u) >> 0U)
#define IO_PORT_MISC_TEST__HSW_DIS_GET() \
	((io0x242 & 0x0002u) >> 1U)
#define IO_PORT_MISC_TEST__IDDQ_LC_GET() \
	((io0x242 & 0x0004u) >> 2U)
#define IO_PORT_MISC_TEST__TEST_HVDIFF_EN_GET() \
	((io0x242 & 0x0008u) >> 3U)
#define IO_PORT_MISC_TEST__DIS_TX_TIMEOUT_GET() \
	((io0x242 & 0x0010u) >> 4U)
#define IO_PORT_AA_OUT__PRE_AA_OUT_GET() \
	(io0x244 >> 0U)
#define IO_PORT_OUTB_AAIN__OUTB_AAIN_GET() \
	(io0x246 >> 0U)
#define IO_PORT_AA_SEL__SEL_AA_OUT_GET() \
	(io0x248 >> 0U)
#define IO_PORT_AA_IN__AA_IN_SYNC_GET() \
	(io0x24a >> 0U)
#define IO_PORT_LIN_XCFG__LIN_XCFG_GET() \
	(io0x24c >> 0U)
#define IO_PORT_LIN_XCFG_VALID__LIN_XCFG_VALID_GET() \
	(io0x24e >> 0U)
#define IO_TRIM_VDD_EXT__TR_VDDIO_GET() \
	((io0x250 & 0x0007u) >> 0U)
#define IO_TRIM_VDD_EXT__LOCK_GET() \
	((io0x250 & 0x8000u) >> 15U)
#define IO_PORT_TEST_VDD_EXT__SWITCHOFFREG_VDDIO_GET() \
	((io0x252 & 0x0003u) >> 0U)
#define IO_PORT_TEST_VDD_EXT__SHOVE_VDDIO_GET() \
	((io0x252 & 0x0004u) >> 2U)
#define IO_PORT_TEST_VDD_EXT__IDDQ_REG_VDDIO_GET() \
	((io0x252 & 0x0008u) >> 3U)

/*****************************************************************************/
/* 3.3 Ports. */

/* 
 * Description of the records from left to right:
 * - Block name, double underscore, field name.
 * - Hosting word variable (or undefined 16-bit variable to catch word access if it not possible).
 * - Hosting byte variable (or undefined 8-bit variable to catch byte access if it not possible).
 * - Absolute word address of the field. Also location of the host variable.
 * - Absolute byte address of the field (or 0xFFFF if byte access is not possible).
 * - Bit mask within word, showing the meaningful bits of the port inside the host withing the word
 * - Bit mask within byte, showing the meaningful bits of the port inside the host withing the byte
 * - Offset within word, shows the location of the first significant bit of the mask within the word
 * - Offset within byte, shows the location of the first significant bit of the mask within the byte
 * - Boolean true (1U) or false (0U), showing whether the port is writable.
 * - Triboolean, showing whether the port is bit accessible i.e. the assembler
 *    operation setb, clrb and mov c can be used for it. The value 2U is
 *    reserved for MlxCCT's private dummy ports.
 * - Bit mask, showing which bits of the host variable are not occupied.
 * - Bit mask, showing how the host variable is occupied by rise-only fields.
 */

#define IO_RST_CTRL__AWD_WBOOT \
	io0x2, io_byte_0x2, 0x0002u, 0x0002, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x1fe0u, 0x801fu
#define IO_RST_CTRL__SOFT_WBOOT \
	io0x2, io_byte_0x2, 0x0002u, 0x0002, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x1fe0u, 0x801fu
#define IO_RST_CTRL__HVDIG_WBOOT \
	io0x2, io_byte_0x2, 0x0002u, 0x0002, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x1fe0u, 0x801fu
#define IO_RST_CTRL__DBG_WBOOT \
	io0x2, io_byte_0x2, 0x0002u, 0x0002, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x1fe0u, 0x801fu
#define IO_RST_CTRL__IWD_WBOOT \
	io0x2, io_byte_0x2, 0x0002u, 0x0002, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x1fe0u, 0x801fu
#define IO_RST_CTRL__SOFT_RESET \
	io0x2, io_byte_0x3, 0x0002u, 0x0003, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x1fe0u, 0x801fu
#define IO_RST_CTRL__HVDIG_USED \
	io0x2, io_byte_0x3, 0x0002u, 0x0003, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x1fe0u, 0x801fu
#define IO_RST_CTRL__HVDIG_OK \
	io0x2, io_byte_0x3, 0x0002u, 0x0003, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x1fe0u, 0x801fu
#define IO_FUNC_TEST__DISABLE_RESET \
	io0x6, io_byte_0x6, 0x0006u, 0x0006, 0x0001u, 0x01u,  0U, 0, 1U, 28u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__DISABLE_SUSPEND \
	io0x6, io_byte_0x6, 0x0006u, 0x0006, 0x0002u, 0x02u,  1U, 1, 1U, 28u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__DISABLE_IRQ \
	io0x6, io_byte_0x6, 0x0006u, 0x0006, 0x0004u, 0x04u,  2U, 2, 1U, 28u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__FTC_SEL \
	io0x6, io_byte_0x7, 0x0006u, 0x0007, 0x0300u, 0x03u,  8U, 0, 1U, 28u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__FTC_DIV \
	io0x6, io_byte_0x7, 0x0006u, 0x0007, 0x3c00u, 0x3cu, 10U, 2, 1U, 28u, 0xc0f8u, 0x0000u
#define IO_FUNC_TEST__TEST_UNPROT \
	io0x8, error_IO_FUNC_TEST__TEST_UNPROT_has_no_byte_access, 0x0008u, 0x0008, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_MUPET__PTC_RECEIVE \
	io0xa, error_IO_MUPET__PTC_RECEIVE_has_no_byte_access, 0x000au, 0x000a, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_MUPET__PTC_SEND \
	io0xa, error_IO_MUPET__PTC_SEND_has_no_byte_access, 0x000au, 0x000a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MUPET__PTCA_PEND \
	io0xc, io_byte_0xc, 0x000cu, 0x000c, 0x0001u, 0x01u,  0U, 0, 0U, 12u, 0x0078u, 0x8000u
#define IO_MUPET__PTCS_PEND \
	io0xc, io_byte_0xc, 0x000cu, 0x000c, 0x0002u, 0x02u,  1U, 1, 0U, 12u, 0x0078u, 0x8000u
#define IO_MUPET__PTCR_PEND \
	io0xc, io_byte_0xc, 0x000cu, 0x000c, 0x0004u, 0x04u,  2U, 2, 0U, 12u, 0x0078u, 0x8000u
#define IO_MUPET__PTC_KEY \
	io0xc, io_byte_0xc, 0x000cu, 0x000c, 0x0080u, 0x80u,  7U, 7, 0U, 12u, 0x0078u, 0x8000u
#define IO_MUPET__WARM_TRIGGER \
	io0xc, io_byte_0xd, 0x000cu, 0x000d, 0x1f00u, 0x1fu,  8U, 0, 1U, 12u, 0x0078u, 0x8000u
#define IO_MUPET__CONNECTION \
	io0xc, io_byte_0xd, 0x000cu, 0x000d, 0x6000u, 0x60u, 13U, 5, 0U, 12u, 0x0078u, 0x8000u
#define IO_MUPET__IN_APPLICATION \
	io0xc, io_byte_0xd, 0x000cu, 0x000d, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0078u, 0x8000u
#define IO_AWD__ACK \
	error_IO_AWD__ACK_has_no_word_access, io_byte_0xe, 0x000eu, 0x000e, 0x0040u, 0x40u,  6U, 6, 1U, 7u, 0x7fbfu, 0x8040u
#define IO_AWD__WIN_OPEN \
	error_IO_AWD__WIN_OPEN_has_no_word_access, io_byte_0xf, 0x000eu, 0x000f, 0x8000u, 0x80u, 15U, 7, 1U, 7u, 0x7fbfu, 0x8040u
#define IO_ROM_SHELL__READY \
	io0x10, io_byte_0x10, 0x0010u, 0x0010, 0x001fu, 0x1fu,  0U, 0, 1U, 12u, 0xffe0u, 0x0000u
#define IO_ROM_BIST__ADD_START_L \
	io0x12, error_IO_ROM_BIST__ADD_START_L_has_no_byte_access, 0x0012u, 0x0012, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ROM_BIST__ADD_START_H \
	io0x14, io_byte_0x14, 0x0014u, 0x0014, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x000fu
#define IO_ROM_BIST__ADD_STOP_L \
	io0x16, error_IO_ROM_BIST__ADD_STOP_L_has_no_byte_access, 0x0016u, 0x0016, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ROM_BIST__ADD_STOP_H \
	io0x18, io_byte_0x18, 0x0018u, 0x0018, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x000fu
#define IO_ROM_BIST__SIG_EXPECTED_L \
	io0x1a, error_IO_ROM_BIST__SIG_EXPECTED_L_has_no_byte_access, 0x001au, 0x001a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_ROM_BIST__SIG_EXPECTED_H \
	io0x1c, io_byte_0x1c, 0x001cu, 0x001c, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xff00u, 0x0000u
#define IO_ROM_BIST__ECC_POSITION \
	io0x1e, io_byte_0x1e, 0x001eu, 0x001e, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x758cu, 0x0000u
#define IO_ROM_BIST__BIST \
	io0x1e, io_byte_0x1e, 0x001eu, 0x001e, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x758cu, 0x0000u
#define IO_ROM_BIST__SINGLE_RAMP \
	io0x1e, io_byte_0x1e, 0x001eu, 0x001e, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x758cu, 0x0000u
#define IO_ROM_BIST__MASK_SIG_ERR \
	io0x1e, io_byte_0x1e, 0x001eu, 0x001e, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x758cu, 0x0000u
#define IO_ROM_BIST__BIST_REQUEST \
	io0x1e, io_byte_0x1f, 0x001eu, 0x001f, 0x0200u, 0x02u,  9U, 1, 0U, 12u, 0x758cu, 0x0000u
#define IO_ROM_BIST__VALID_CLOCK \
	io0x1e, io_byte_0x1f, 0x001eu, 0x001f, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0x758cu, 0x0000u
#define IO_ROM_BIST__COMPLETED \
	io0x1e, io_byte_0x1f, 0x001eu, 0x001f, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0x758cu, 0x0000u
#define IO_ROM_BIST__START_BIST \
	io0x20, error_IO_ROM_BIST__START_BIST_has_no_byte_access, 0x0020u, 0x0020, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ROM_BIST__SIG_RECEIVED_L \
	io0x22, error_IO_ROM_BIST__SIG_RECEIVED_L_has_no_byte_access, 0x0022u, 0x0022, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ROM_BIST__SIG_RECEIVED_H \
	io0x24, io_byte_0x24, 0x0024u, 0x0024, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xff00u, 0x00ffu
#define IO_IWD__WDT \
	io0x28, io_byte_0x28, 0x0028u, 0x0028, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xf800u, 0x0000u
#define IO_IWD__DIV \
	io0x28, io_byte_0x29, 0x0028u, 0x0029, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0xf800u, 0x0000u
#define IO_IWD__WTG \
	io0x2a, io_byte_0x2a, 0x002au, 0x002a, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0f00u, 0xf000u
#define IO_IWD__WIN_ENABLE \
	io0x2a, io_byte_0x2b, 0x002au, 0x002b, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0f00u, 0xf000u
#define IO_IWD__WIN_DISABLE \
	io0x2a, io_byte_0x2b, 0x002au, 0x002b, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0f00u, 0xf000u
#define IO_IWD__ATT_INT \
	io0x2a, io_byte_0x2b, 0x002au, 0x002b, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0f00u, 0xf000u
#define IO_IWD__WIN_OPEN \
	io0x2a, io_byte_0x2b, 0x002au, 0x002b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0f00u, 0xf000u
#define IO_STIMER__CURRENT \
	io0x2c, error_IO_STIMER__CURRENT_has_no_byte_access, 0x002cu, 0x002c, 0x3fffu, 0xffu,  0U, 0, 0U, 8u, 0xc000u, 0x0000u
#define IO_STIMER__VALUE \
	io0x2e, error_IO_STIMER__VALUE_has_no_byte_access, 0x002eu, 0x002e, 0x3fffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_STIMER__MODE \
	io0x2e, error_IO_STIMER__MODE_has_no_byte_access, 0x002eu, 0x002f, 0xc000u, 0xc0u, 14U, 6, 1U, 8u, 0x0000u, 0x0000u
#define IO_CTIMER0__TREGB \
	io0x30, error_IO_CTIMER0__TREGB_has_no_byte_access, 0x0030u, 0x0030, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CTIMER0__TREGA \
	io0x32, error_IO_CTIMER0__TREGA_has_no_byte_access, 0x0032u, 0x0032, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CTIMER0__TCNT \
	io0x34, error_IO_CTIMER0__TCNT_has_no_byte_access, 0x0034u, 0x0034, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_CTIMER0__START \
	io0x36, io_byte_0x36, 0x0036u, 0x0036, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__STOP \
	io0x36, io_byte_0x36, 0x0036u, 0x0036, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__EDGA \
	io0x36, io_byte_0x36, 0x0036u, 0x0036, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__EDGB \
	io0x36, io_byte_0x36, 0x0036u, 0x0036, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__PWMI \
	io0x36, io_byte_0x36, 0x0036u, 0x0036, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__POL \
	io0x36, io_byte_0x36, 0x0036u, 0x0036, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__OVRA \
	io0x36, io_byte_0x37, 0x0036u, 0x0037, 0x0100u, 0x01u,  8U, 0, 0U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__OVRB \
	io0x36, io_byte_0x37, 0x0036u, 0x0037, 0x0200u, 0x02u,  9U, 1, 0U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__ENCMP \
	io0x36, io_byte_0x37, 0x0036u, 0x0037, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__MODE \
	io0x36, io_byte_0x37, 0x0036u, 0x0037, 0x3800u, 0x38u, 11U, 3, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER0__DIV \
	io0x36, io_byte_0x37, 0x0036u, 0x0037, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0043u
#define IO_MLX16__DBG_DATA0 \
	io0x38, error_IO_MLX16__DBG_DATA0_has_no_byte_access, 0x0038u, 0x0038, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_DATA1 \
	io0x3a, error_IO_MLX16__DBG_DATA1_has_no_byte_access, 0x003au, 0x003a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_DATA2 \
	io0x3c, error_IO_MLX16__DBG_DATA2_has_no_byte_access, 0x003cu, 0x003c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_DATA3 \
	io0x3e, error_IO_MLX16__DBG_DATA3_has_no_byte_access, 0x003eu, 0x003e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_ADDRESS0 \
	io0x40, error_IO_MLX16__DBG_ADDRESS0_has_no_byte_access, 0x0040u, 0x0040, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_ADREXT0 \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_CONTROL0 \
	io0x42, error_IO_MLX16__DBG_CONTROL0_has_no_byte_access, 0x0042u, 0x0042, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_COND0 \
	io0x42, io_byte_0x42, 0x0042u, 0x0042, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_RANGE0 \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_MODE0 \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x0600u, 0x06u,  9U, 1, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_LOCK0 \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_MARKEXT0 \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_OVR0 \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_READY0 \
	io0x42, io_byte_0x43, 0x0042u, 0x0043, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_ADDRESS1 \
	io0x44, error_IO_MLX16__DBG_ADDRESS1_has_no_byte_access, 0x0044u, 0x0044, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_ADREXT1 \
	io0x46, io_byte_0x46, 0x0046u, 0x0046, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_CONTROL1 \
	io0x46, error_IO_MLX16__DBG_CONTROL1_has_no_byte_access, 0x0046u, 0x0046, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_COND1 \
	io0x46, io_byte_0x46, 0x0046u, 0x0046, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_RANGE1 \
	io0x46, io_byte_0x47, 0x0046u, 0x0047, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_MODE1 \
	io0x46, io_byte_0x47, 0x0046u, 0x0047, 0x0600u, 0x06u,  9U, 1, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_LOCK1 \
	io0x46, io_byte_0x47, 0x0046u, 0x0047, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_MARKEXT1 \
	io0x46, io_byte_0x47, 0x0046u, 0x0047, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_OVR1 \
	io0x46, io_byte_0x47, 0x0046u, 0x0047, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_READY1 \
	io0x46, io_byte_0x47, 0x0046u, 0x0047, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_ADDRESS2 \
	io0x48, error_IO_MLX16__DBG_ADDRESS2_has_no_byte_access, 0x0048u, 0x0048, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_ADREXT2 \
	io0x4a, io_byte_0x4a, 0x004au, 0x004a, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_CONTROL2 \
	io0x4a, error_IO_MLX16__DBG_CONTROL2_has_no_byte_access, 0x004au, 0x004a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_COND2 \
	io0x4a, io_byte_0x4a, 0x004au, 0x004a, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_RANGE2 \
	io0x4a, io_byte_0x4b, 0x004au, 0x004b, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_MODE2 \
	io0x4a, io_byte_0x4b, 0x004au, 0x004b, 0x0600u, 0x06u,  9U, 1, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_LOCK2 \
	io0x4a, io_byte_0x4b, 0x004au, 0x004b, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_MARKEXT2 \
	io0x4a, io_byte_0x4b, 0x004au, 0x004b, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_OVR2 \
	io0x4a, io_byte_0x4b, 0x004au, 0x004b, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_READY2 \
	io0x4a, io_byte_0x4b, 0x004au, 0x004b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_ADDRESS3 \
	io0x4c, error_IO_MLX16__DBG_ADDRESS3_has_no_byte_access, 0x004cu, 0x004c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_ADREXT3 \
	io0x4e, io_byte_0x4e, 0x004eu, 0x004e, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_CONTROL3 \
	io0x4e, error_IO_MLX16__DBG_CONTROL3_has_no_byte_access, 0x004eu, 0x004e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_COND3 \
	io0x4e, io_byte_0x4e, 0x004eu, 0x004e, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_RANGE3 \
	io0x4e, io_byte_0x4f, 0x004eu, 0x004f, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_MODE3 \
	io0x4e, io_byte_0x4f, 0x004eu, 0x004f, 0x0600u, 0x06u,  9U, 1, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_LOCK3 \
	io0x4e, io_byte_0x4f, 0x004eu, 0x004f, 0x0800u, 0x08u, 11U, 3, 0U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_MARKEXT3 \
	io0x4e, io_byte_0x4f, 0x004eu, 0x004f, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_OVR3 \
	io0x4e, io_byte_0x4f, 0x004eu, 0x004f, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__DBG_READY3 \
	io0x4e, io_byte_0x4f, 0x004eu, 0x004f, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PEND0 \
	io0x50, error_IO_MLX16__ITC_PEND0_has_no_byte_access, 0x0050u, 0x0050, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_MLX16__MLX16_EXCHG_PEND \
	io0x50, io_byte_0x50, 0x0050u, 0x0050, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__MLX16_DMAERR_PEND \
	io0x50, io_byte_0x50, 0x0050u, 0x0050, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__AWD_ATT_PEND \
	io0x50, io_byte_0x50, 0x0050u, 0x0050, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__IWD_ATT_PEND \
	io0x50, io_byte_0x50, 0x0050u, 0x0050, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__EE_ECC_PEND \
	io0x50, io_byte_0x50, 0x0050u, 0x0050, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__FL_ECC_PEND \
	io0x50, io_byte_0x50, 0x0050u, 0x0050, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UV_VDDA_PEND \
	io0x50, io_byte_0x50, 0x0050u, 0x0050, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UV_VS_PEND \
	io0x50, io_byte_0x50, 0x0050u, 0x0050, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__OVT_PEND \
	io0x50, io_byte_0x51, 0x0050u, 0x0051, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__STIMER_PEND \
	io0x50, io_byte_0x51, 0x0050u, 0x0051, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__CTIMER0_1_PEND \
	io0x50, io_byte_0x51, 0x0050u, 0x0051, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__CTIMER0_2_PEND \
	io0x50, io_byte_0x51, 0x0050u, 0x0051, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__CTIMER0_3_PEND \
	io0x50, io_byte_0x51, 0x0050u, 0x0051, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__CTIMER1_1_PEND \
	io0x50, io_byte_0x51, 0x0050u, 0x0051, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__CTIMER1_2_PEND \
	io0x50, io_byte_0x51, 0x0050u, 0x0051, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__CTIMER1_3_PEND \
	io0x50, io_byte_0x51, 0x0050u, 0x0051, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__ITC_PEND1 \
	io0x52, error_IO_MLX16__ITC_PEND1_has_no_byte_access, 0x0052u, 0x0052, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_MASTER1_CMP_PEND \
	io0x52, io_byte_0x52, 0x0052u, 0x0052, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_MASTER1_END_PEND \
	io0x52, io_byte_0x52, 0x0052u, 0x0052, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE1_CMP_PEND \
	io0x52, io_byte_0x52, 0x0052u, 0x0052, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE2_CMP_PEND \
	io0x52, io_byte_0x52, 0x0052u, 0x0052, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE3_CMP_PEND \
	io0x52, io_byte_0x52, 0x0052u, 0x0052, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE4_CMP_PEND \
	io0x52, io_byte_0x52, 0x0052u, 0x0052, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE5_CMP_PEND \
	io0x52, io_byte_0x52, 0x0052u, 0x0052, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE6_CMP_PEND \
	io0x52, io_byte_0x52, 0x0052u, 0x0052, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE7_CMP_PEND \
	io0x52, io_byte_0x53, 0x0052u, 0x0053, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE8_CMP_PEND \
	io0x52, io_byte_0x53, 0x0052u, 0x0053, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE9_CMP_PEND \
	io0x52, io_byte_0x53, 0x0052u, 0x0053, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE10_CMP_PEND \
	io0x52, io_byte_0x53, 0x0052u, 0x0053, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__PWM_SLAVE11_CMP_PEND \
	io0x52, io_byte_0x53, 0x0052u, 0x0053, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__ADC_SAR_PEND \
	io0x52, io_byte_0x53, 0x0052u, 0x0053, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__EE_COMPLETE_PEND \
	io0x52, io_byte_0x53, 0x0052u, 0x0053, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__FL_COMPLETE_PEND \
	io0x52, io_byte_0x53, 0x0052u, 0x0053, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__ITC_PEND2 \
	io0x54, error_IO_MLX16__ITC_PEND2_has_no_byte_access, 0x0054u, 0x0054, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_MLX16__OV_VS_PEND \
	io0x54, io_byte_0x54, 0x0054u, 0x0054, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__OVT_WARN_PEND \
	io0x54, io_byte_0x54, 0x0054u, 0x0054, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UV_VDD5_PEND \
	io0x54, io_byte_0x54, 0x0054u, 0x0054, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__AA_IN_FE_PEND \
	io0x54, io_byte_0x54, 0x0054u, 0x0054, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UART_SB_PEND \
	io0x54, io_byte_0x54, 0x0054u, 0x0054, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UART_RS_PEND \
	io0x54, io_byte_0x54, 0x0054u, 0x0054, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UART_RR_PEND \
	io0x54, io_byte_0x54, 0x0054u, 0x0054, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UART_TS_PEND \
	io0x54, io_byte_0x54, 0x0054u, 0x0054, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UART_TR_PEND \
	io0x54, io_byte_0x55, 0x0054u, 0x0055, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__UART_TE_PEND \
	io0x54, io_byte_0x55, 0x0054u, 0x0055, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__MFT_PEND \
	io0x54, io_byte_0x55, 0x0054u, 0x0055, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__MFR_PEND \
	io0x54, io_byte_0x55, 0x0054u, 0x0055, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__MHR_PEND \
	io0x54, io_byte_0x55, 0x0054u, 0x0055, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__MER_PEND \
	io0x54, io_byte_0x55, 0x0054u, 0x0055, 0x2000u, 0x20u, 13U, 5, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__TX_TIMEOUT_PEND \
	io0x54, io_byte_0x55, 0x0054u, 0x0055, 0x4000u, 0x40u, 14U, 6, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__MLX16_SOFT_PEND \
	io0x54, io_byte_0x55, 0x0054u, 0x0055, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0xffffu
#define IO_MLX16__SWI \
	io0x6a, io_byte_0x6b, 0x006au, 0x006b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7fffu, 0x0000u
#define IO_MLX16__ITC_MASK0 \
	io0x6c, error_IO_MLX16__ITC_MASK0_has_no_byte_access, 0x006cu, 0x006c, 0xffffu, 0xffu,  0U, 0, 1U, 10u, 0x0000u, 0x0000u
#define IO_MLX16__MLX16_EXCHG_ITC \
	io0x6c, io_byte_0x6c, 0x006cu, 0x006c, 0x0001u, 0x01u,  0U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__MLX16_DMAERR_ITC \
	io0x6c, io_byte_0x6c, 0x006cu, 0x006c, 0x0002u, 0x02u,  1U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__AWD_ATT_ITC \
	io0x6c, io_byte_0x6c, 0x006cu, 0x006c, 0x0004u, 0x04u,  2U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__IWD_ATT_ITC \
	io0x6c, io_byte_0x6c, 0x006cu, 0x006c, 0x0008u, 0x08u,  3U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__EE_ECC_ITC \
	io0x6c, io_byte_0x6c, 0x006cu, 0x006c, 0x0010u, 0x10u,  4U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__FL_ECC_ITC \
	io0x6c, io_byte_0x6c, 0x006cu, 0x006c, 0x0020u, 0x20u,  5U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UV_VDDA_ITC \
	io0x6c, io_byte_0x6c, 0x006cu, 0x006c, 0x0040u, 0x40u,  6U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UV_VS_ITC \
	io0x6c, io_byte_0x6c, 0x006cu, 0x006c, 0x0080u, 0x80u,  7U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__OVT_ITC \
	io0x6c, io_byte_0x6d, 0x006cu, 0x006d, 0x0100u, 0x01u,  8U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__STIMER_ITC \
	io0x6c, io_byte_0x6d, 0x006cu, 0x006d, 0x0200u, 0x02u,  9U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER0_1_ITC \
	io0x6c, io_byte_0x6d, 0x006cu, 0x006d, 0x0400u, 0x04u, 10U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER0_2_ITC \
	io0x6c, io_byte_0x6d, 0x006cu, 0x006d, 0x0800u, 0x08u, 11U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER0_3_ITC \
	io0x6c, io_byte_0x6d, 0x006cu, 0x006d, 0x1000u, 0x10u, 12U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER1_1_ITC \
	io0x6c, io_byte_0x6d, 0x006cu, 0x006d, 0x2000u, 0x20u, 13U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER1_2_ITC \
	io0x6c, io_byte_0x6d, 0x006cu, 0x006d, 0x4000u, 0x40u, 14U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER1_3_ITC \
	io0x6c, io_byte_0x6d, 0x006cu, 0x006d, 0x8000u, 0x80u, 15U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_MASK1 \
	io0x6e, error_IO_MLX16__ITC_MASK1_has_no_byte_access, 0x006eu, 0x006e, 0xffffu, 0xffu,  0U, 0, 1U, 10u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_MASTER1_CMP_ITC \
	io0x6e, io_byte_0x6e, 0x006eu, 0x006e, 0x0001u, 0x01u,  0U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_MASTER1_END_ITC \
	io0x6e, io_byte_0x6e, 0x006eu, 0x006e, 0x0002u, 0x02u,  1U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE1_CMP_ITC \
	io0x6e, io_byte_0x6e, 0x006eu, 0x006e, 0x0004u, 0x04u,  2U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE2_CMP_ITC \
	io0x6e, io_byte_0x6e, 0x006eu, 0x006e, 0x0008u, 0x08u,  3U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE3_CMP_ITC \
	io0x6e, io_byte_0x6e, 0x006eu, 0x006e, 0x0010u, 0x10u,  4U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE4_CMP_ITC \
	io0x6e, io_byte_0x6e, 0x006eu, 0x006e, 0x0020u, 0x20u,  5U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE5_CMP_ITC \
	io0x6e, io_byte_0x6e, 0x006eu, 0x006e, 0x0040u, 0x40u,  6U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE6_CMP_ITC \
	io0x6e, io_byte_0x6e, 0x006eu, 0x006e, 0x0080u, 0x80u,  7U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE7_CMP_ITC \
	io0x6e, io_byte_0x6f, 0x006eu, 0x006f, 0x0100u, 0x01u,  8U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE8_CMP_ITC \
	io0x6e, io_byte_0x6f, 0x006eu, 0x006f, 0x0200u, 0x02u,  9U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE9_CMP_ITC \
	io0x6e, io_byte_0x6f, 0x006eu, 0x006f, 0x0400u, 0x04u, 10U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE10_CMP_ITC \
	io0x6e, io_byte_0x6f, 0x006eu, 0x006f, 0x0800u, 0x08u, 11U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE11_CMP_ITC \
	io0x6e, io_byte_0x6f, 0x006eu, 0x006f, 0x1000u, 0x10u, 12U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__ADC_SAR_ITC \
	io0x6e, io_byte_0x6f, 0x006eu, 0x006f, 0x2000u, 0x20u, 13U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__EE_COMPLETE_ITC \
	io0x6e, io_byte_0x6f, 0x006eu, 0x006f, 0x4000u, 0x40u, 14U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__FL_COMPLETE_ITC \
	io0x6e, io_byte_0x6f, 0x006eu, 0x006f, 0x8000u, 0x80u, 15U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_MASK2 \
	io0x70, error_IO_MLX16__ITC_MASK2_has_no_byte_access, 0x0070u, 0x0070, 0xffffu, 0xffu,  0U, 0, 1U, 10u, 0x0000u, 0x0000u
#define IO_MLX16__OV_VS_ITC \
	io0x70, io_byte_0x70, 0x0070u, 0x0070, 0x0001u, 0x01u,  0U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__OVT_WARN_ITC \
	io0x70, io_byte_0x70, 0x0070u, 0x0070, 0x0002u, 0x02u,  1U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UV_VDD5_ITC \
	io0x70, io_byte_0x70, 0x0070u, 0x0070, 0x0004u, 0x04u,  2U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__AA_IN_FE_ITC \
	io0x70, io_byte_0x70, 0x0070u, 0x0070, 0x0008u, 0x08u,  3U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UART_SB_ITC \
	io0x70, io_byte_0x70, 0x0070u, 0x0070, 0x0010u, 0x10u,  4U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UART_RS_ITC \
	io0x70, io_byte_0x70, 0x0070u, 0x0070, 0x0020u, 0x20u,  5U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UART_RR_ITC \
	io0x70, io_byte_0x70, 0x0070u, 0x0070, 0x0040u, 0x40u,  6U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UART_TS_ITC \
	io0x70, io_byte_0x70, 0x0070u, 0x0070, 0x0080u, 0x80u,  7U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UART_TR_ITC \
	io0x70, io_byte_0x71, 0x0070u, 0x0071, 0x0100u, 0x01u,  8U, 0, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__UART_TE_ITC \
	io0x70, io_byte_0x71, 0x0070u, 0x0071, 0x0200u, 0x02u,  9U, 1, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__MFT_ITC \
	io0x70, io_byte_0x71, 0x0070u, 0x0071, 0x0400u, 0x04u, 10U, 2, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__MFR_ITC \
	io0x70, io_byte_0x71, 0x0070u, 0x0071, 0x0800u, 0x08u, 11U, 3, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__MHR_ITC \
	io0x70, io_byte_0x71, 0x0070u, 0x0071, 0x1000u, 0x10u, 12U, 4, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__MER_ITC \
	io0x70, io_byte_0x71, 0x0070u, 0x0071, 0x2000u, 0x20u, 13U, 5, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__TX_TIMEOUT_ITC \
	io0x70, io_byte_0x71, 0x0070u, 0x0071, 0x4000u, 0x40u, 14U, 6, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__MLX16_SOFT_ITC \
	io0x70, io_byte_0x71, 0x0070u, 0x0071, 0x8000u, 0x80u, 15U, 7, 1U, 14u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO0 \
	io0x88, error_IO_MLX16__ITC_PRIO0_has_no_byte_access, 0x0088u, 0x0088, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__STIMER_PRIO \
	io0x88, io_byte_0x88, 0x0088u, 0x0088, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER0_1_PRIO \
	io0x88, io_byte_0x88, 0x0088u, 0x0088, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER0_2_PRIO \
	io0x88, io_byte_0x88, 0x0088u, 0x0088, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER0_3_PRIO \
	io0x88, io_byte_0x88, 0x0088u, 0x0088, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER1_1_PRIO \
	io0x88, io_byte_0x89, 0x0088u, 0x0089, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER1_2_PRIO \
	io0x88, io_byte_0x89, 0x0088u, 0x0089, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__CTIMER1_3_PRIO \
	io0x88, io_byte_0x89, 0x0088u, 0x0089, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_MASTER1_CMP_PRIO \
	io0x88, io_byte_0x89, 0x0088u, 0x0089, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO1 \
	io0x8a, error_IO_MLX16__ITC_PRIO1_has_no_byte_access, 0x008au, 0x008a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_MASTER1_END_PRIO \
	io0x8a, io_byte_0x8a, 0x008au, 0x008a, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE1_CMP_PRIO \
	io0x8a, io_byte_0x8a, 0x008au, 0x008a, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE2_CMP_PRIO \
	io0x8a, io_byte_0x8a, 0x008au, 0x008a, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE3_CMP_PRIO \
	io0x8a, io_byte_0x8a, 0x008au, 0x008a, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE4_CMP_PRIO \
	io0x8a, io_byte_0x8b, 0x008au, 0x008b, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE5_CMP_PRIO \
	io0x8a, io_byte_0x8b, 0x008au, 0x008b, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE6_CMP_PRIO \
	io0x8a, io_byte_0x8b, 0x008au, 0x008b, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE7_CMP_PRIO \
	io0x8a, io_byte_0x8b, 0x008au, 0x008b, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO2 \
	io0x8c, error_IO_MLX16__ITC_PRIO2_has_no_byte_access, 0x008cu, 0x008c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE8_CMP_PRIO \
	io0x8c, io_byte_0x8c, 0x008cu, 0x008c, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE9_CMP_PRIO \
	io0x8c, io_byte_0x8c, 0x008cu, 0x008c, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE10_CMP_PRIO \
	io0x8c, io_byte_0x8c, 0x008cu, 0x008c, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__PWM_SLAVE11_CMP_PRIO \
	io0x8c, io_byte_0x8c, 0x008cu, 0x008c, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ADC_SAR_PRIO \
	io0x8c, io_byte_0x8d, 0x008cu, 0x008d, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__EE_COMPLETE_PRIO \
	io0x8c, io_byte_0x8d, 0x008cu, 0x008d, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__FL_COMPLETE_PRIO \
	io0x8c, io_byte_0x8d, 0x008cu, 0x008d, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__OV_VS_PRIO \
	io0x8c, io_byte_0x8d, 0x008cu, 0x008d, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO3 \
	io0x8e, error_IO_MLX16__ITC_PRIO3_has_no_byte_access, 0x008eu, 0x008e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__OVT_WARN_PRIO \
	io0x8e, io_byte_0x8e, 0x008eu, 0x008e, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__UV_VDD5_PRIO \
	io0x8e, io_byte_0x8e, 0x008eu, 0x008e, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__AA_IN_FE_PRIO \
	io0x8e, io_byte_0x8e, 0x008eu, 0x008e, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__UART_SB_PRIO \
	io0x8e, io_byte_0x8e, 0x008eu, 0x008e, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__UART_RS_PRIO \
	io0x8e, io_byte_0x8f, 0x008eu, 0x008f, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__UART_RR_PRIO \
	io0x8e, io_byte_0x8f, 0x008eu, 0x008f, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__UART_TS_PRIO \
	io0x8e, io_byte_0x8f, 0x008eu, 0x008f, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__UART_TR_PRIO \
	io0x8e, io_byte_0x8f, 0x008eu, 0x008f, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0000u
#define IO_MLX16__ITC_PRIO4 \
	io0x90, error_IO_MLX16__ITC_PRIO4_has_no_byte_access, 0x0090u, 0x0090, 0x0fffu, 0xffu,  0U, 0, 1U, 8u, 0xf000u, 0x0000u
#define IO_MLX16__UART_TE_PRIO \
	io0x90, io_byte_0x90, 0x0090u, 0x0090, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_MLX16__MFT_PRIO \
	io0x90, io_byte_0x90, 0x0090u, 0x0090, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0xf000u, 0x0000u
#define IO_MLX16__MFR_PRIO \
	io0x90, io_byte_0x90, 0x0090u, 0x0090, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0xf000u, 0x0000u
#define IO_MLX16__MHR_PRIO \
	io0x90, io_byte_0x90, 0x0090u, 0x0090, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0xf000u, 0x0000u
#define IO_MLX16__MER_PRIO \
	io0x90, io_byte_0x91, 0x0090u, 0x0091, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_MLX16__TX_TIMEOUT_PRIO \
	io0x90, io_byte_0x91, 0x0090u, 0x0091, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0xf000u, 0x0000u
#define IO_MLX16__SHELL_VERSION \
	io0xbe, error_IO_MLX16__SHELL_VERSION_has_no_byte_access, 0x00beu, 0x00be, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_MLX16__CPU_FP0ADR \
	io0xc0, error_IO_MLX16__CPU_FP0ADR_has_no_byte_access, 0x00c0u, 0x00c0, 0x0fffu, 0xffu,  0U, 0, 1U, 8u, 0xf000u, 0x0000u
#define IO_VERSION__VERSION_L \
	io0x100, error_IO_VERSION__VERSION_L_has_no_byte_access, 0x0100u, 0x0100, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_VERSION__VERSION_H \
	io0x102, error_IO_VERSION__VERSION_H_has_no_byte_access, 0x0102u, 0x0102, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__KEY \
	io0x104, error_IO_RAM_BIST__KEY_has_no_byte_access, 0x0104u, 0x0104, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__PHASE \
	io0x104, error_IO_RAM_BIST__PHASE_has_no_byte_access, 0x0104u, 0x0104, 0x0007u, 0x07u,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__TRANSPARENT \
	io0x104, error_IO_RAM_BIST__TRANSPARENT_has_no_byte_access, 0x0104u, 0x0105, 0x0100u, 0x01u,  8U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__REGULAR \
	io0x104, error_IO_RAM_BIST__REGULAR_has_no_byte_access, 0x0104u, 0x0105, 0x0200u, 0x02u,  9U, 1, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__RUNNING \
	io0x104, error_IO_RAM_BIST__RUNNING_has_no_byte_access, 0x0104u, 0x0105, 0x0400u, 0x04u, 10U, 2, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__VALID_CLOCK \
	io0x104, error_IO_RAM_BIST__VALID_CLOCK_has_no_byte_access, 0x0104u, 0x0105, 0x0800u, 0x08u, 11U, 3, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__REGULAR_BIST_ERROR \
	io0x104, error_IO_RAM_BIST__REGULAR_BIST_ERROR_has_no_byte_access, 0x0104u, 0x0105, 0x1000u, 0x10u, 12U, 4, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__COMPLETED \
	io0x104, error_IO_RAM_BIST__COMPLETED_has_no_byte_access, 0x0104u, 0x0105, 0x8000u, 0x80u, 15U, 7, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__LFSR \
	io0x106, error_IO_RAM_BIST__LFSR_has_no_byte_access, 0x0106u, 0x0106, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__LSFR_GOT \
	io0x106, error_IO_RAM_BIST__LSFR_GOT_has_no_byte_access, 0x0106u, 0x0106, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__LFSR_EXPECTED \
	io0x108, error_IO_RAM_BIST__LFSR_EXPECTED_has_no_byte_access, 0x0108u, 0x0108, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__ADL \
	io0x10a, error_IO_RAM_BIST__ADL_has_no_byte_access, 0x010au, 0x010a, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__ADH \
	io0x10c, error_IO_RAM_BIST__ADH_has_no_byte_access, 0x010cu, 0x010c, 0x000fu, 0x0fu,  0U, 0, 0U, 8u, 0xfff0u, 0x0000u
#define IO_RAM_BIST__ADD_START_L \
	io0x10e, error_IO_RAM_BIST__ADD_START_L_has_no_byte_access, 0x010eu, 0x010e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__ADD_START_H \
	io0x110, io_byte_0x110, 0x0110u, 0x0110, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x0000u
#define IO_RAM_BIST__ADD_STOP_L \
	io0x112, error_IO_RAM_BIST__ADD_STOP_L_has_no_byte_access, 0x0112u, 0x0112, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_RAM_BIST__ADD_STOP_H \
	io0x114, io_byte_0x114, 0x0114u, 0x0114, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x0000u
#define IO_RAM_BIST__NB_ECC_BITS \
	io0x116, io_byte_0x116, 0x0116u, 0x0116, 0x001fu, 0x1fu,  0U, 0, 1U, 12u, 0xe880u, 0x0000u
#define IO_RAM_BIST__WORD_BIST \
	io0x116, io_byte_0x116, 0x0116u, 0x0116, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0xe880u, 0x0000u
#define IO_RAM_BIST__FUNCTIONAL_BIST \
	io0x116, io_byte_0x116, 0x0116u, 0x0116, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0xe880u, 0x0000u
#define IO_RAM_BIST__ADD_SCRAMBLE \
	io0x116, io_byte_0x117, 0x0116u, 0x0117, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0xe880u, 0x0000u
#define IO_RAM_BIST__SIGNATURE_INIT \
	io0x116, io_byte_0x117, 0x0116u, 0x0117, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0xe880u, 0x0000u
#define IO_CTIMER1__TREGB \
	io0x118, error_IO_CTIMER1__TREGB_has_no_byte_access, 0x0118u, 0x0118, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CTIMER1__TREGA \
	io0x11a, error_IO_CTIMER1__TREGA_has_no_byte_access, 0x011au, 0x011a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_CTIMER1__TCNT \
	io0x11c, error_IO_CTIMER1__TCNT_has_no_byte_access, 0x011cu, 0x011c, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_CTIMER1__START \
	io0x11e, io_byte_0x11e, 0x011eu, 0x011e, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__STOP \
	io0x11e, io_byte_0x11e, 0x011eu, 0x011e, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__EDGA \
	io0x11e, io_byte_0x11e, 0x011eu, 0x011e, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__EDGB \
	io0x11e, io_byte_0x11e, 0x011eu, 0x011e, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__PWMI \
	io0x11e, io_byte_0x11e, 0x011eu, 0x011e, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__POL \
	io0x11e, io_byte_0x11e, 0x011eu, 0x011e, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__OVRA \
	io0x11e, io_byte_0x11f, 0x011eu, 0x011f, 0x0100u, 0x01u,  8U, 0, 0U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__OVRB \
	io0x11e, io_byte_0x11f, 0x011eu, 0x011f, 0x0200u, 0x02u,  9U, 1, 0U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__ENCMP \
	io0x11e, io_byte_0x11f, 0x011eu, 0x011f, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__MODE \
	io0x11e, io_byte_0x11f, 0x011eu, 0x011f, 0x3800u, 0x38u, 11U, 3, 1U, 12u, 0x0000u, 0x0043u
#define IO_CTIMER1__DIV \
	io0x11e, io_byte_0x11f, 0x011eu, 0x011f, 0xc000u, 0xc0u, 14U, 6, 1U, 12u, 0x0000u, 0x0043u
#define IO_PWM_MASTER1__CMP \
	io0x120, error_IO_PWM_MASTER1__CMP_has_no_byte_access, 0x0120u, 0x0120, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_MASTER1__HT \
	io0x122, error_IO_PWM_MASTER1__HT_has_no_byte_access, 0x0122u, 0x0122, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_MASTER1__LT \
	io0x124, error_IO_PWM_MASTER1__LT_has_no_byte_access, 0x0124u, 0x0124, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_MASTER1__PER \
	io0x126, error_IO_PWM_MASTER1__PER_has_no_byte_access, 0x0126u, 0x0126, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_MASTER1__START \
	io0x128, io_byte_0x128, 0x0128u, 0x0128, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_MASTER1__STOP \
	io0x128, io_byte_0x128, 0x0128u, 0x0128, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_MASTER1__SLAVE \
	io0x128, io_byte_0x128, 0x0128u, 0x0128, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_MASTER1__MODE \
	io0x128, io_byte_0x128, 0x0128u, 0x0128, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_MASTER1__POL \
	io0x128, io_byte_0x128, 0x0128u, 0x0128, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_MASTER1__IDLE \
	io0x128, io_byte_0x128, 0x0128u, 0x0128, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_MASTER1__PWM_IN \
	io0x128, io_byte_0x128, 0x0128u, 0x0128, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_MASTER1__PSCLN \
	io0x128, io_byte_0x129, 0x0128u, 0x0129, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_MASTER1__PSCLM \
	io0x128, io_byte_0x129, 0x0128u, 0x0129, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__CMP \
	io0x12a, error_IO_PWM_SLAVE1__CMP_has_no_byte_access, 0x012au, 0x012a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE1__HT \
	io0x12c, error_IO_PWM_SLAVE1__HT_has_no_byte_access, 0x012cu, 0x012c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE1__LT \
	io0x12e, error_IO_PWM_SLAVE1__LT_has_no_byte_access, 0x012eu, 0x012e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE1__PER \
	io0x130, error_IO_PWM_SLAVE1__PER_has_no_byte_access, 0x0130u, 0x0130, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE1__START \
	io0x132, io_byte_0x132, 0x0132u, 0x0132, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__STOP \
	io0x132, io_byte_0x132, 0x0132u, 0x0132, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__SLAVE \
	io0x132, io_byte_0x132, 0x0132u, 0x0132, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__MODE \
	io0x132, io_byte_0x132, 0x0132u, 0x0132, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__POL \
	io0x132, io_byte_0x132, 0x0132u, 0x0132, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__IDLE \
	io0x132, io_byte_0x132, 0x0132u, 0x0132, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__PWM_IN \
	io0x132, io_byte_0x132, 0x0132u, 0x0132, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__PSCLN \
	io0x132, io_byte_0x133, 0x0132u, 0x0133, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE1__PSCLM \
	io0x132, io_byte_0x133, 0x0132u, 0x0133, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__CMP \
	io0x134, error_IO_PWM_SLAVE2__CMP_has_no_byte_access, 0x0134u, 0x0134, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE2__HT \
	io0x136, error_IO_PWM_SLAVE2__HT_has_no_byte_access, 0x0136u, 0x0136, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE2__LT \
	io0x138, error_IO_PWM_SLAVE2__LT_has_no_byte_access, 0x0138u, 0x0138, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE2__PER \
	io0x13a, error_IO_PWM_SLAVE2__PER_has_no_byte_access, 0x013au, 0x013a, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE2__START \
	io0x13c, io_byte_0x13c, 0x013cu, 0x013c, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__STOP \
	io0x13c, io_byte_0x13c, 0x013cu, 0x013c, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__SLAVE \
	io0x13c, io_byte_0x13c, 0x013cu, 0x013c, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__MODE \
	io0x13c, io_byte_0x13c, 0x013cu, 0x013c, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__POL \
	io0x13c, io_byte_0x13c, 0x013cu, 0x013c, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__IDLE \
	io0x13c, io_byte_0x13c, 0x013cu, 0x013c, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__PWM_IN \
	io0x13c, io_byte_0x13c, 0x013cu, 0x013c, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__PSCLN \
	io0x13c, io_byte_0x13d, 0x013cu, 0x013d, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE2__PSCLM \
	io0x13c, io_byte_0x13d, 0x013cu, 0x013d, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__CMP \
	io0x13e, error_IO_PWM_SLAVE3__CMP_has_no_byte_access, 0x013eu, 0x013e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE3__HT \
	io0x140, error_IO_PWM_SLAVE3__HT_has_no_byte_access, 0x0140u, 0x0140, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE3__LT \
	io0x142, error_IO_PWM_SLAVE3__LT_has_no_byte_access, 0x0142u, 0x0142, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE3__PER \
	io0x144, error_IO_PWM_SLAVE3__PER_has_no_byte_access, 0x0144u, 0x0144, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE3__START \
	io0x146, io_byte_0x146, 0x0146u, 0x0146, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__STOP \
	io0x146, io_byte_0x146, 0x0146u, 0x0146, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__SLAVE \
	io0x146, io_byte_0x146, 0x0146u, 0x0146, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__MODE \
	io0x146, io_byte_0x146, 0x0146u, 0x0146, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__POL \
	io0x146, io_byte_0x146, 0x0146u, 0x0146, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__IDLE \
	io0x146, io_byte_0x146, 0x0146u, 0x0146, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__PWM_IN \
	io0x146, io_byte_0x146, 0x0146u, 0x0146, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__PSCLN \
	io0x146, io_byte_0x147, 0x0146u, 0x0147, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE3__PSCLM \
	io0x146, io_byte_0x147, 0x0146u, 0x0147, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__CMP \
	io0x148, error_IO_PWM_SLAVE4__CMP_has_no_byte_access, 0x0148u, 0x0148, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE4__HT \
	io0x14a, error_IO_PWM_SLAVE4__HT_has_no_byte_access, 0x014au, 0x014a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE4__LT \
	io0x14c, error_IO_PWM_SLAVE4__LT_has_no_byte_access, 0x014cu, 0x014c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE4__PER \
	io0x14e, error_IO_PWM_SLAVE4__PER_has_no_byte_access, 0x014eu, 0x014e, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE4__START \
	io0x150, io_byte_0x150, 0x0150u, 0x0150, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__STOP \
	io0x150, io_byte_0x150, 0x0150u, 0x0150, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__SLAVE \
	io0x150, io_byte_0x150, 0x0150u, 0x0150, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__MODE \
	io0x150, io_byte_0x150, 0x0150u, 0x0150, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__POL \
	io0x150, io_byte_0x150, 0x0150u, 0x0150, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__IDLE \
	io0x150, io_byte_0x150, 0x0150u, 0x0150, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__PWM_IN \
	io0x150, io_byte_0x150, 0x0150u, 0x0150, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__PSCLN \
	io0x150, io_byte_0x151, 0x0150u, 0x0151, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE4__PSCLM \
	io0x150, io_byte_0x151, 0x0150u, 0x0151, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__CMP \
	io0x152, error_IO_PWM_SLAVE5__CMP_has_no_byte_access, 0x0152u, 0x0152, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE5__HT \
	io0x154, error_IO_PWM_SLAVE5__HT_has_no_byte_access, 0x0154u, 0x0154, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE5__LT \
	io0x156, error_IO_PWM_SLAVE5__LT_has_no_byte_access, 0x0156u, 0x0156, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE5__PER \
	io0x158, error_IO_PWM_SLAVE5__PER_has_no_byte_access, 0x0158u, 0x0158, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE5__START \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__STOP \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__SLAVE \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__MODE \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__POL \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__IDLE \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__PWM_IN \
	io0x15a, io_byte_0x15a, 0x015au, 0x015a, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__PSCLN \
	io0x15a, io_byte_0x15b, 0x015au, 0x015b, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE5__PSCLM \
	io0x15a, io_byte_0x15b, 0x015au, 0x015b, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__CMP \
	io0x15c, error_IO_PWM_SLAVE6__CMP_has_no_byte_access, 0x015cu, 0x015c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE6__HT \
	io0x15e, error_IO_PWM_SLAVE6__HT_has_no_byte_access, 0x015eu, 0x015e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE6__LT \
	io0x160, error_IO_PWM_SLAVE6__LT_has_no_byte_access, 0x0160u, 0x0160, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE6__PER \
	io0x162, error_IO_PWM_SLAVE6__PER_has_no_byte_access, 0x0162u, 0x0162, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE6__START \
	io0x164, io_byte_0x164, 0x0164u, 0x0164, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__STOP \
	io0x164, io_byte_0x164, 0x0164u, 0x0164, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__SLAVE \
	io0x164, io_byte_0x164, 0x0164u, 0x0164, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__MODE \
	io0x164, io_byte_0x164, 0x0164u, 0x0164, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__POL \
	io0x164, io_byte_0x164, 0x0164u, 0x0164, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__IDLE \
	io0x164, io_byte_0x164, 0x0164u, 0x0164, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__PWM_IN \
	io0x164, io_byte_0x164, 0x0164u, 0x0164, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__PSCLN \
	io0x164, io_byte_0x165, 0x0164u, 0x0165, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE6__PSCLM \
	io0x164, io_byte_0x165, 0x0164u, 0x0165, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__CMP \
	io0x166, error_IO_PWM_SLAVE7__CMP_has_no_byte_access, 0x0166u, 0x0166, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE7__HT \
	io0x168, error_IO_PWM_SLAVE7__HT_has_no_byte_access, 0x0168u, 0x0168, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE7__LT \
	io0x16a, error_IO_PWM_SLAVE7__LT_has_no_byte_access, 0x016au, 0x016a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE7__PER \
	io0x16c, error_IO_PWM_SLAVE7__PER_has_no_byte_access, 0x016cu, 0x016c, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE7__START \
	io0x16e, io_byte_0x16e, 0x016eu, 0x016e, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__STOP \
	io0x16e, io_byte_0x16e, 0x016eu, 0x016e, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__SLAVE \
	io0x16e, io_byte_0x16e, 0x016eu, 0x016e, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__MODE \
	io0x16e, io_byte_0x16e, 0x016eu, 0x016e, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__POL \
	io0x16e, io_byte_0x16e, 0x016eu, 0x016e, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__IDLE \
	io0x16e, io_byte_0x16e, 0x016eu, 0x016e, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__PWM_IN \
	io0x16e, io_byte_0x16e, 0x016eu, 0x016e, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__PSCLN \
	io0x16e, io_byte_0x16f, 0x016eu, 0x016f, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE7__PSCLM \
	io0x16e, io_byte_0x16f, 0x016eu, 0x016f, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__CMP \
	io0x170, error_IO_PWM_SLAVE8__CMP_has_no_byte_access, 0x0170u, 0x0170, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE8__HT \
	io0x172, error_IO_PWM_SLAVE8__HT_has_no_byte_access, 0x0172u, 0x0172, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE8__LT \
	io0x174, error_IO_PWM_SLAVE8__LT_has_no_byte_access, 0x0174u, 0x0174, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE8__PER \
	io0x176, error_IO_PWM_SLAVE8__PER_has_no_byte_access, 0x0176u, 0x0176, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE8__START \
	io0x178, io_byte_0x178, 0x0178u, 0x0178, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__STOP \
	io0x178, io_byte_0x178, 0x0178u, 0x0178, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__SLAVE \
	io0x178, io_byte_0x178, 0x0178u, 0x0178, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__MODE \
	io0x178, io_byte_0x178, 0x0178u, 0x0178, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__POL \
	io0x178, io_byte_0x178, 0x0178u, 0x0178, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__IDLE \
	io0x178, io_byte_0x178, 0x0178u, 0x0178, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__PWM_IN \
	io0x178, io_byte_0x178, 0x0178u, 0x0178, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__PSCLN \
	io0x178, io_byte_0x179, 0x0178u, 0x0179, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE8__PSCLM \
	io0x178, io_byte_0x179, 0x0178u, 0x0179, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__CMP \
	io0x17a, error_IO_PWM_SLAVE9__CMP_has_no_byte_access, 0x017au, 0x017a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE9__HT \
	io0x17c, error_IO_PWM_SLAVE9__HT_has_no_byte_access, 0x017cu, 0x017c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE9__LT \
	io0x17e, error_IO_PWM_SLAVE9__LT_has_no_byte_access, 0x017eu, 0x017e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE9__PER \
	io0x180, error_IO_PWM_SLAVE9__PER_has_no_byte_access, 0x0180u, 0x0180, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE9__START \
	io0x182, io_byte_0x182, 0x0182u, 0x0182, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__STOP \
	io0x182, io_byte_0x182, 0x0182u, 0x0182, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__SLAVE \
	io0x182, io_byte_0x182, 0x0182u, 0x0182, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__MODE \
	io0x182, io_byte_0x182, 0x0182u, 0x0182, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__POL \
	io0x182, io_byte_0x182, 0x0182u, 0x0182, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__IDLE \
	io0x182, io_byte_0x182, 0x0182u, 0x0182, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__PWM_IN \
	io0x182, io_byte_0x182, 0x0182u, 0x0182, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__PSCLN \
	io0x182, io_byte_0x183, 0x0182u, 0x0183, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE9__PSCLM \
	io0x182, io_byte_0x183, 0x0182u, 0x0183, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__CMP \
	io0x184, error_IO_PWM_SLAVE10__CMP_has_no_byte_access, 0x0184u, 0x0184, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE10__HT \
	io0x186, error_IO_PWM_SLAVE10__HT_has_no_byte_access, 0x0186u, 0x0186, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE10__LT \
	io0x188, error_IO_PWM_SLAVE10__LT_has_no_byte_access, 0x0188u, 0x0188, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE10__PER \
	io0x18a, error_IO_PWM_SLAVE10__PER_has_no_byte_access, 0x018au, 0x018a, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE10__START \
	io0x18c, io_byte_0x18c, 0x018cu, 0x018c, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__STOP \
	io0x18c, io_byte_0x18c, 0x018cu, 0x018c, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__SLAVE \
	io0x18c, io_byte_0x18c, 0x018cu, 0x018c, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__MODE \
	io0x18c, io_byte_0x18c, 0x018cu, 0x018c, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__POL \
	io0x18c, io_byte_0x18c, 0x018cu, 0x018c, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__IDLE \
	io0x18c, io_byte_0x18c, 0x018cu, 0x018c, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__PWM_IN \
	io0x18c, io_byte_0x18c, 0x018cu, 0x018c, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__PSCLN \
	io0x18c, io_byte_0x18d, 0x018cu, 0x018d, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE10__PSCLM \
	io0x18c, io_byte_0x18d, 0x018cu, 0x018d, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__CMP \
	io0x18e, error_IO_PWM_SLAVE11__CMP_has_no_byte_access, 0x018eu, 0x018e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE11__HT \
	io0x190, error_IO_PWM_SLAVE11__HT_has_no_byte_access, 0x0190u, 0x0190, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE11__LT \
	io0x192, error_IO_PWM_SLAVE11__LT_has_no_byte_access, 0x0192u, 0x0192, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE11__PER \
	io0x194, error_IO_PWM_SLAVE11__PER_has_no_byte_access, 0x0194u, 0x0194, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_PWM_SLAVE11__START \
	io0x196, io_byte_0x196, 0x0196u, 0x0196, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__STOP \
	io0x196, io_byte_0x196, 0x0196u, 0x0196, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__SLAVE \
	io0x196, io_byte_0x196, 0x0196u, 0x0196, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__MODE \
	io0x196, io_byte_0x196, 0x0196u, 0x0196, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__POL \
	io0x196, io_byte_0x196, 0x0196u, 0x0196, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__IDLE \
	io0x196, io_byte_0x196, 0x0196u, 0x0196, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__PWM_IN \
	io0x196, io_byte_0x196, 0x0196u, 0x0196, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__PSCLN \
	io0x196, io_byte_0x197, 0x0196u, 0x0197, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x0000u, 0x0083u
#define IO_PWM_SLAVE11__PSCLM \
	io0x196, io_byte_0x197, 0x0196u, 0x0197, 0xf000u, 0xf0u, 12U, 4, 1U, 12u, 0x0000u, 0x0083u
#define IO_ADC_SAR__START \
	io0x198, io_byte_0x198, 0x0198u, 0x0198, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__STOP \
	io0x198, io_byte_0x198, 0x0198u, 0x0198, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__SOS_SOURCE \
	io0x198, io_byte_0x198, 0x0198u, 0x0198, 0x000cu, 0x0cu,  2U, 2, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__SOC_SOURCE \
	io0x198, io_byte_0x198, 0x0198u, 0x0198, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__NO_INTERLEAVE \
	io0x198, io_byte_0x198, 0x0198u, 0x0198, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__SATURATE \
	io0x198, io_byte_0x198, 0x0198u, 0x0198, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__INT_SCHEME \
	io0x198, io_byte_0x199, 0x0198u, 0x0199, 0x0300u, 0x03u,  8U, 0, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__ASB \
	io0x198, io_byte_0x199, 0x0198u, 0x0199, 0x0c00u, 0x0cu, 10U, 2, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__ADC_WIDTH \
	io0x198, io_byte_0x199, 0x0198u, 0x0199, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0xe000u, 0x0003u
#define IO_ADC_SAR__SBASE_0 \
	io0x19a, error_IO_ADC_SAR__SBASE_0_has_no_byte_access, 0x019au, 0x019a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0xffffu
#define IO_ADC_SAR__PAUSE \
	io0x19c, io_byte_0x19c, 0x019cu, 0x019c, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__RESUME \
	io0x19c, io_byte_0x19c, 0x019cu, 0x019c, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__SW_TRIG \
	io0x19c, io_byte_0x19c, 0x019cu, 0x019c, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__READY \
	io0x19c, io_byte_0x19c, 0x019cu, 0x019c, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__LAST_INT_SRC \
	io0x19c, io_byte_0x19c, 0x019cu, 0x019c, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__STATE \
	io0x19c, io_byte_0x19c, 0x019cu, 0x019c, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__ADC_OVF \
	io0x19c, io_byte_0x19d, 0x019cu, 0x019d, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__ADC_ERR \
	io0x19c, io_byte_0x19d, 0x019cu, 0x019d, 0x0200u, 0x02u,  9U, 1, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__MEM_ERR \
	io0x19c, io_byte_0x19d, 0x019cu, 0x019d, 0x0400u, 0x04u, 10U, 2, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__FRAME_ERR \
	io0x19c, io_byte_0x19d, 0x019cu, 0x019d, 0x0800u, 0x08u, 11U, 3, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__ABORTED \
	io0x19c, io_byte_0x19d, 0x019cu, 0x019d, 0x1000u, 0x10u, 12U, 4, 1U, 12u, 0xe000u, 0x1fffu
#define IO_ADC_SAR__ADC_CLK_DIV \
	io0x19e, io_byte_0x19e, 0x019eu, 0x019e, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0xff80u, 0x0000u
#define IO_ADC_SAR__TEST_CONV2 \
	io0x1a0, io_byte_0x1a0, 0x01a0u, 0x01a0, 0x0001u, 0x01u,  0U, 0, 1U, 28u, 0xfff0u, 0x0000u
#define IO_ADC_SAR__TEST_CONV3 \
	io0x1a0, io_byte_0x1a0, 0x01a0u, 0x01a0, 0x0002u, 0x02u,  1U, 1, 1U, 28u, 0xfff0u, 0x0000u
#define IO_ADC_SAR__TEST_CONV4 \
	io0x1a0, io_byte_0x1a0, 0x01a0u, 0x01a0, 0x0004u, 0x04u,  2U, 2, 1U, 28u, 0xfff0u, 0x0000u
#define IO_ADC_SAR__TEST_TRIM \
	io0x1a0, io_byte_0x1a0, 0x01a0u, 0x01a0, 0x0008u, 0x08u,  3U, 3, 1U, 28u, 0xfff0u, 0x0000u
#define IO_ADC_SAR__ADC_DATA \
	io0x1a2, error_IO_ADC_SAR__ADC_DATA_has_no_byte_access, 0x01a2u, 0x01a2, 0x03ffu, 0xffu,  0U, 0, 1U, 24u, 0xf800u, 0x0000u
#define IO_ADC_SAR__SEL_TR_ADCREF \
	io0x1a2, io_byte_0x1a3, 0x01a2u, 0x01a3, 0x0400u, 0x04u, 10U, 2, 1U, 28u, 0xf800u, 0x0000u
#define IO_ADC_SAR__TR_ADCREF1 \
	io0x1a4, io_byte_0x1a4, 0x01a4u, 0x01a4, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0xc000u, 0x0000u
#define IO_ADC_SAR__TR_ADCREF2 \
	io0x1a4, error_IO_ADC_SAR__TR_ADCREF2_has_no_byte_access, 0x01a4u, 0x01a4, 0x3f80u, 0x80u,  7U, 7, 1U, 8u, 0xc000u, 0x0000u
#define IO_ADC_SAR__TR_ADCREF3 \
	io0x1a6, io_byte_0x1a6, 0x01a6u, 0x01a6, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0xff00u, 0x0000u
#define IO_ADC_SAR__IDDQ_ADC \
	io0x1a6, io_byte_0x1a6, 0x01a6u, 0x01a6, 0x0080u, 0x80u,  7U, 7, 1U, 12u, 0xff00u, 0x0000u
#define IO_PORT_ADC_CTRL__ADC_INV_COMPOUT \
	io0x1a8, io_byte_0x1a8, 0x01a8u, 0x01a8, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfffcu, 0x0000u
#define IO_PORT_ADC_CTRL__ADC_EN \
	io0x1a8, io_byte_0x1a8, 0x01a8u, 0x01a8, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xfffcu, 0x0000u
#define IO_PORT_TEST_ADC__TEST_DAC \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0001u, 0x01u,  0U, 0, 1U, 28u, 0xf000u, 0x0000u
#define IO_PORT_TEST_ADC__TEST_DAC_BUF \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0002u, 0x02u,  1U, 1, 1U, 28u, 0xf000u, 0x0000u
#define IO_PORT_TEST_ADC__TEST_LDAC \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0004u, 0x04u,  2U, 2, 1U, 28u, 0xf000u, 0x0000u
#define IO_PORT_TEST_ADC__TEST_HDAC \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0008u, 0x08u,  3U, 3, 1U, 28u, 0xf000u, 0x0000u
#define IO_PORT_TEST_ADC__TEST_ADC_CURSRC \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0010u, 0x10u,  4U, 4, 1U, 28u, 0xf000u, 0x0000u
#define IO_PORT_TEST_ADC__TEST_ADC_REF \
	io0x1aa, io_byte_0x1aa, 0x01aau, 0x01aa, 0x0060u, 0x60u,  5U, 5, 1U, 28u, 0xf000u, 0x0000u
#define IO_PORT_TEST_ADC__TEST_ADC_IN \
	io0x1aa, error_IO_PORT_TEST_ADC__TEST_ADC_IN_has_no_byte_access, 0x01aau, 0x01aa, 0x0180u, 0x80u,  7U, 7, 1U, 24u, 0xf000u, 0x0000u
#define IO_PORT_TEST_ADC__TEST_ADC_SPARE \
	io0x1aa, error_IO_PORT_TEST_ADC__TEST_ADC_SPARE_has_no_byte_access, 0x01aau, 0x01aa, 0x0e00u, 0x0eu,  9U, 1, 1U, 24u, 0xf000u, 0x0000u
#define IO_EEPROM_FLASH__T_10NS \
	io0x1ac, io_byte_0x1ac, 0x01acu, 0x01ac, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0002u, 0x0000u
#define IO_EEPROM_FLASH__T_50NS \
	io0x1ac, io_byte_0x1ac, 0x01acu, 0x01ac, 0x001cu, 0x1cu,  2U, 2, 1U, 12u, 0x0002u, 0x0000u
#define IO_EEPROM_FLASH__T_10US \
	io0x1ac, error_IO_EEPROM_FLASH__T_10US_has_no_byte_access, 0x01acu, 0x01ac, 0x03e0u, 0xe0u,  5U, 5, 1U, 8u, 0x0002u, 0x0000u
#define IO_EEPROM_FLASH__T_50US \
	io0x1ac, io_byte_0x1ad, 0x01acu, 0x01ad, 0x7c00u, 0x7cu, 10U, 2, 1U, 12u, 0x0002u, 0x0000u
#define IO_EEPROM_FLASH__LOCK_T \
	io0x1ac, io_byte_0x1ad, 0x01acu, 0x01ad, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0002u, 0x0000u
#define IO_EEPROM_FLASH__T_1US \
	io0x1ae, io_byte_0x1ae, 0x01aeu, 0x01ae, 0x001fu, 0x1fu,  0U, 0, 1U, 12u, 0x7fe0u, 0x0000u
#define IO_EEPROM_FLASH__LOCK_1US \
	io0x1ae, io_byte_0x1af, 0x01aeu, 0x01af, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7fe0u, 0x0000u
#define IO_EEPROM_FLASH__FL_DMA \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x0001u, 0x01u,  0U, 0, 1U, 28u, 0x7c0eu, 0x00f0u
#define IO_EEPROM_FLASH__EE_FL_VERSION \
	io0x1b0, io_byte_0x1b0, 0x01b0u, 0x01b0, 0x00f0u, 0xf0u,  4U, 4, 1U, 28u, 0x7c0eu, 0x00f0u
#define IO_EEPROM_FLASH__EE_DMA \
	io0x1b0, io_byte_0x1b1, 0x01b0u, 0x01b1, 0x0100u, 0x01u,  8U, 0, 1U, 28u, 0x7c0eu, 0x00f0u
#define IO_EEPROM_FLASH__EE_EXTEND_DATA \
	io0x1b0, io_byte_0x1b1, 0x01b0u, 0x01b1, 0x0200u, 0x02u,  9U, 1, 1U, 28u, 0x7c0eu, 0x00f0u
#define IO_EEPROM_FLASH__EE_FL_PDN_LOW \
	io0x1b0, io_byte_0x1b1, 0x01b0u, 0x01b1, 0x8000u, 0x80u, 15U, 7, 1U, 28u, 0x7c0eu, 0x00f0u
#define IO_EEPROM_FLASH__FL_COMMAND \
	io0x1b2, error_IO_EEPROM_FLASH__FL_COMMAND_has_no_byte_access, 0x01b2u, 0x01b2, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_EEPROM_FLASH__FL_STATUS \
	io0x1b2, io_byte_0x1b2, 0x01b2u, 0x01b2, 0x000fu, 0x0fu,  0U, 0, 0U, 12u, 0x0000u, 0x0000u
#define IO_EEPROM_FLASH__FL_ER_TIME \
	io0x1b4, io_byte_0x1b4, 0x01b4u, 0x01b4, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_EEPROM_FLASH__FL_WR_TIME \
	io0x1b4, io_byte_0x1b5, 0x01b4u, 0x01b5, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_EEPROM_FLASH__FL_LOCK_ER_WR \
	io0x1b4, io_byte_0x1b5, 0x01b4u, 0x01b5, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_EEPROM_FLASH__FL_WAIT_STATES \
	io0x1b6, io_byte_0x1b6, 0x01b6u, 0x01b6, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x4828u, 0x0000u
#define IO_EEPROM_FLASH__FL_BYPASS_QUEUE \
	io0x1b6, io_byte_0x1b6, 0x01b6u, 0x01b6, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0x4828u, 0x0000u
#define IO_EEPROM_FLASH__FL_PREDICTION_BEHAVIOR \
	io0x1b6, io_byte_0x1b6, 0x01b6u, 0x01b6, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0x4828u, 0x0000u
#define IO_EEPROM_FLASH__FL_DED_RETRY \
	io0x1b6, io_byte_0x1b7, 0x01b6u, 0x01b7, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0x4828u, 0x0000u
#define IO_EEPROM_FLASH__FL_HALT_BEHAVIOR \
	io0x1b6, io_byte_0x1b7, 0x01b6u, 0x01b7, 0x3000u, 0x30u, 12U, 4, 1U, 12u, 0x4828u, 0x0000u
#define IO_EEPROM_FLASH__FL_LOCK_RDY \
	io0x1b6, io_byte_0x1b7, 0x01b6u, 0x01b7, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x4828u, 0x0000u
#define IO_EEPROM_FLASH__FL_SBE \
	io0x1b8, io_byte_0x1b8, 0x01b8u, 0x01b8, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfefcu, 0x0003u
#define IO_EEPROM_FLASH__FL_DATA_CORRUPTED \
	io0x1b8, io_byte_0x1b8, 0x01b8u, 0x01b8, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xfefcu, 0x0003u
#define IO_EEPROM_FLASH__FL_EXTENDED_DATA \
	io0x1b8, io_byte_0x1b9, 0x01b8u, 0x01b9, 0x0100u, 0x01u,  8U, 0, 1U, 12u, 0xfefcu, 0x0003u
#define IO_EEPROM_FLASH__FL_PAGE_NUMBER \
	io0x1ba, error_IO_EEPROM_FLASH__FL_PAGE_NUMBER_has_no_byte_access, 0x01bau, 0x01ba, 0x000fu, 0x0fu,  0U, 0, 1U, 8u, 0xfe00u, 0x0000u
#define IO_EEPROM_FLASH__FL_SECTOR_NUMBER \
	io0x1ba, error_IO_EEPROM_FLASH__FL_SECTOR_NUMBER_has_no_byte_access, 0x01bau, 0x01ba, 0x00f0u, 0xf0u,  4U, 4, 1U, 8u, 0xfe00u, 0x0000u
#define IO_EEPROM_FLASH__FL_CS_AREA \
	io0x1ba, error_IO_EEPROM_FLASH__FL_CS_AREA_has_no_byte_access, 0x01bau, 0x01bb, 0x0100u, 0x01u,  8U, 0, 1U, 8u, 0xfe00u, 0x0000u
#define IO_EEPROM_FLASH__EE_ACTIVE \
	io0x1bc, io_byte_0x1bc, 0x01bcu, 0x01bc, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x080cu, 0x0000u
#define IO_EEPROM_FLASH__EE_CONFIGURED \
	io0x1bc, io_byte_0x1bc, 0x01bcu, 0x01bc, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x080cu, 0x0000u
#define IO_EEPROM_FLASH__EE_WE_KEY \
	io0x1bc, io_byte_0x1bc, 0x01bcu, 0x01bc, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x080cu, 0x0000u
#define IO_EEPROM_FLASH__EE_W_MODE \
	io0x1bc, io_byte_0x1bd, 0x01bcu, 0x01bd, 0x0700u, 0x07u,  8U, 0, 1U, 12u, 0x080cu, 0x0000u
#define IO_EEPROM_FLASH__EE_BUSY_BUF_NOT_EMPTY \
	io0x1bc, io_byte_0x1bd, 0x01bcu, 0x01bd, 0x1000u, 0x10u, 12U, 4, 0U, 12u, 0x080cu, 0x0000u
#define IO_EEPROM_FLASH__EE_BUSY_WR \
	io0x1bc, io_byte_0x1bd, 0x01bcu, 0x01bd, 0x2000u, 0x20u, 13U, 5, 0U, 12u, 0x080cu, 0x0000u
#define IO_EEPROM_FLASH__EE_BUSY_STBY \
	io0x1bc, io_byte_0x1bd, 0x01bcu, 0x01bd, 0x4000u, 0x40u, 14U, 6, 0U, 12u, 0x080cu, 0x0000u
#define IO_EEPROM_FLASH__EE_BUSY \
	io0x1bc, io_byte_0x1bd, 0x01bcu, 0x01bd, 0x8000u, 0x80u, 15U, 7, 0U, 12u, 0x080cu, 0x0000u
#define IO_EEPROM_FLASH__EE_SBE_1 \
	io0x1be, io_byte_0x1be, 0x01beu, 0x01be, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfff0u, 0x000fu
#define IO_EEPROM_FLASH__EE_DATA_CORRUPTED_1 \
	io0x1be, io_byte_0x1be, 0x01beu, 0x01be, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xfff0u, 0x000fu
#define IO_EEPROM_FLASH__EE_SBE_2 \
	io0x1be, io_byte_0x1be, 0x01beu, 0x01be, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xfff0u, 0x000fu
#define IO_EEPROM_FLASH__EE_DATA_CORRUPTED_2 \
	io0x1be, io_byte_0x1be, 0x01beu, 0x01be, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xfff0u, 0x000fu
#define IO_EEPROM_FLASH__EE_PRE_WR_TIME \
	io0x1c0, io_byte_0x1c0, 0x01c0u, 0x01c0, 0x007fu, 0x7fu,  0U, 0, 1U, 12u, 0x0080u, 0x0000u
#define IO_EEPROM_FLASH__EE_WR_TIME \
	io0x1c0, io_byte_0x1c1, 0x01c0u, 0x01c1, 0x7f00u, 0x7fu,  8U, 0, 1U, 12u, 0x0080u, 0x0000u
#define IO_EEPROM_FLASH__EE_LOCK_WR \
	io0x1c0, io_byte_0x1c1, 0x01c0u, 0x01c1, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0080u, 0x0000u
#define IO_EEPROM_FLASH__EE_ER_TIME \
	io0x1c2, io_byte_0x1c2, 0x01c2u, 0x01c2, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x7fc0u, 0x0000u
#define IO_EEPROM_FLASH__EE_LOCK_ER \
	io0x1c2, io_byte_0x1c3, 0x01c2u, 0x01c3, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7fc0u, 0x0000u
#define IO_EEPROM_FLASH__EE_PROGRAM_CYCLE \
	io0x1c4, io_byte_0x1c4, 0x01c4u, 0x01c4, 0x001fu, 0x1fu,  0U, 0, 1U, 12u, 0x7fe0u, 0x0000u
#define IO_EEPROM_FLASH__EE_PROGRAM_CYCLE_LOCK \
	io0x1c4, io_byte_0x1c5, 0x01c4u, 0x01c5, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7fe0u, 0x0000u
#define IO_EEPROM_FLASH__EE_HALT_BEHAVIOR \
	io0x1c6, io_byte_0x1c6, 0x01c6u, 0x01c6, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0x70fcu, 0x0000u
#define IO_EEPROM_FLASH__EE_WAIT_STATES \
	io0x1c6, io_byte_0x1c7, 0x01c6u, 0x01c7, 0x0f00u, 0x0fu,  8U, 0, 1U, 12u, 0x70fcu, 0x0000u
#define IO_EEPROM_FLASH__EE_LOCK_RD \
	io0x1c6, io_byte_0x1c7, 0x01c6u, 0x01c7, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x70fcu, 0x0000u
#define IO_EEPROM_FLASH__EE_INT_INVALID_SRC \
	io0x1c8, io_byte_0x1c8, 0x01c8u, 0x01c8, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0xfff0u, 0x000fu
#define IO_EEPROM_FLASH__FL_PATTERN_ID \
	io0x1d2, io_byte_0x1d2, 0x01d2u, 0x01d2, 0x0070u, 0x70u,  4U, 4, 1U, 28u, 0x308fu, 0x8000u
#define IO_EEPROM_FLASH__FL_ACCESS_ORDER \
	io0x1d2, io_byte_0x1d3, 0x01d2u, 0x01d3, 0x0300u, 0x03u,  8U, 0, 1U, 28u, 0x308fu, 0x8000u
#define IO_EEPROM_FLASH__FL_TEST_AREA \
	io0x1d2, io_byte_0x1d3, 0x01d2u, 0x01d3, 0x0c00u, 0x0cu, 10U, 2, 1U, 28u, 0x308fu, 0x8000u
#define IO_EEPROM_FLASH__FL_ERROR \
	io0x1d2, io_byte_0x1d3, 0x01d2u, 0x01d3, 0x4000u, 0x40u, 14U, 6, 0U, 28u, 0x308fu, 0x8000u
#define IO_EEPROM_FLASH__FL_COMPLETED \
	io0x1d2, io_byte_0x1d3, 0x01d2u, 0x01d3, 0x8000u, 0x80u, 15U, 7, 1U, 28u, 0x308fu, 0x8000u
#define IO_EEPROM_FLASH__FL_BITS_ERRORS \
	io0x1d4, error_IO_EEPROM_FLASH__FL_BITS_ERRORS_has_no_byte_access, 0x01d4u, 0x01d4, 0x01ffu, 0xffu,  0U, 0, 1U, 8u, 0xfe00u, 0x0000u
#define IO_EEPROM_FLASH__FL_SIG_L \
	io0x1d6, error_IO_EEPROM_FLASH__FL_SIG_L_has_no_byte_access, 0x01d6u, 0x01d6, 0xffffu, 0xffu,  0U, 0, 1U, 24u, 0x0000u, 0xffffu
#define IO_EEPROM_FLASH__FL_SIG_H \
	io0x1d8, error_IO_EEPROM_FLASH__FL_SIG_H_has_no_byte_access, 0x01d8u, 0x01d8, 0xffffu, 0xffu,  0U, 0, 1U, 24u, 0x0000u, 0xffffu
#define IO_EEPROM_FLASH__EE_CMD \
	io0x1da, io_byte_0x1da, 0x01dau, 0x01da, 0x0003u, 0x03u,  0U, 0, 1U, 28u, 0x108cu, 0x8000u
#define IO_EEPROM_FLASH__EE_PATTERN_ID \
	io0x1da, io_byte_0x1da, 0x01dau, 0x01da, 0x0070u, 0x70u,  4U, 4, 1U, 28u, 0x108cu, 0x8000u
#define IO_EEPROM_FLASH__EE_ACCESS_ORDER \
	io0x1da, io_byte_0x1db, 0x01dau, 0x01db, 0x0300u, 0x03u,  8U, 0, 1U, 28u, 0x108cu, 0x8000u
#define IO_EEPROM_FLASH__EE_TEST_AREA \
	io0x1da, io_byte_0x1db, 0x01dau, 0x01db, 0x0c00u, 0x0cu, 10U, 2, 1U, 28u, 0x108cu, 0x8000u
#define IO_EEPROM_FLASH__EE_IN_PROGRESS \
	io0x1da, io_byte_0x1db, 0x01dau, 0x01db, 0x2000u, 0x20u, 13U, 5, 0U, 28u, 0x108cu, 0x8000u
#define IO_EEPROM_FLASH__EE_ERROR \
	io0x1da, io_byte_0x1db, 0x01dau, 0x01db, 0x4000u, 0x40u, 14U, 6, 0U, 28u, 0x108cu, 0x8000u
#define IO_EEPROM_FLASH__EE_COMPLETED \
	io0x1da, io_byte_0x1db, 0x01dau, 0x01db, 0x8000u, 0x80u, 15U, 7, 1U, 28u, 0x108cu, 0x8000u
#define IO_EEPROM_FLASH__EE_BITS_ERRORS \
	io0x1dc, error_IO_EEPROM_FLASH__EE_BITS_ERRORS_has_no_byte_access, 0x01dcu, 0x01dc, 0x01ffu, 0xffu,  0U, 0, 1U, 8u, 0xfe00u, 0x0000u
#define IO_EEPROM_FLASH__EE_SIG_L \
	io0x1de, error_IO_EEPROM_FLASH__EE_SIG_L_has_no_byte_access, 0x01deu, 0x01de, 0xffffu, 0xffu,  0U, 0, 1U, 24u, 0x0000u, 0xffffu
#define IO_EEPROM_FLASH__EE_SIG_H \
	io0x1e0, error_IO_EEPROM_FLASH__EE_SIG_H_has_no_byte_access, 0x01e0u, 0x01e0, 0xffffu, 0xffu,  0U, 0, 1U, 24u, 0x0000u, 0xffffu
#define IO_PORT_LIN_XKEY__LIN_XKEY \
	io0x1e2, error_IO_PORT_LIN_XKEY__LIN_XKEY_has_no_byte_access, 0x01e2u, 0x01e2, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_TRIM_BG_BIAS__PRE_TR_BGA \
	io0x1e4, io_byte_0x1e4, 0x01e4u, 0x01e4, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_TRIM_BG_BIAS__PRE_TR_BGD \
	io0x1e4, io_byte_0x1e4, 0x01e4u, 0x01e4, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_TRIM_BG_BIAS__PRE_TR_BIAS \
	io0x1e4, error_IO_TRIM_BG_BIAS__PRE_TR_BIAS_has_no_byte_access, 0x01e4u, 0x01e4, 0x3f00u, 0x3fu,  8U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_TRIM_BG_BIAS__TR_UNUSED \
	io0x1e4, error_IO_TRIM_BG_BIAS__TR_UNUSED_has_no_byte_access, 0x01e4u, 0x01e4, 0x4000u, 0x40u, 14U, 6, 1U, 8u, 0x0000u, 0x0000u
#define IO_TRIM_BG_BIAS__LOCK \
	io0x1e4, io_byte_0x1e5, 0x01e4u, 0x01e5, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_TRIM_VDD__PRE_TR_VDDA \
	io0x1e6, io_byte_0x1e6, 0x01e6u, 0x01e6, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_TRIM_VDD__PRE_TR_VDDD \
	io0x1e6, io_byte_0x1e6, 0x01e6u, 0x01e6, 0x0038u, 0x38u,  3U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_TRIM_VDD__PRE_TR_SUP \
	io0x1e6, error_IO_TRIM_VDD__PRE_TR_SUP_has_no_byte_access, 0x01e6u, 0x01e6, 0x3fc0u, 0xc0u,  6U, 6, 1U, 8u, 0x0000u, 0x0000u
#define IO_TRIM_VDD__TR_UNUSED \
	io0x1e6, error_IO_TRIM_VDD__TR_UNUSED_has_no_byte_access, 0x01e6u, 0x01e6, 0x4000u, 0x40u, 14U, 6, 1U, 8u, 0x0000u, 0x0000u
#define IO_TRIM_VDD__LOCK \
	io0x1e6, io_byte_0x1e7, 0x01e6u, 0x01e7, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_TRIM_RCO32M__TR_RCO32M_IN \
	io0x1e8, error_IO_TRIM_RCO32M__TR_RCO32M_IN_has_no_byte_access, 0x01e8u, 0x01e8, 0x03ffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_TRIM_RCO32M__TR_UNUSED \
	io0x1e8, error_IO_TRIM_RCO32M__TR_UNUSED_has_no_byte_access, 0x01e8u, 0x01e8, 0x7c00u, 0x7cu, 10U, 2, 1U, 8u, 0x0000u, 0x0000u
#define IO_TRIM_RCO32M__LOCK \
	io0x1e8, io_byte_0x1e9, 0x01e8u, 0x01e9, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x0000u, 0x0000u
#define IO_TRIM_RCO1M__PRE_TR_RCO1M \
	io0x1ea, io_byte_0x1ea, 0x01eau, 0x01ea, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0x7f00u, 0x0000u
#define IO_TRIM_RCO1M__LOCK \
	io0x1ea, io_byte_0x1eb, 0x01eau, 0x01eb, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7f00u, 0x0000u
#define IO_PORT_SSCM_CONF__SSCM_EN \
	io0x1ec, io_byte_0x1ec, 0x01ecu, 0x01ec, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfffcu, 0x0000u
#define IO_PORT_SSCM_CONF__SSCM_SINGLEBIT \
	io0x1ec, io_byte_0x1ec, 0x01ecu, 0x01ec, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xfffcu, 0x0000u
#define IO_PORT_STEP_CONF__STEP_INC \
	io0x1ee, io_byte_0x1ee, 0x01eeu, 0x01ee, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_STEP_CONF__STEP_DUR \
	io0x1ee, io_byte_0x1ee, 0x01eeu, 0x01ee, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_STEP_CONF__STEP_CNT \
	io0x1ee, error_IO_PORT_STEP_CONF__STEP_CNT_has_no_byte_access, 0x01eeu, 0x01ee, 0xff00u, 0xffu,  8U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__TEST_BGA \
	io0x1f0, io_byte_0x1f0, 0x01f0u, 0x01f0, 0x0001u, 0x01u,  0U, 0, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__TEST_VAUX_ADDCURRENT \
	io0x1f0, io_byte_0x1f0, 0x01f0u, 0x01f0, 0x0002u, 0x02u,  1U, 1, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__TEST_10U_BIAS \
	io0x1f0, io_byte_0x1f0, 0x01f0u, 0x01f0, 0x0004u, 0x04u,  2U, 2, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__TEST_5U_BIASAUX \
	io0x1f0, io_byte_0x1f0, 0x01f0u, 0x01f0, 0x0008u, 0x08u,  3U, 3, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__PORTEST \
	io0x1f0, io_byte_0x1f0, 0x01f0u, 0x01f0, 0x0010u, 0x10u,  4U, 4, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__SWITCHOFFREG_VDDA \
	io0x1f0, io_byte_0x1f0, 0x01f0u, 0x01f0, 0x0060u, 0x60u,  5U, 5, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__SWITCHOFFREG_VDDD \
	io0x1f0, error_IO_PORT_SUPP_TEST__SWITCHOFFREG_VDDD_has_no_byte_access, 0x01f0u, 0x01f0, 0x0180u, 0x80u,  7U, 7, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__SWITCHOFFUV_VDDD_RES \
	io0x1f0, error_IO_PORT_SUPP_TEST__SWITCHOFFUV_VDDD_RES_has_no_byte_access, 0x01f0u, 0x01f0, 0x0200u, 0x02u,  9U, 1, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__SBY_BIAS \
	io0x1f0, error_IO_PORT_SUPP_TEST__SBY_BIAS_has_no_byte_access, 0x01f0u, 0x01f0, 0x0400u, 0x04u, 10U, 2, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__LOW_VDDD \
	io0x1f0, error_IO_PORT_SUPP_TEST__LOW_VDDD_has_no_byte_access, 0x01f0u, 0x01f0, 0x0800u, 0x08u, 11U, 3, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__SHOVE_VDDD \
	io0x1f0, error_IO_PORT_SUPP_TEST__SHOVE_VDDD_has_no_byte_access, 0x01f0u, 0x01f0, 0x1000u, 0x10u, 12U, 4, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__SHOVE_VDDA \
	io0x1f0, error_IO_PORT_SUPP_TEST__SHOVE_VDDA_has_no_byte_access, 0x01f0u, 0x01f0, 0x2000u, 0x20u, 13U, 5, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__SHOVE_VAUX \
	io0x1f0, error_IO_PORT_SUPP_TEST__SHOVE_VAUX_has_no_byte_access, 0x01f0u, 0x01f0, 0x4000u, 0x40u, 14U, 6, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP_TEST__DISABLE_RAM_ROM \
	io0x1f0, error_IO_PORT_SUPP_TEST__DISABLE_RAM_ROM_has_no_byte_access, 0x01f0u, 0x01f0, 0x8000u, 0x80u, 15U, 7, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__IDDQ_REG_VDDA \
	io0x1f2, io_byte_0x1f2, 0x01f2u, 0x01f2, 0x0001u, 0x01u,  0U, 0, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__IDDQ_REG_VDDD \
	io0x1f2, io_byte_0x1f2, 0x01f2u, 0x01f2, 0x0002u, 0x02u,  1U, 1, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__IDDQSENS_REG_VDDD \
	io0x1f2, io_byte_0x1f2, 0x01f2u, 0x01f2, 0x0004u, 0x04u,  2U, 2, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__INT_WU_TEST \
	io0x1f2, io_byte_0x1f2, 0x01f2u, 0x01f2, 0x0008u, 0x08u,  3U, 3, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__IDDQX \
	io0x1f2, io_byte_0x1f2, 0x01f2u, 0x01f2, 0x0030u, 0x30u,  4U, 4, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__TEST_TA0_EN \
	io0x1f2, io_byte_0x1f2, 0x01f2u, 0x01f2, 0x0040u, 0x40u,  6U, 6, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__TEST_TA1_EN \
	io0x1f2, io_byte_0x1f2, 0x01f2u, 0x01f2, 0x0080u, 0x80u,  7U, 7, 1U, 28u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__TEST_MEM_ANA \
	io0x1f2, error_IO_PORT_SUPP2_TEST__TEST_MEM_ANA_has_no_byte_access, 0x01f2u, 0x01f2, 0x0100u, 0x01u,  8U, 0, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__TM_35DEGC \
	io0x1f2, error_IO_PORT_SUPP2_TEST__TM_35DEGC_has_no_byte_access, 0x01f2u, 0x01f2, 0x0200u, 0x02u,  9U, 1, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__TM_TSDCMP \
	io0x1f2, error_IO_PORT_SUPP2_TEST__TM_TSDCMP_has_no_byte_access, 0x01f2u, 0x01f2, 0x0400u, 0x04u, 10U, 2, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__TM_VGB_O \
	io0x1f2, error_IO_PORT_SUPP2_TEST__TM_VGB_O_has_no_byte_access, 0x01f2u, 0x01f2, 0x0800u, 0x08u, 11U, 3, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__IDDQ_TEMPSENSE \
	io0x1f2, error_IO_PORT_SUPP2_TEST__IDDQ_TEMPSENSE_has_no_byte_access, 0x01f2u, 0x01f2, 0x1000u, 0x10u, 12U, 4, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__IDDQ_CLK10K \
	io0x1f2, error_IO_PORT_SUPP2_TEST__IDDQ_CLK10K_has_no_byte_access, 0x01f2u, 0x01f2, 0x2000u, 0x20u, 13U, 5, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__FGTSM \
	io0x1f2, error_IO_PORT_SUPP2_TEST__FGTSM_has_no_byte_access, 0x01f2u, 0x01f2, 0x4000u, 0x40u, 14U, 6, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SUPP2_TEST__FSTOP \
	io0x1f2, error_IO_PORT_SUPP2_TEST__FSTOP_has_no_byte_access, 0x01f2u, 0x01f2, 0x8000u, 0x80u, 15U, 7, 1U, 24u, 0x0000u, 0x0000u
#define IO_PORT_SPARE_TEST__TEST_SUP \
	io0x1f4, io_byte_0x1f4, 0x01f4u, 0x01f4, 0x00ffu, 0xffu,  0U, 0, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_BGA \
	io0x1f6, io_byte_0x1f6, 0x01f6u, 0x01f6, 0x0001u, 0x01u,  0U, 0, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_BGD \
	io0x1f6, io_byte_0x1f6, 0x01f6u, 0x01f6, 0x0002u, 0x02u,  1U, 1, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_VDDA \
	io0x1f6, io_byte_0x1f6, 0x01f6u, 0x01f6, 0x0004u, 0x04u,  2U, 2, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_VDDD \
	io0x1f6, io_byte_0x1f6, 0x01f6u, 0x01f6, 0x0008u, 0x08u,  3U, 3, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_BIAS \
	io0x1f6, io_byte_0x1f6, 0x01f6u, 0x01f6, 0x0010u, 0x10u,  4U, 4, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_SUP \
	io0x1f6, io_byte_0x1f6, 0x01f6u, 0x01f6, 0x0020u, 0x20u,  5U, 5, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_RCO32M \
	io0x1f6, io_byte_0x1f6, 0x01f6u, 0x01f6, 0x0040u, 0x40u,  6U, 6, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_ADC_TRIM_TEST__SEL_TR_RCO1M \
	io0x1f6, io_byte_0x1f6, 0x01f6u, 0x01f6, 0x0080u, 0x80u,  7U, 7, 1U, 28u, 0xff00u, 0x0000u
#define IO_PORT_SUPP_IN__UV_VDDA_IT \
	io0x1f8, error_IO_PORT_SUPP_IN__UV_VDDA_IT_has_no_byte_access, 0x01f8u, 0x01f8, 0x0001u, 0x01u,  0U, 0, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__UV_VDDA_SYNC \
	io0x1f8, error_IO_PORT_SUPP_IN__UV_VDDA_SYNC_has_no_byte_access, 0x01f8u, 0x01f8, 0x0002u, 0x02u,  1U, 1, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__UV_VS_IT \
	io0x1f8, error_IO_PORT_SUPP_IN__UV_VS_IT_has_no_byte_access, 0x01f8u, 0x01f8, 0x0004u, 0x04u,  2U, 2, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__UV_VS_SYNC \
	io0x1f8, error_IO_PORT_SUPP_IN__UV_VS_SYNC_has_no_byte_access, 0x01f8u, 0x01f8, 0x0008u, 0x08u,  3U, 3, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__OV_VS_IT \
	io0x1f8, error_IO_PORT_SUPP_IN__OV_VS_IT_has_no_byte_access, 0x01f8u, 0x01f8, 0x0010u, 0x10u,  4U, 4, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__OV_VS_SYNC \
	io0x1f8, error_IO_PORT_SUPP_IN__OV_VS_SYNC_has_no_byte_access, 0x01f8u, 0x01f8, 0x0020u, 0x20u,  5U, 5, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__OVT_IT \
	io0x1f8, error_IO_PORT_SUPP_IN__OVT_IT_has_no_byte_access, 0x01f8u, 0x01f8, 0x0040u, 0x40u,  6U, 6, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__OVT_SYNC \
	io0x1f8, error_IO_PORT_SUPP_IN__OVT_SYNC_has_no_byte_access, 0x01f8u, 0x01f8, 0x0080u, 0x80u,  7U, 7, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__OVT_WARN_IT \
	io0x1f8, error_IO_PORT_SUPP_IN__OVT_WARN_IT_has_no_byte_access, 0x01f8u, 0x01f8, 0x0100u, 0x01u,  8U, 0, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__OVT_WARN_SYNC \
	io0x1f8, error_IO_PORT_SUPP_IN__OVT_WARN_SYNC_has_no_byte_access, 0x01f8u, 0x01f8, 0x0200u, 0x02u,  9U, 1, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__UV_VDD5_IT \
	io0x1f8, error_IO_PORT_SUPP_IN__UV_VDD5_IT_has_no_byte_access, 0x01f8u, 0x01f8, 0x0400u, 0x04u, 10U, 2, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SUPP_IN__UV_VDD5_SYNC \
	io0x1f8, error_IO_PORT_SUPP_IN__UV_VDD5_SYNC_has_no_byte_access, 0x01f8u, 0x01f8, 0x0800u, 0x08u, 11U, 3, 0U, 8u, 0xf000u, 0x0000u
#define IO_PORT_MISC_IN__AIN_SUP \
	io0x1fa, error_IO_PORT_MISC_IN__AIN_SUP_has_no_byte_access, 0x01fau, 0x01fa, 0x00ffu, 0xffu,  0U, 0, 0U, 8u, 0xc000u, 0x0000u
#define IO_PORT_MISC_IN__STOP_MODE \
	io0x1fa, error_IO_PORT_MISC_IN__STOP_MODE_has_no_byte_access, 0x01fau, 0x01fa, 0x0100u, 0x01u,  8U, 0, 0U, 8u, 0xc000u, 0x0000u
#define IO_PORT_MISC_IN__RSTAT \
	io0x1fa, error_IO_PORT_MISC_IN__RSTAT_has_no_byte_access, 0x01fau, 0x01fa, 0x0200u, 0x02u,  9U, 1, 0U, 8u, 0xc000u, 0x0000u
#define IO_PORT_MISC_IN__INTERNAL_WU \
	io0x1fa, error_IO_PORT_MISC_IN__INTERNAL_WU_has_no_byte_access, 0x01fau, 0x01fa, 0x0400u, 0x04u, 10U, 2, 0U, 8u, 0xc000u, 0x0000u
#define IO_PORT_MISC_IN__LIN_WU \
	io0x1fa, error_IO_PORT_MISC_IN__LIN_WU_has_no_byte_access, 0x01fau, 0x01fa, 0x0800u, 0x08u, 11U, 3, 0U, 8u, 0xc000u, 0x0000u
#define IO_PORT_MISC_IN__PHYSTAT1 \
	io0x1fa, error_IO_PORT_MISC_IN__PHYSTAT1_has_no_byte_access, 0x01fau, 0x01fa, 0x1000u, 0x10u, 12U, 4, 0U, 8u, 0xc000u, 0x0000u
#define IO_PORT_MISC_IN__AAOUT_IN \
	io0x1fa, error_IO_PORT_MISC_IN__AAOUT_IN_has_no_byte_access, 0x01fau, 0x01fa, 0x2000u, 0x20u, 13U, 5, 0U, 8u, 0xc000u, 0x0000u
#define IO_PORT_SUPP_CFG__UV_VDDA_FILT_SEL \
	io0x1fc, io_byte_0x1fc, 0x01fcu, 0x01fc, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xffc0u, 0x0000u
#define IO_PORT_SUPP_CFG__UV_VS_FILT_SEL \
	io0x1fc, io_byte_0x1fc, 0x01fcu, 0x01fc, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xffc0u, 0x0000u
#define IO_PORT_SUPP_CFG__OV_VS_FILT_SEL \
	io0x1fc, io_byte_0x1fc, 0x01fcu, 0x01fc, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xffc0u, 0x0000u
#define IO_PORT_SUPP_CFG__OVT_FILT_SEL \
	io0x1fc, io_byte_0x1fc, 0x01fcu, 0x01fc, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xffc0u, 0x0000u
#define IO_PORT_SUPP_CFG__OVT_WARN_FILT_SEL \
	io0x1fc, io_byte_0x1fc, 0x01fcu, 0x01fc, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0xffc0u, 0x0000u
#define IO_PORT_SUPP_CFG__UV_VDD5_FILT_SEL \
	io0x1fc, io_byte_0x1fc, 0x01fcu, 0x01fc, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0xffc0u, 0x0000u
#define IO_PORT_TIMER_CFG__TIMER0_CHA_SEL \
	io0x1fe, io_byte_0x1fe, 0x01feu, 0x01fe, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_TIMER_CFG__TIMER0_CHB_SEL \
	io0x1fe, io_byte_0x1fe, 0x01feu, 0x01fe, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_TIMER_CFG__TIMER1_CHA_SEL \
	io0x1fe, error_IO_PORT_TIMER_CFG__TIMER1_CHA_SEL_has_no_byte_access, 0x01feu, 0x01fe, 0x0f00u, 0x0fu,  8U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_TIMER_CFG__TIMER1_CHB_SEL \
	io0x1fe, error_IO_PORT_TIMER_CFG__TIMER1_CHB_SEL_has_no_byte_access, 0x01feu, 0x01fe, 0xf000u, 0xf0u, 12U, 4, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__CLEAR_STOP \
	io0x200, io_byte_0x200, 0x0200u, 0x0200, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__SET_RSTAT \
	io0x200, io_byte_0x200, 0x0200u, 0x0200, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__CLEAR_RSTAT \
	io0x200, io_byte_0x200, 0x0200u, 0x0200, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__WUI \
	io0x200, io_byte_0x200, 0x0200u, 0x0200, 0x0018u, 0x18u,  3U, 3, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__SWITCH_VDDA_TO_5V \
	io0x200, io_byte_0x200, 0x0200u, 0x0200, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__PRUV_VS \
	io0x200, error_IO_PORT_MISC_OUT__PRUV_VS_has_no_byte_access, 0x0200u, 0x0200, 0x01c0u, 0xc0u,  6U, 6, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__PROV_VS \
	io0x200, error_IO_PORT_MISC_OUT__PROV_VS_has_no_byte_access, 0x0200u, 0x0200, 0x0600u, 0x06u,  9U, 1, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__PRUV_VDDA \
	io0x200, error_IO_PORT_MISC_OUT__PRUV_VDDA_has_no_byte_access, 0x0200u, 0x0200, 0x0800u, 0x08u, 11U, 3, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_MISC_OUT__SEL_TEMP \
	io0x200, error_IO_PORT_MISC_OUT__SEL_TEMP_has_no_byte_access, 0x0200u, 0x0200, 0xf000u, 0xf0u, 12U, 4, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_MISC2_OUT__AOUT_SUP \
	io0x202, io_byte_0x202, 0x0202u, 0x0202, 0x00ffu, 0xffu,  0U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_PORT_MISC2_OUT__OVT_DISABLE \
	io0x202, error_IO_PORT_MISC2_OUT__OVT_DISABLE_has_no_byte_access, 0x0202u, 0x0202, 0x0100u, 0x01u,  8U, 0, 1U, 8u, 0xf000u, 0x0000u
#define IO_PORT_MISC2_OUT__SWITCHOFFUV_VDD5 \
	io0x202, error_IO_PORT_MISC2_OUT__SWITCHOFFUV_VDD5_has_no_byte_access, 0x0202u, 0x0202, 0x0200u, 0x02u,  9U, 1, 1U, 8u, 0xf000u, 0x0000u
#define IO_PORT_MISC2_OUT__ENABLE_TX \
	io0x202, error_IO_PORT_MISC2_OUT__ENABLE_TX_has_no_byte_access, 0x0202u, 0x0202, 0x0400u, 0x04u, 10U, 2, 1U, 8u, 0xf000u, 0x0000u
#define IO_PORT_MISC2_OUT__EN_IN_AAIN \
	io0x202, error_IO_PORT_MISC2_OUT__EN_IN_AAIN_has_no_byte_access, 0x0202u, 0x0202, 0x0800u, 0x08u, 11U, 3, 1U, 8u, 0xf000u, 0x0000u
#define IO_PORT_STOPMD_CTRL__SEL_STOP_MODE \
	io0x204, io_byte_0x204, 0x0204u, 0x0204, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfffeu, 0x0000u
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFUV_VDDA \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xf800u, 0x0000u
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFOV_VDDA \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xf800u, 0x0000u
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFOV_VDDD \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xf800u, 0x0000u
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFUV_VS \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xf800u, 0x0000u
#define IO_PORT_STOPMD_CFG__PRE_SWITCHOFFOV_VS \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0010u, 0x10u,  4U, 4, 1U, 12u, 0xf800u, 0x0000u
#define IO_PORT_STOPMD_CFG__PRE_SBY_RCO32M \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0020u, 0x20u,  5U, 5, 1U, 12u, 0xf800u, 0x0000u
#define IO_PORT_STOPMD_CFG__PRE_SBY_RCO1M \
	io0x206, io_byte_0x206, 0x0206u, 0x0206, 0x0040u, 0x40u,  6U, 6, 1U, 12u, 0xf800u, 0x0000u
#define IO_PORT_STOPMD_CFG__STOPMD_CFG_ANA \
	io0x206, error_IO_PORT_STOPMD_CFG__STOPMD_CFG_ANA_has_no_byte_access, 0x0206u, 0x0206, 0x0780u, 0x80u,  7U, 7, 1U, 8u, 0xf800u, 0x0000u
#define IO_PORT_DIS_GTSM__DIS_GTSM \
	io0x208, io_byte_0x208, 0x0208u, 0x0208, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfffeu, 0x0000u
#define IO_PORT_CLOCK_CTRL__AC_SEL \
	io0x20a, io_byte_0x20a, 0x020au, 0x020a, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfffeu, 0x0000u
#define IO_UART__BRRD \
	io0x20c, error_IO_UART__BRRD_has_no_byte_access, 0x020cu, 0x020c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_UART__TRD \
	io0x20e, error_IO_UART__TRD_has_no_byte_access, 0x020eu, 0x020e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_UART__RRD \
	io0x210, error_IO_UART__RRD_has_no_byte_access, 0x0210u, 0x0210, 0xffffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_UART__LFCD \
	io0x212, error_IO_UART__LFCD_has_no_byte_access, 0x0212u, 0x0212, 0x00ffu, 0xffu,  0U, 0, 0U, 8u, 0x0000u, 0x0000u
#define IO_UART__LFDD \
	io0x212, error_IO_UART__LFDD_has_no_byte_access, 0x0212u, 0x0213, 0xff00u, 0xffu,  8U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_UART__TRE \
	io0x214, error_IO_UART__TRE_has_no_byte_access, 0x0214u, 0x0214, 0x0002u, 0x02u,  1U, 1, 1U, 8u, 0x0011u, 0x0000u
#define IO_UART__REE \
	io0x214, error_IO_UART__REE_has_no_byte_access, 0x0214u, 0x0214, 0x0004u, 0x04u,  2U, 2, 1U, 8u, 0x0011u, 0x0000u
#define IO_UART__ISB \
	io0x214, error_IO_UART__ISB_has_no_byte_access, 0x0214u, 0x0214, 0x0008u, 0x08u,  3U, 3, 1U, 8u, 0x0011u, 0x0000u
#define IO_UART__LBE \
	io0x214, error_IO_UART__LBE_has_no_byte_access, 0x0214u, 0x0214, 0x0020u, 0x20u,  5U, 5, 0U, 8u, 0x0011u, 0x0000u
#define IO_UART__LSE \
	io0x214, error_IO_UART__LSE_has_no_byte_access, 0x0214u, 0x0214, 0x0040u, 0x40u,  6U, 6, 0U, 8u, 0x0011u, 0x0000u
#define IO_UART__LTE \
	io0x214, error_IO_UART__LTE_has_no_byte_access, 0x0214u, 0x0214, 0x0080u, 0x80u,  7U, 7, 0U, 8u, 0x0011u, 0x0000u
#define IO_UART__MLS \
	io0x214, error_IO_UART__MLS_has_no_byte_access, 0x0214u, 0x0215, 0x0700u, 0x07u,  8U, 0, 1U, 8u, 0x0011u, 0x0000u
#define IO_UART__BSC \
	io0x214, error_IO_UART__BSC_has_no_byte_access, 0x0214u, 0x0215, 0x1800u, 0x18u, 11U, 3, 1U, 8u, 0x0011u, 0x0000u
#define IO_UART__LBC \
	io0x214, error_IO_UART__LBC_has_no_byte_access, 0x0214u, 0x0215, 0x2000u, 0x20u, 13U, 5, 1U, 8u, 0x0011u, 0x0000u
#define IO_UART__LSC \
	io0x214, error_IO_UART__LSC_has_no_byte_access, 0x0214u, 0x0215, 0x4000u, 0x40u, 14U, 6, 1U, 8u, 0x0011u, 0x0000u
#define IO_UART__LDC \
	io0x214, error_IO_UART__LDC_has_no_byte_access, 0x0214u, 0x0215, 0x8000u, 0x80u, 15U, 7, 1U, 8u, 0x0011u, 0x0000u
#define IO_UART__TRO \
	io0x216, error_IO_UART__TRO_has_no_byte_access, 0x0216u, 0x0217, 0x0100u, 0x01u,  8U, 0, 0U, 8u, 0x00ffu, 0x0000u
#define IO_UART__TRB \
	io0x216, error_IO_UART__TRB_has_no_byte_access, 0x0216u, 0x0217, 0x0200u, 0x02u,  9U, 1, 0U, 8u, 0x00ffu, 0x0000u
#define IO_UART__TSB \
	io0x216, error_IO_UART__TSB_has_no_byte_access, 0x0216u, 0x0217, 0x0400u, 0x04u, 10U, 2, 0U, 8u, 0x00ffu, 0x0000u
#define IO_UART__RSO \
	io0x216, error_IO_UART__RSO_has_no_byte_access, 0x0216u, 0x0217, 0x0800u, 0x08u, 11U, 3, 0U, 8u, 0x00ffu, 0x0000u
#define IO_UART__RSB \
	io0x216, error_IO_UART__RSB_has_no_byte_access, 0x0216u, 0x0217, 0x1000u, 0x10u, 12U, 4, 0U, 8u, 0x00ffu, 0x0000u
#define IO_UART__RRF \
	io0x216, error_IO_UART__RRF_has_no_byte_access, 0x0216u, 0x0217, 0x2000u, 0x20u, 13U, 5, 0U, 8u, 0x00ffu, 0x0000u
#define IO_UART__NBR \
	io0x216, error_IO_UART__NBR_has_no_byte_access, 0x0216u, 0x0217, 0x4000u, 0x40u, 14U, 6, 0U, 8u, 0x00ffu, 0x0000u
#define IO_UART__SBE \
	io0x216, error_IO_UART__SBE_has_no_byte_access, 0x0216u, 0x0217, 0x8000u, 0x80u, 15U, 7, 0U, 8u, 0x00ffu, 0x0000u
#define IO_PORT_ID__ID \
	io0x218, io_byte_0x218, 0x0218u, 0x0218, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0xf000u, 0x0000u
#define IO_PORT_ID__IDMASK \
	io0x218, error_IO_PORT_ID__IDMASK_has_no_byte_access, 0x0218u, 0x0218, 0x0fc0u, 0xc0u,  6U, 6, 1U, 8u, 0xf000u, 0x0000u
#define IO_PORT_DMA_RDA__DMA_RDA \
	io0x21a, error_IO_PORT_DMA_RDA__DMA_RDA_has_no_byte_access, 0x021au, 0x021a, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_DMA_RDB__DMA_RDB \
	io0x21c, error_IO_PORT_DMA_RDB__DMA_RDB_has_no_byte_access, 0x021cu, 0x021c, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_DMA_TX__DMA_TX \
	io0x21e, error_IO_PORT_DMA_TX__DMA_TX_has_no_byte_access, 0x021eu, 0x021e, 0xffffu, 0xffu,  0U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_MELIBUS_CTRL__MELIBUS_EN \
	io0x220, io_byte_0x220, 0x0220u, 0x0220, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_CTRL__MELIBUS_ACK_EN \
	io0x220, io_byte_0x220, 0x0220u, 0x0220, 0x0002u, 0x02u,  1U, 1, 1U, 12u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_CTRL__MELIBUS_DIS_PARITY \
	io0x220, io_byte_0x220, 0x0220u, 0x0220, 0x0004u, 0x04u,  2U, 2, 1U, 12u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_CTRL__MELIBUS_DIS_CRC \
	io0x220, io_byte_0x220, 0x0220u, 0x0220, 0x0008u, 0x08u,  3U, 3, 1U, 12u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_CTRL__SEL_TXD \
	io0x220, io_byte_0x220, 0x0220u, 0x0220, 0x0030u, 0x30u,  4U, 4, 1U, 12u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_CTRL__SEL_RX_IN \
	io0x220, io_byte_0x220, 0x0220u, 0x0220, 0x00c0u, 0xc0u,  6U, 6, 1U, 12u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_STAT__RD_BUFFER_VALID \
	io0x222, error_IO_PORT_MELIBUS_STAT__RD_BUFFER_VALID_has_no_byte_access, 0x0222u, 0x0222, 0x0001u, 0x01u,  0U, 0, 0U, 8u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_STAT__FRAME_RECEIVED \
	io0x222, error_IO_PORT_MELIBUS_STAT__FRAME_RECEIVED_has_no_byte_access, 0x0222u, 0x0222, 0x0002u, 0x02u,  1U, 1, 0U, 8u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_STAT__FRAME_TRANSMIT_RDY \
	io0x222, error_IO_PORT_MELIBUS_STAT__FRAME_TRANSMIT_RDY_has_no_byte_access, 0x0222u, 0x0222, 0x0004u, 0x04u,  2U, 2, 0U, 8u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_STAT__PARITY_ERROR \
	io0x222, error_IO_PORT_MELIBUS_STAT__PARITY_ERROR_has_no_byte_access, 0x0222u, 0x0222, 0x0008u, 0x08u,  3U, 3, 0U, 8u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_STAT__CRC_ERROR \
	io0x222, error_IO_PORT_MELIBUS_STAT__CRC_ERROR_has_no_byte_access, 0x0222u, 0x0222, 0x0010u, 0x10u,  4U, 4, 0U, 8u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_STAT__WRONG_BREAK \
	io0x222, error_IO_PORT_MELIBUS_STAT__WRONG_BREAK_has_no_byte_access, 0x0222u, 0x0222, 0x0020u, 0x20u,  5U, 5, 0U, 8u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_STAT__BREAK_IN_FRAME \
	io0x222, error_IO_PORT_MELIBUS_STAT__BREAK_IN_FRAME_has_no_byte_access, 0x0222u, 0x0222, 0x0040u, 0x40u,  6U, 6, 0U, 8u, 0xff00u, 0x0000u
#define IO_PORT_MELIBUS_STAT__TX_TIMEOUT \
	io0x222, error_IO_PORT_MELIBUS_STAT__TX_TIMEOUT_has_no_byte_access, 0x0222u, 0x0222, 0x0080u, 0x80u,  7U, 7, 0U, 8u, 0xff00u, 0x0000u
#define IO_PORT_LC_SEL1__LC0_SEL \
	io0x224, io_byte_0x224, 0x0224u, 0x0224, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL1__LC1_SEL \
	io0x224, io_byte_0x224, 0x0224u, 0x0224, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL1__LC2_SEL \
	io0x224, error_IO_PORT_LC_SEL1__LC2_SEL_has_no_byte_access, 0x0224u, 0x0224, 0x0f00u, 0x0fu,  8U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL1__LC3_SEL \
	io0x224, error_IO_PORT_LC_SEL1__LC3_SEL_has_no_byte_access, 0x0224u, 0x0224, 0xf000u, 0xf0u, 12U, 4, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL2__LC4_SEL \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL2__LC5_SEL \
	io0x226, io_byte_0x226, 0x0226u, 0x0226, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL2__LC6_SEL \
	io0x226, error_IO_PORT_LC_SEL2__LC6_SEL_has_no_byte_access, 0x0226u, 0x0226, 0x0f00u, 0x0fu,  8U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL2__LC7_SEL \
	io0x226, error_IO_PORT_LC_SEL2__LC7_SEL_has_no_byte_access, 0x0226u, 0x0226, 0xf000u, 0xf0u, 12U, 4, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL3__LC8_SEL \
	io0x228, io_byte_0x228, 0x0228u, 0x0228, 0x000fu, 0x0fu,  0U, 0, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL3__LC9_SEL \
	io0x228, io_byte_0x228, 0x0228u, 0x0228, 0x00f0u, 0xf0u,  4U, 4, 1U, 12u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL3__LC10_SEL \
	io0x228, error_IO_PORT_LC_SEL3__LC10_SEL_has_no_byte_access, 0x0228u, 0x0228, 0x0f00u, 0x0fu,  8U, 0, 1U, 8u, 0x0000u, 0x0000u
#define IO_PORT_LC_SEL3__LC11_SEL \
	io0x228, error_IO_PORT_LC_SEL3__LC11_SEL_has_no_byte_access, 0x0228u, 0x0228, 0xf000u, 0xf0u, 12U, 4, 1U, 8u, 0x0000u, 0x0000u
#define IO_TRIM1_LC__LC0_TRIM \
	io0x22a, io_byte_0x22a, 0x022au, 0x022a, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM1_LC__LC1_TRIM \
	io0x22a, error_IO_TRIM1_LC__LC1_TRIM_has_no_byte_access, 0x022au, 0x022a, 0x0fc0u, 0xc0u,  6U, 6, 1U, 8u, 0x7000u, 0x0000u
#define IO_TRIM1_LC__LOCK \
	io0x22a, io_byte_0x22b, 0x022au, 0x022b, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM2_LC__LC2_TRIM \
	io0x22c, io_byte_0x22c, 0x022cu, 0x022c, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM2_LC__LC3_TRIM \
	io0x22c, error_IO_TRIM2_LC__LC3_TRIM_has_no_byte_access, 0x022cu, 0x022c, 0x0fc0u, 0xc0u,  6U, 6, 1U, 8u, 0x7000u, 0x0000u
#define IO_TRIM2_LC__LOCK \
	io0x22c, io_byte_0x22d, 0x022cu, 0x022d, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM3_LC__LC4_TRIM \
	io0x22e, io_byte_0x22e, 0x022eu, 0x022e, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM3_LC__LC5_TRIM \
	io0x22e, error_IO_TRIM3_LC__LC5_TRIM_has_no_byte_access, 0x022eu, 0x022e, 0x0fc0u, 0xc0u,  6U, 6, 1U, 8u, 0x7000u, 0x0000u
#define IO_TRIM3_LC__LOCK \
	io0x22e, io_byte_0x22f, 0x022eu, 0x022f, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM4_LC__LC6_TRIM \
	io0x230, io_byte_0x230, 0x0230u, 0x0230, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM4_LC__LC7_TRIM \
	io0x230, error_IO_TRIM4_LC__LC7_TRIM_has_no_byte_access, 0x0230u, 0x0230, 0x0fc0u, 0xc0u,  6U, 6, 1U, 8u, 0x7000u, 0x0000u
#define IO_TRIM4_LC__LOCK \
	io0x230, io_byte_0x231, 0x0230u, 0x0231, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM5_LC__LC8_TRIM \
	io0x232, io_byte_0x232, 0x0232u, 0x0232, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM5_LC__LC9_TRIM \
	io0x232, error_IO_TRIM5_LC__LC9_TRIM_has_no_byte_access, 0x0232u, 0x0232, 0x0fc0u, 0xc0u,  6U, 6, 1U, 8u, 0x7000u, 0x0000u
#define IO_TRIM5_LC__LOCK \
	io0x232, io_byte_0x233, 0x0232u, 0x0233, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM6_LC__LC10_TRIM \
	io0x234, io_byte_0x234, 0x0234u, 0x0234, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0x7000u, 0x0000u
#define IO_TRIM6_LC__LC11_TRIM \
	io0x234, error_IO_TRIM6_LC__LC11_TRIM_has_no_byte_access, 0x0234u, 0x0234, 0x0fc0u, 0xc0u,  6U, 6, 1U, 8u, 0x7000u, 0x0000u
#define IO_TRIM6_LC__LOCK \
	io0x234, io_byte_0x235, 0x0234u, 0x0235, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7000u, 0x0000u
#define IO_PORT_LC_DISPU_DIAG__LC_DISPU \
	io0x236, error_IO_PORT_LC_DISPU_DIAG__LC_DISPU_has_no_byte_access, 0x0236u, 0x0236, 0x0fffu, 0xffu,  0U, 0, 1U, 8u, 0x8000u, 0x0000u
#define IO_PORT_LC_DISPU_DIAG__LCDIAG_SEL \
	io0x236, error_IO_PORT_LC_DISPU_DIAG__LCDIAG_SEL_has_no_byte_access, 0x0236u, 0x0236, 0x7000u, 0x70u, 12U, 4, 1U, 8u, 0x8000u, 0x0000u
#define IO_PORT_LC_OUT__LC_OUT \
	io0x238, error_IO_PORT_LC_OUT__LC_OUT_has_no_byte_access, 0x0238u, 0x0238, 0x0fffu, 0xffu,  0U, 0, 1U, 8u, 0xf000u, 0x0000u
#define IO_PORT_SELP_LC__SELP_LC \
	io0x23a, error_IO_PORT_SELP_LC__SELP_LC_has_no_byte_access, 0x023au, 0x023a, 0x0fffu, 0xffu,  0U, 0, 1U, 8u, 0xf000u, 0x0000u
#define IO_PORT_HSW_OUT__HSW_OUT \
	io0x23c, io_byte_0x23c, 0x023cu, 0x023c, 0x003fu, 0x3fu,  0U, 0, 1U, 12u, 0xffc0u, 0x0000u
#define IO_PORT_HSW_ENPD__HSW_ENPD \
	io0x23e, io_byte_0x23e, 0x023eu, 0x023e, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0xfff8u, 0x0000u
#define IO_TRIM_MISC__TR_TAU \
	io0x240, io_byte_0x240, 0x0240u, 0x0240, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0x6000u, 0x0000u
#define IO_TRIM_MISC__TR_HYS \
	io0x240, io_byte_0x240, 0x0240u, 0x0240, 0x000eu, 0x0eu,  1U, 1, 1U, 12u, 0x6000u, 0x0000u
#define IO_TRIM_MISC__LCDIAG_TRIM \
	io0x240, error_IO_TRIM_MISC__LCDIAG_TRIM_has_no_byte_access, 0x0240u, 0x0240, 0x03f0u, 0xf0u,  4U, 4, 1U, 8u, 0x6000u, 0x0000u
#define IO_TRIM_MISC__TR_OVT \
	io0x240, error_IO_TRIM_MISC__TR_OVT_has_no_byte_access, 0x0240u, 0x0240, 0x1c00u, 0x1cu, 10U, 2, 1U, 8u, 0x6000u, 0x0000u
#define IO_TRIM_MISC__LOCK \
	io0x240, io_byte_0x241, 0x0240u, 0x0241, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x6000u, 0x0000u
#define IO_PORT_MISC_TEST__LC_DIS \
	io0x242, io_byte_0x242, 0x0242u, 0x0242, 0x0001u, 0x01u,  0U, 0, 1U, 28u, 0xffe0u, 0x0000u
#define IO_PORT_MISC_TEST__HSW_DIS \
	io0x242, io_byte_0x242, 0x0242u, 0x0242, 0x0002u, 0x02u,  1U, 1, 1U, 28u, 0xffe0u, 0x0000u
#define IO_PORT_MISC_TEST__IDDQ_LC \
	io0x242, io_byte_0x242, 0x0242u, 0x0242, 0x0004u, 0x04u,  2U, 2, 1U, 28u, 0xffe0u, 0x0000u
#define IO_PORT_MISC_TEST__TEST_HVDIFF_EN \
	io0x242, io_byte_0x242, 0x0242u, 0x0242, 0x0008u, 0x08u,  3U, 3, 1U, 28u, 0xffe0u, 0x0000u
#define IO_PORT_MISC_TEST__DIS_TX_TIMEOUT \
	io0x242, io_byte_0x242, 0x0242u, 0x0242, 0x0010u, 0x10u,  4U, 4, 1U, 28u, 0xffe0u, 0x0000u
#define IO_PORT_AA_OUT__PRE_AA_OUT \
	io0x244, io_byte_0x244, 0x0244u, 0x0244, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfffeu, 0x0000u
#define IO_PORT_OUTB_AAIN__OUTB_AAIN \
	io0x246, io_byte_0x246, 0x0246u, 0x0246, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfffeu, 0x0000u
#define IO_PORT_AA_SEL__SEL_AA_OUT \
	io0x248, io_byte_0x248, 0x0248u, 0x0248, 0x0003u, 0x03u,  0U, 0, 1U, 12u, 0xfffcu, 0x0000u
#define IO_PORT_AA_IN__AA_IN_SYNC \
	io0x24a, error_IO_PORT_AA_IN__AA_IN_SYNC_has_no_byte_access, 0x024au, 0x024a, 0x0001u, 0x01u,  0U, 0, 0U, 8u, 0xfffeu, 0x0000u
#define IO_PORT_LIN_XCFG__LIN_XCFG \
	io0x24c, io_byte_0x24c, 0x024cu, 0x024c, 0x0001u, 0x01u,  0U, 0, 1U, 12u, 0xfffeu, 0x0000u
#define IO_PORT_LIN_XCFG_VALID__LIN_XCFG_VALID \
	io0x24e, error_IO_PORT_LIN_XCFG_VALID__LIN_XCFG_VALID_has_no_byte_access, 0x024eu, 0x024e, 0x0001u, 0x01u,  0U, 0, 0U, 8u, 0xfffeu, 0x0000u
#define IO_TRIM_VDD_EXT__TR_VDDIO \
	io0x250, io_byte_0x250, 0x0250u, 0x0250, 0x0007u, 0x07u,  0U, 0, 1U, 12u, 0x7ff8u, 0x0000u
#define IO_TRIM_VDD_EXT__LOCK \
	io0x250, io_byte_0x251, 0x0250u, 0x0251, 0x8000u, 0x80u, 15U, 7, 1U, 12u, 0x7ff8u, 0x0000u
#define IO_PORT_TEST_VDD_EXT__SWITCHOFFREG_VDDIO \
	io0x252, io_byte_0x252, 0x0252u, 0x0252, 0x0003u, 0x03u,  0U, 0, 1U, 28u, 0xfff0u, 0x0000u
#define IO_PORT_TEST_VDD_EXT__SHOVE_VDDIO \
	io0x252, io_byte_0x252, 0x0252u, 0x0252, 0x0004u, 0x04u,  2U, 2, 1U, 28u, 0xfff0u, 0x0000u
#define IO_PORT_TEST_VDD_EXT__IDDQ_REG_VDDIO \
	io0x252, io_byte_0x252, 0x0252u, 0x0252, 0x0008u, 0x08u,  3U, 3, 1U, 28u, 0xfff0u, 0x0000u

/*****************************************************************************/
/* 3.4 Dummy ports. */

/* MlxCCT private. */

extern volatile uint16_t iodummy __attribute__((addr(0x0)));
extern volatile uint8_t  iodummy8 __attribute__((addr(0x0)));

#define IO_ADC_SAR__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ADC_SAR__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_AWD__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_AWD__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_CTIMER0__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER0__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_CTIMER1__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_CTIMER1__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_EEPROM_FLASH__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_EEPROM_FLASH__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_FUNC_TEST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_FUNC_TEST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_IWD__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_IWD__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_MLX16__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MLX16__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_MUPET__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_MUPET__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_AA_IN__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_IN__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_AA_OUT__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_OUT__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_AA_SEL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_AA_SEL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_ADC_CTRL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_CTRL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_ADC_TRIM_TEST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ADC_TRIM_TEST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_CLOCK_CTRL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_CLOCK_CTRL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_DIS_GTSM__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DIS_GTSM__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_DMA_RDA__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDA__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_DMA_RDB__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_RDB__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_DMA_TX__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_DMA_TX__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_HSW_ENPD__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_ENPD__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_HSW_OUT__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_HSW_OUT__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_ID__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_ID__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_LC_DISPU_DIAG__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_DISPU_DIAG__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_LC_OUT__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_OUT__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_LC_SEL1__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL1__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_LC_SEL2__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL2__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_LC_SEL3__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LC_SEL3__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_LIN_XCFG__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_LIN_XCFG_VALID__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XCFG_VALID__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_LIN_XKEY__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_LIN_XKEY__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_MELIBUS_CTRL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_CTRL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_MELIBUS_STAT__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MELIBUS_STAT__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_MISC2_OUT__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC2_OUT__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_MISC_IN__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_IN__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_MISC_OUT__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_OUT__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_MISC_TEST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_MISC_TEST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_OUTB_AAIN__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_OUTB_AAIN__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_SELP_LC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SELP_LC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_SPARE_TEST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SPARE_TEST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_SSCM_CONF__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SSCM_CONF__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_STEP_CONF__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STEP_CONF__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_STOPMD_CFG__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CFG__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_STOPMD_CTRL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_STOPMD_CTRL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_SUPP2_TEST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP2_TEST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_SUPP_CFG__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_CFG__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_SUPP_IN__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_IN__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_SUPP_TEST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_SUPP_TEST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_TEST_ADC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_ADC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_TEST_VDD_EXT__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TEST_VDD_EXT__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PORT_TIMER_CFG__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PORT_TIMER_CFG__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_MASTER1__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_MASTER1__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE1__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE1__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE10__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE10__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE11__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE11__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE2__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE2__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE3__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE3__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE4__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE4__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE5__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE5__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE6__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE6__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE7__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE7__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE8__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE8__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_PWM_SLAVE9__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_PWM_SLAVE9__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_RAM_BIST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RAM_BIST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_ROM_BIST__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_BIST__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_ROM_SHELL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_ROM_SHELL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_RST_CTRL__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_RST_CTRL__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_STIMER__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_STIMER__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM1_LC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM1_LC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM2_LC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM2_LC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM3_LC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM3_LC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM4_LC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM4_LC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM5_LC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM5_LC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM6_LC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM6_LC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM_BG_BIAS__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_BG_BIAS__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM_MISC__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_MISC__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM_RCO1M__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO1M__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM_RCO32M__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_RCO32M__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM_VDD__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_TRIM_VDD_EXT__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_TRIM_VDD_EXT__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_UART__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_UART__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U

#define IO_VERSION__ \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY2  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY3  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY4  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY5  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY6  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY7  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY8  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY9  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY10  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY11  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY12  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY13  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY14  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY15  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY16  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U
#define IO_VERSION__DUMMY17  \
	iodummy, iodummy8, 0xFFFFU, 0xFFFF, 0x0000U, 0x00U,  0U, 0, 1U, 0u, 0x0000U, 0x0000U


/*****************************************************************************/
/* 4. Statistics. */

/* Report of values which might be of interest. */

/*
 * Range: 0x0002u to 0x0252u
 * Variables: 217
 * Fields: 777
 * Occupation: 88.85% (385.5 bytes)
 */

#endif /* IO1_H */

/* EOF */
