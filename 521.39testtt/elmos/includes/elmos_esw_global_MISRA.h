/* PRQA S 0292 EOF #justification: As the tags are spread all over the code files, 
                   the message is switched off globally. This tags are nescessary
                   e.g. doxygen documentations */
/***************************************************************************//**
 * @file     elmos_esw_global_MISRA.h
 *
 * @creator   siko
 * @created   17.03.2020
 *
 * @brief     GLOABAL MISRA rule exceptions
 *
 * $Id: elmos_esw_global_MISRA.h 8439 2021-03-01 11:23:42Z poe $
 *
 * $Revision: 8439 $
 *
 
 PRQA S XXXX ++ #justification: why is it not suitable to satisfy the rule

 Rule: <PRQA#> MISRA <Category a/r/m> <Dir/Rule-X.Y>
 <QA-C Text of rule violation>
 Circumstances: <why does the rule violation occur>
 Potential consequences: <...>
 Alternative safety assurance: <...>
 

 * *****************************************************************************/

#ifndef ELMOS_ESW_GLOBAL_MISRA_H_
#define ELMOS_ESW_GLOBAL_MISRA_H_

/* Rule: 0292 MISRA r Dir-1.1
     Source file <TEXT> has comments containing one of the characters dollar, at or accent
   Circumstances: Special characters are used as dedicated tags by tools, which 
                  extracts additional information from code files for special purposes, 
                  like doxygen for documentation generation. 
   Potential consequences: Characters may not be representable in all environments.
   Alternative safety assurance: none */


/* PRQA S 0320 ++ #justification: Declarations for indexes in for statements are commonly 
                   used in c99 projects.*/
/* Rule: 0320 MISRA r Rule-1.1 and Rule 1.2
     [C99] Declaration within 'for' statement.
   Circumstances: Declarations for indexes in for statements are commonly 
                  used in c99 projects.
   Potential consequences: Source-code is not compatible to c90 projects.
   Alternative safety assurance: none */
   
   
/* PRQA S 0750 ++ #justification: Union types are used due to limited system resources 
                   (packed access to MEMORY configuration data and RAM) and constraints
                   by hardware design (registers access).*/
/* Rule: 0750 MISRA a Rule-19.2
     A union type specifier has been defined.
   Circumstances: Union types are used due to limited system resources (packed access to MEMORY             
                  configuration data and RAM) and constraints by hardware design (registers 
				          access). Union types are used all over the code and thus the message is 
				          switched off globally.
   Potential consequences: When using union data types, special care has to be taken regarding 
                           padding, alignment, endianness and bit order, to avoid 
                           misinterpretation of memory.
   Alternative safety assurance: none */
   
 
/* PRQA S 0759 ++ #justification: Union types are used due to limited system resources 
                   (packed access to MEMORY configuration data and RAM) and constraints
                   by hardware design (registers access).*/
/* Rule: 0759 MISRA a Rule-19.2
     An object of union type has been defined.
   Circumstances: Union types are used due to limited system resources (packed access to MEMORY             
                  configuration data and RAM) and constraints by hardware design (registers 
				          access). Union types are used all over the code and thus the message is 
				          switched off globally.
   Potential consequences: When using union data types, special care has to be taken regarding 
                           padding, alignment, endianness and bit order, to avoid 
                           misinterpretation of memory.
   Alternative safety assurance: none */

   
/* PRQA S 0828 ++ #justification: All active ESW project uses ISO-C99  */
/* Rule: 0828 MISRA r Dir-1.1
     [L] More than 8 levels of nested conditional inclusion - program does not conform strictly to ISO:C90.
   Circumstances: In highly reusable source code, there may be a need for some nested precompiler 
                  conditions that break the limit of level 8.
   Potential consequences: Source-code is not compatible to c90 projects.
   Alternative safety assurance: Compiler will issue an error when higher number of nested conditions are 
                                 not supported. */ 
                         
                         
/* PRQA S 0930 ++ #justification: It is a defined behavior in C99  */
/* Rule: 0930 MISRA r Dir-1.1 / a Rule-1.2
     [C99] Trailing comma at the end of an enumerator-list.
   Circumstances: Editing enums could lead to this finding. It is a defined behavior in C99.
   Potential consequences: Source-code is not compatible to c90 projects.
   Alternative safety assurance: Compiler will issue an error when trailing commas are not supported. */ 
  

/* PRQA S 1026 ++ #justification: @-characters are used to define absolute memory addresses which is 
                   acceptable in embedded SW programming.  */
