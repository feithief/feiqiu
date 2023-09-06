 /* ------------------------------------------------------------------- 
  * @file  main.c
  * @brief main funtion (code)
  *
  * Purpose:
  *    
  *
  * @section history_sec Revision History
  * $Id: main.c, v 1.0 2018/09/13 $
  *
  * @verbatim
  * ------------------------------------------------------------------- 
  * Date     B/F Who Comment
  * ------------------------------------------------------------------- 
  * 2018/09/13 (F) JEFFERY.WU initial version
  * ------------------------------------------------------------------- 
  * 
  *
  * @creator      JEFFERY.WU
  * @created      2018/09/13
  * 
  * @project      521.36
  * @requested    -
  * @inspector    - 
  * @verification -
  * @version      $Revision: V1.0 $
  * 
  * ------------------------------------------------------------------- 
  */

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "main.h"
#include "SystemMain.h"
#include "SystemType.h"

 
void main(void)
{
  dint();
  systemInit();
  eint();

  while(1)
  {
    systemMainLoop();
  }
}
