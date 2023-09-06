// PRQA S 292 ++
/***************************************************************************//**
 * @file      Lin_MISRA.h
 *
 * @creator		sbai
 * @created		29.06.2016
 *
 * @brief  		Project dependent MISRA rule exceptions
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 *
 * $Id: Lin_MISRA.h 8305 2021-02-03 13:29:35Z poe $
 *
 * $Revision: 8305 $
 *
 ******************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
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
 ******************************************************************************
 *
 * @misra{M3CM Dir-1.1. - PRQA Msg 105\,828\,930\, 1053 and 1055,
 * The IAR C/C++ Compiler for MSP430 Version 5.40.2 (5.40.2.50380) is compatible to ISO:C99.,
 * A non-C99-compatible compiler will not be able to compile the code.,
 * None}
 * _____
 *
 * @misra{M3CM Dir-1.1. - PRQA Msg 292,
 * The characters '$'\, '@' and "`" are often used by version control tools like SVN or CVS\,
 * code documentation tools like doxygen and many other tools. It does not affect the code.,
 * Wrong displayed letters in comments.,
 * None}
 * _____
 *
 * @misra{M3CM Dir-2.2. - PRQA Msg 2981,
 * It is a a good programming style to initialize variables at the declaration\,
 * even though they are not modified before been used. This prevents the usage
 * of uninitialized variables\, which can be dangerous.,
 * Unnecessary code.,
 * None}
 * _____
 *
 * @misra{M3CM Dir-11.5. - PRQA Msg 316 and 317,
 * One of the main concept of the Elmos LIN Driver is to let the user reserve the
 * memory for it and pass it as a pointer to every function as the first argument.
 * This way it is assured\, that there are no conflicts between the memory management
 * of the bootloader and user application\, because both are using the same driver.
 * As tradeoff the LIN Driver casts this memory to it's internal data structure.
 * Elmos took care of the correct data alignment\, so the reserved memory can used
 * like beeing returned by function malloc\, calloc and realloc.,
 * Alignment conflicts if it has not been taken care off.,
 * The reserved memory is tested against NULL and correct alignment.}
 * _____
 *
 * @misra{M3CM Dir-10.5. - PRQA Msg 4342,
 * The Elmos LIN Driver only uses enums with unsigned values.,
 * Unrecognized type cast when using enums in calculations.,
 * Set up a project rule to only use unsigned enums values.}
 * _____
 *
 * @misra{M3CM Dir-1.2. and 6.1 - PRQA Msg 635,
 * The IAR C/C++ Compiler for MSP430 Version 5.40.2 (5.40.2.50380) supports bit-fields
 * with other types then int. This language extension is not portable but supported
 * by most of the modern c-compilers.,
 * Unportable extension\, if the compiler does not support this language extension.,
 * None\, the compiler won't compile the code if this language extension is not supported.}
 * _____
 *
 * @misra{M3CM Dir-19.2. - PRQA Msg 750 and 759,
 * There are useful cases\, where the usage of a union is helpful and efficient. Like
 * access to an register on bit level and as uintX_t.,
 * Can be cause problems\, if the developer does not know how to use it correctly.,
 * None.}
 * _____
 *
 * @misra{M3CM Rule-15.4. - PRQA Msg 771,
 * The usage of more then one break statement can be very efficient\, while
 * the code stays readable. It should not be prohibited.,
 * Can make the code unstructured.,
 * It is a good programming directive to make sure the code is structured properly\,
 * regardless of whether one or more break statement is used.}
 * _____
 *
 * @misra{M3CM Rule-8.10. - PRQA Msg 3243,
 * It is a good practice to declare a function inline in a c-file and extern in a
 * header file to give the compiler the possibility to inline function outside of the
 * containing module. On this way a compiler like the IAR compiler\, which supports
 * "multi-file compilation"\, is able to inline generic functions\, i.e. like memory
 * write/copy/etc functions\, to other modules.,
 * The compiler will at least warn you about an unexpected behavior or just
 * don't inline the function.,
 * None.}
 * _____
 *
 * @misra{M3CM Rule-13.3. - PRQA Msg 3440 and 3441,
 * It is common practice to use the increment and decrement operator.,
 * None.,
 * A software developer should know how to properly use this operands.}
 * _____
 *
 * @misra{M3CM Dir-4.9. - PRQA Msg 3453,
 * Often it is more efficient to use macros for atomic operations.,
 * No type-checking.,
 * None.}
 *
 * _____
 *
 * @misra{M3CM Dir-1.1. - PRQA Msg 0778,0776,
 * ISO:C90 minimum standard is to have , first 31 characters to be unique
 * Micro Vision supports C99, We can have similar characters in start
 * None.}
 * _____
 *
 * @misra{M3CM Rule-1.3. - PRQA Msg 0602,0603,
 * The Macros in header files are used to avoid any possible conflict with other source macros
 * We have tested it and we have no conflict with the system macros
 * None.}
 * _____
 *
 * @misra{M3CM Dir-1.1. - PRQA 1056
 * Micro Vision supports C99. This is supported in ISO:C99
 * None.}
 * _____
 *
 * @misra{M3CM Dir-1.1. - PRQA 0857
 * Our C implementation standard is ISO:C99, in which the limit for macros in a file are of 4095 macros. 
 * None.}
 * _____
 *
 * @misra{M3CM Dir-1.1. - PRQA 0314
 * We have a generic interface for pointer - to - object. This generic interface help us cast 
 * pointer-to-object to pointer-to-void safely vice versa.
 * None.}
 * _____
 *
 * @misra{M3CM Dir-1.1. - PRQA 0604
 * Block - Statement is allowed in ISO:C99, it is a safe block of statements
 * None.}
 * _____
 *
 * @misra{M3CM Dir-1.1. - PRQA 0320
 * It is ISO:C99, in this standard we can declare and initialize counter variable inside the for loop
 * None.}
 * _____
 *
 * @misra{M3CM Rule-11.4. - PRQA 0306, 0303, 0305
 * Cast between a pointer to object and an integral type.
 * We need to perform pointer to object and an integral type, because we are on hardware level implementation
 * None.}
 * _____
 *
 * @misra{M3CM Rule-6.2. - PRQA 3660
 * Warning is occuring because of defining enums as a bitfield. Bitfield is unsigned by nature on contrary enums are signed
 * We are ignoring it gloabally, but this warning is still in discussion for future improvements
 * None.}
 * _____
 *
 * @misra{M3CM Rule-6.1., Dir-1.1. - PRQA 0634
 * Warning is occuring because of defining enums as a bitfield. Bitfield is unsigned by nature on contrary enums are signed
 * We are ignoring it gloabally, but this warning is still in discussion for future improvements
 * None.}
 * _____
 *
 * @misra{M3CM Rule-20.13. - PRQA 3115
 * #warning and #error is being used in our project which is not supported by compiler
 * These are compiler extensions which we can use without any problems.(tested solution)
 * None.}
  * _____
 *
 * @misra{M3CM Rule-1.2., Dir-1.1.  - PRQA 1008
 * #warning and #error is being used in our project which is not supported by compiler
 * These are compiler extensions which we can use without any problems.(tested solution)
 * None.}

 ******************************************************************************/
