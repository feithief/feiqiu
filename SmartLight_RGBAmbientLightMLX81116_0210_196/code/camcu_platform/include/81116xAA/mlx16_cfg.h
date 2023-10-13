/**
 * @file
 * @brief Header file determining the right compilation flags for mathlib
 * @internal
 *
 * @copyright (C) Melexis N.V.
 * git flash 137e5489
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
 * @ingroup mathlib
 *
 * @details
 */

#ifndef MLX16_CFG_H_
#define MLX16_CFG_H_

/* Check if MLX16 co-processor is available */
#if defined (__MLX16_X8__) || defined (__MLX16_FX__) \
    || defined (__MLX16_EX__)
#define HAS_MLX16_COPROCESSOR
#endif

/* Check if new extra instructions are available, e.g. FSB / SFB  */
#if defined (__MLX16_8__) || defined (__MLX16_X8__) || defined (__MLX16_FX__) \
    || defined (__MLX16_E8__) || defined (__MLX16_EX__)
#define HAS_MLX16_FSB_SFB_INSTRUCTIONS
#endif

#endif /* MLX16_CFG_H_ */