/* Rule: 1026 MISRA r Dir-1.1 / a Rule-1.2
     [E] The indicated @word construct has been ignore
   Circumstances: A compiler extension with the @-character has been detected in the code. There are used 
                  to define absolute memory addresses which is acceptable in embedded SW programming. 
   Potential consequences: The code maybe not portable to other compiler which do not support this language 
                           extension.
   Alternative safety assurance: none */  
   
   
/* PRQA S 1031 ++ #justification: Arrays and Structs that are not declared as constant expression, 
                   can be initialized by e.g. function parameters. This is a defined behavior in C99.*/
/* Rule: 1031 MISRA r Dir-1.1 / a Rule-1.2
     [C99] Initializer for 'struct', 'union' or array type is not a constant expression.
   Circumstances: Arrays and Structs that are not declared as constant expression, can be initialized by 
                  e.g. function parameters. Also it is defined behavior in C99.
   Potential consequences: Source-code is not compatible to c90 projects.
   Alternative safety assurance: none */
      
   
/* PRQA S 1055 ++ #justification: Inline functions are commonly used having both, a well structured and 
                   speed optimized code. It is a defined behavior in C99.*/
/* Rule: 1055 MISRA r Dir-1.1 / a Rule-1.2
     [C99] The keyword 'inline' has been used.
   Circumstances: Inline functions are commonly used having both, a well structured and speed optimized code. 
                  It is a defined behavior in C99.
   Potential consequences: Source-code is not compatible to c90 projects.
   Alternative safety assurance: Compiler will issue an error when inlining is not supported. */
   
   
/* PRQA S 1056 ++ #justification: Type '_Bool' is a feature of ISO:C99. All our common used compilers 
                  are configured to support C99.*/
/* Rule: 1056 MISRA r Dir-1.1 / a Rule-1.2
     [C99] The keyword '_Bool' has been used.
   Circumstances: Type '_Bool' is a feature of ISO:C99. This syntax is not supported in ISO:C90. 
                  All our common used compilers are configured to support C99.
   Potential consequences: Source-code is not compatible to c90 projects.
   Alternative safety assurance: Compiler will issue an error, if keyword is not understood or supported. */
   
   
/* This is an rcma cross file check rule. This needs to be switched off in rcf file in both mentioned rules. */
/* PRQA S 1509 ++ #justification: Message is caused by inline functions in header files. 
                   The compiler demands this programming style. */
/* Rule: 1509 MISRA r Rule-1.3 / r Rule-8.6
     <fcn_name> has external linkage and has multiple definitions.
   Circumstances: Message is caused by inline functions in header files. The compiler demands this
                  programming style. QA-C checks this rule as a cross-module analysis. 
                  Thus the message has to be switched off globally.
   Potential consequences: Differing symbols with the same identifier are declared and the linker may 
                           chose the wrong one.
   Alternative safety assurance: If not caused by an inline function, the Linker emits an error like: 
                                 [symbol <fcn_name> multiply defined] e.g. COSMIC */ 


/* PRQA S 3387 ++ #justification: Incrementing and decrementing are commonly used constructs to write 
                  compact code. Also it is defined behavior in C99. */
/* Rule: 3387 MISRA a Rule-13.3
     A full expression containing an increment (++) or decrement (--) operator should have no 
     potential side effects other than that caused by the increment or decrement operator.
   Circumstances: Incrementing and decrementing are commonly used constructs to write compact code. 
                  Also it is defined behavior in C99.
   Potential consequences: Potential unwanted side effects and source-code is not compatible to c90 projects.
   Alternative safety assurance: Software developer has to be careful by using this constructs. 
                                 Software Unit-Tests can also find wrong usage. */         


/* PRQA S 3406 ++ #justification: The compiler demands this programming style. */
/* Rule: 3406 MISRA r Rule-8.6
     Object/function '%s', with external linkage, has been defined in a header file.
   Circumstances: Message is caused by inline functions in header files. The compiler demands this 
                  programming style. QA-C checks this rule localy in the header files. Thus the message 
                  is to be switched off for inline functions in header files localy.
   Potential consequences: If no mechanism is implemented to prevent multiple includes of a header file, 
                           symbols may multiply be defined.
   Alternative safety assurance: If not caused by an inline function, the Linker emits an error like: 
                                 [symbol <fcn_name> multiply defined] e.g. COSMIC*/   

/* PRQA S 0292 ++ #justification: As the tags are spread all over the code files, 
                   the message is switched off globally. This tags are nescessary
                   e.g. doxygen documentations */
                                 
#endif /* ELMOS_ESW_GLOBAL_MISRA_H_ */