// PRQA S 292 --

#ifndef LIN_MISRA_H_
#define LIN_MISRA_H_

// PRQA S 0105 ++
// PRQA S 0828 ++
// PRQA S 0930 ++
// PRQA S 1053 ++
// PRQA S 1055 ++

// PRQA S 0292 ++

// PRQA S 2981 ++

// PRQA S 0316 ++
// PRQA S 0317 ++

// PRQA S 4342 ++

// PRQA S 0635 ++

// PRQA S 0750 ++
// PRQA S 0759 ++

// PRQA S 0771 ++

// PRQA S 3243 ++

// PRQA S 3440 ++
// PRQA S 3441 ++

// PRQA S 3453 ++

// PRQA S 0778 ++
// PRQA S 0776 ++
// PRQA S 0779 ++

// PRQA S 0602 ++
// PRQA S 0603 ++

// PRQA S 1056 ++

// PRQA S 4095 ++

// PRQA S 0857 ++

// PRQA S 0314 ++

// PRQA S 0604 ++

// PRQA S 0320 ++

// PRQA S 0306 ++

// PRQA S 0303 ++

// PRQA S 0305 ++

// PRQA S 3660 ++

// PRQA S 0634 ++

// PRQA S 3115 ++

// PRQA S 1008 ++


#endif /* LIN_MISRA_H_ */





/*! @} ELMOS_LIN_DRIVER */
