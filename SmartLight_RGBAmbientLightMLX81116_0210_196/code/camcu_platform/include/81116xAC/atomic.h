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

#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <stdint.h>
#include <builtin_mlx16.h>          /* from mathlib */
#include <compiler_abstraction.h>

#if defined(__COVERITY__)
void __coverity_exclusive_lock_acquire__(void * lock);
void __coverity_exclusive_lock_release__(void * lock);
#endif

#if defined(__POLYSPACE__)
void fenter_critical(void);
void fexit_critical(void);
#endif


#if !defined(__DOXYGEN__)

#if defined(__COVERITY__)
static int16_t m_lock;
#endif

/*
 * Restores previously saved CPU status
 */
static INLINE void mlx16_restore_status (const uint16_t *pm)
{
    uint16_t status = *pm;      /* get saved M value */
    builtin_mlx16_set_status(status);
}


static INLINE uint16_t mlx16_di_keep_mode(void)
{
    builtin_mlx16_disable_interrupts();
    return 1;
}


/*
 * Enter system mode with all interrupt disabled (CPU prio is set to 0)
 */
static INLINE uint16_t mlx16_di_enter_system_mode(void)
{
    __asm__ __volatile__ (".word OPCODE_CALL_FP_ENTER_SYSTEM_MODE_PRIO_0");
    return 1;
}

/*
 * Enter system mode without changing current CPU priority
 */
static INLINE uint16_t mlx16_enter_system_mode_keep_prio(void)
{
    __asm__ __volatile__ (".word OPCODE_CALL_FP_ENTER_SYSTEM_MODE_KEEP_PRIO");
    return 1;
}

#endif  /* !__DOXYGEN__ */

/**
 * \file
 *
 */

/** \defgroup atomic <atomic.h> Code Sections

    The macros in this header file create Code Sections that can be:
    - Atomic, i.e. executed without any interruption
    - System, i.e. CPU is switched temporarily into System mode
    - or both: Atomic and System

    The term "Atomic" in this context means that code inside the section
    can not be interrupted.

    \code
    #include <atomic.h>
    \endcode

   \note The macros in this header file require the ISO/IEC 9899:1999
    ("ISO C99") feature of for loop variables that are declared inside
    the for loop itself.  For that reason, this header file can only
    be used if the standard level of the compiler (option --std=) is
    set to \c gnu99.

 */

/** \def ENTER_SECTION()
    \ingroup atomic

    \brief Creates a code section in combination with EXIT_SECTION()

    Depending on `TYPE` argument the section can be:
    
    @param section_type
    - ::ATOMIC_KEEP_MODE, i.e. executed without any interruption
    - ::SYSTEM_MODE, i.e. CPU is switched temporarily into System mode
    - or both: Atomic and System ::ATOMIC_SYSTEM_MODE

  \code

#include <atomic.h>

volatile uint16_t ctr;

void simple_timer1_it(void) __attribute__ ((interrupt));
void simple_timer1_it(void)
{
  ctr--;
}

...
int
main(void)
{
   ...
   ctr = 0x200;
   start_timer();

   uint16_t ctr_copy;
   do
   {
      ENTER_SECTION(ATOMIC_KEEP_MODE);
      ctr_copy = ctr;
      EXIT_SECTION();
   }
   while (ctr_copy != 0);
   ...
}
    \endcode

    This will install the appropriate interrupt protection before
    accessing variable \c ctr, so it is guaranteed to be consistently
    tested.
*/
#if defined(__DOXYGEN__)
#define ENTER_SECTION(TYPE)
#define EXIT_SECTION()
#elif defined(__COVERITY__)
#define ENTER_SECTION(TYPE)    __coverity_exclusive_lock_acquire__(&m_lock)
#define EXIT_SECTION()         __coverity_exclusive_lock_release__(&m_lock)
#elif defined(__POLYSPACE__)
#define ENTER_SECTION(TYPE)    fenter_critical()
#define EXIT_SECTION()         fexit_critical()
#else
#define ENTER_SECTION(TYPE) \
        { \
            uint16_t m_reg_save __attribute__((cleanup(mlx16_restore_status))) = builtin_mlx16_get_status(); \
            TYPE
#define EXIT_SECTION() \
        }
#endif

#if defined(__DOXYGEN__)
/** Atomic Code Section: CPU priority is set to 0 (interrupt disabled).
    CPU mode (System or User) remains unchanged. When CPU leaves the
    section, previous CPU priority is restored.
*/
#define ATOMIC_KEEP_MODE


/** Atomic Code Section executed in System mode: CPU priority is set to 0 (interrupt
    disabled) and CPU is switched to the System mode. When CPU leaves the
    section, previous CPU priority and mode are restored.
*/
#define ATOMIC_SYSTEM_MODE


/** System Mode Section: CPU is switched to System mode while code in this
    section is executed. When CPU leaves the section, previous CPU mode is
    restored.
    
    @warning Code in this ::SYSTEM_MODE section is **not atomic** and can be
    interrupted. Use ::ATOMIC_SYSTEM_MODE or ::ATOMIC_KEEP_MODE instead if
    atomic property is needed.
*/
#define SYSTEM_MODE

#else
#define ATOMIC_KEEP_MODE        mlx16_di_keep_mode()
#define ATOMIC_SYSTEM_MODE      mlx16_di_enter_system_mode()
#define SYSTEM_MODE             mlx16_enter_system_mode_keep_prio()

#endif  /* __DOXYGEN__ */

#endif  /* ATOMIC_H_ */
