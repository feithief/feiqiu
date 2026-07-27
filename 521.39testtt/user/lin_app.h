 /* ------------------------------------------------------------------- 
  * @file  lin_app.h
  * @brief lin application layer header (header)
  *
  * Purpose:
  *    
  *
  * @section history_sec Revision History
  * $Id: lin_app.h, v 1.0 2018/09/13 $ 
  * $Id: lin_app.h, v 1.1 2018/09/30 $ 
  *
  * @verbatim
  * ------------------------------------------------------------------- 
  * Date     B/F Who Comment
  * ------------------------------------------------------------------- 
  * 2018/09/13 (F) JEFFERY.WU initial version
  * 2018/09/30 (F) JEFFERY.WU add b4 diag service structure and definition
  * ------------------------------------------------------------------- 
  * 
  *
  * @creator      JEFFERY.WU
  * @created      2018/09/30
  * 
  * @project      521.36
  * @requested    -
  * @inspector    - 
  * @verification -
  * @version      $Revision: V1.0 $
  * @version      $Revision: V1.1 $
  * 
  *
  * ------------------------------------------------------------------- 
  */

#ifndef LIN_APP_H_
#define LIN_APP_H_
/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */

#include "genLinConfig.h"
//#include "lin_driver_api.h"
#include "stdint.h"
#include "io_e52139b.h"
#include "CommonInterruptHandler.h"

//#include "mod_sys.h"
#include "el_types.h"
#include "vic_InterruptHandler.h"
#include "linsci_InterruptHandler.h"

#ifndef LIN_TYPES
#define LIN_TYPES
typedef unsigned char  l_bool;   /* Boolean Datatype */
typedef unsigned char  l_u8;     /* 8Bit Datatype */
typedef unsigned short l_u16;    /* 16Bit Datatype */
typedef unsigned long  l_u32;    /* 32Bit Datatype */
typedef unsigned short l_irqmask, l_ioctl_op;                                            /* Datatypes for special LIN functions */
typedef unsigned short l_signal_handle, l_flag_handle, l_ifc_handle, l_schedule_handle;  /* Datatypes for special LIN functions */
#endif /* end #ifndef LIN_TYPES */
/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */

/*---------------------------- LIN DATA DUMP CONTROL DEFINITION -----------------------*/


/* ============================================================================ */
/* ----------------------------- USER DEFINES --------------------------------- */
/* ============================================================================ */


/* ============================================================================ */
/* ----------------------- STRUCTS, ENUMS AND TYPEDEFS ------------------------ */
/* ============================================================================ */
  

/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
  
/* ============================================================================ */
/* ------------------------ MODULE GLOBALE VARIABLES -------------------------- */
/* ============================================================================ */


/* ============================================================================ */
/* -------------------- FORWARD DECLARATIONS / PROTOTYPES --------------------- */
/* ============================================================================ */
  
#ifdef LIN_COOKED_API
void Diag_Handle(void);
#endif // ifdef LIN_COOKED_API

void LIN_Task(void);
l_irqmask l_sys_irq_disable (void);
void l_sys_irq_restore (l_irqmask mask);
void LIN_Sleep_Status(void);

#endif /* LIN_H_ */

