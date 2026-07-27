/**
 * @file
 * @brief Amalthea Software Platform
 * @internal
 *
 * @copyright (C) 2015-2016 Melexis N.V.
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
 * @ingroup amalthea_platform
 *
 * @details
 */

#ifndef COMPILER_ABSTRACTION_H
#define COMPILER_ABSTRACTION_H

/* Abstraction for expressing likelyness of conditions: only has effect when optimization for speed is on (O2,O3,Ofast) */
#define LIKELY(x)    __builtin_expect (!!(x), 1)
#define UNLIKELY(x)  __builtin_expect (!!(x), 0)

/* Abstraction for inlining */
#ifndef INLINE
#	ifdef UNITTEST
#		define INLINE
#	else
#		define INLINE  __attribute__((always_inline)) inline
#	endif
#else
#	warning "An existing definition of INLINE exists!"
#endif /* INLINE */

/* Abstraction for no-inline */
#ifndef NOINLINE
#	ifdef UNITTEST
#		define NOINLINE
#	else
#		define NOINLINE  __attribute__((noinline))
#	endif
#else
#	warning "An existing definition of NOINLINE exists!"
#endif /* NOINLINE */

/* Static */
#ifndef STATIC
#	ifdef UNITTEST
#		define STATIC
#	else
#		define STATIC static
#	endif
#else
#	warning "An existing definition of STATIC exists!"
#endif

/* Const */
#ifndef CONST
#	ifdef UNITTEST
#		define CONST
#	else
#		define CONST const
#	endif
#else
#	warning "An existing definition of CONST exists!"
#endif

/* Interrupt */
#ifndef INTERRUPT
#	ifdef UNITTEST
#		define INTERRUPT
#	else
#		define INTERRUPT __attribute__((interrupt))
#	endif
#else
#	warning "An existing definition of INTERRUPT exists!"
#endif

/* Avoid GCC warning about parameter to function not being used */
#define UNUSED(x) ((void)(x))


#endif /* COMPILER_ABSTRACTION_H */
