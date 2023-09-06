/* ***************************************************************************
 *  $Date: 2017-02-10 16:44:44 +0100 (Fr, 10 Feb 2017) $    
 *  $Author: poe $
 *  $Revision: 1929 $
 * ****************************************************************************/
#ifndef _VERSION_H_
#define _VERSION_H_

// Numbering suggestion:
// 
// ++Major: Redesign / Major changes
// ++Minor: Backward incompatible API changes
// ++Build:	Any change, small/internal bugfixes etc.

#define MAIN_VERSION_MAJOR		1
#define MAIN_VERSION_MAJOR_STR	        "1" 
 
#define MAIN_VERSION_MINOR		0
#define MAIN_VERSION_MINOR_STR	        "0"

#define MAIN_VERSION_BUILD		27
#define MAIN_VERSION_BUILD_STR	        "27"

#define MAIN_VERSION_PROJECT            "E52136A_SampleProject"
#define MAIN_VERSION_AUTHOR             "ELMOS Semiconductor AG"

#define MAIN_VERSION_YEAR               "2016"

#define MAIN_VERSION_NUMBER		((MAIN_VERSION_MAJOR<<24)|(MAIN_VERSION_MINOR<<16)|MAIN_VERSION_BUILD) // Bit: 31-24 Major, 23-16 Minor, 15-0 Build
#define MAIN_VERSION_STRING		"v"  MAIN_VERSION_MAJOR_STR "." MAIN_VERSION_MINOR_STR "." MAIN_VERSION_BUILD_STR
#define MAIN_VERSION_STRING_LONG	MAIN_VERSION_PROJECT " " MAIN_VERSION_STRING " (c)" MAIN_VERSION_YEAR " " MAIN_VERSION_AUTHOR

/* ****************************************************************************/
/*                                 USER DEFINES                               */
/* ****************************************************************************/

#define APP_REVISION		    0x6525 /**< Major version define. */
#define APP_VERSION_MAJOR_STR	    "2565" /**< Major version string. */
 

#define APP_VERSION_PROJECT         "E52136A_SampleProject"  /**< Project name string. */
#define APP_VERSION_AUTHOR          "ELMOS Semiconductor AG" /**< Company name string. */

#define APP_VERSION_YEAR            "2016" /**< Version year string. */

#define APP_VERSION_NUMBER	    (APP_REVISION) /**< Macro to build version number. */
#define APP_VERSION_STRING	    "r"  APP_REVISION /**< Complete version string. */
#define APP_VERSION_STRING_LONG	    APP_VERSION_PROJECT " " APP_VERSION_STRING " (c)" APP_VERSION_YEAR " " APP_VERSION_AUTHOR /**< Long version string. */

/* ****************************************************************************/
/*                              MODULE VARIABLES                              */
/* ****************************************************************************/

struct main_sAppInfo
{
    uint16_t                   ApplicationVersion;
    uint8_t                    ApplicationName[32];
};                                    
typedef struct main_sAppInfo     main_sAppInfo_t;

#pragma location="APP_VERSION"
__root const main_sAppInfo_t main_AppInfo =
{
  .ApplicationVersion = APP_REVISION,  
  .ApplicationName= APP_VERSION_PROJECT

};


/* --------------------------------------------------------------------------------------------------------------- *
 * Date      Version    Who   Comment
 * YY/MM/DD  Ma.Mi.Bd
 * --------------------------------------------------------------------------------------------------------------- *
 * 17/02/10  1.0.28     POE   added BusLayer rx_buffer CRC error fix
 * 17/02/01  1.0.27     POE   added TransportLayer ResponseFrame processing interrupted fix
 * 16/11/16  1.0.26     POE   addded n_Cr timeout workaround.
 * 16/07/20  1.0.25     POE   added toggable WAIT_GOTOSLEEP state for slave node sending the WakeupCluster call/added Project string/PWM on/off LIN Sleep
 * 16/07/14  1.0.24     POE   Extended support to CT 8:NCNM: Sleep/Wakeup Tests
 * 16/07/08  1.0.23     POE   Optimizations of LINDrvImp_Init()
 * 16/06/29  1.0.22     POE   Reduced stack size buildup in the LINDrvImp_Init() by partitioning the layer inits to separate function calls
 * 16/06/19  1.0.21     POE   Added support for alternative LookUpLayer implementation
 * 16/05/30  1.0.20     POE   Adjusted mods for updated irq handling
 * 16/05/12  1.0.19     POE   Readded and adjusted LIN conformance test source files
 * 16/04/26  1.0.18     POE   Added SignalList based Frame decoding for pwm control example
 * 16/03/13  1.0.17     POE   Ported Project to 521.36B Controller
 * 15/10/23  1.0.16     SBAI  Added function to set Init-Vals for LEDC module. Increased step-resolution in PI-Proto.
 * 15/09/14  1.0.15     SBAI  Fixed a inter message delay bug. Fading stops if a ne command is send now.
 * 15/09/01  1.0.14     RPY   Removed power fail maonitoring.
 * 15/08/27  1.0.13     RPY   Added alternative PID mapping for ELDP protocol
 * 15/07/28  1.0.12     SBAI  Implemented ONSEMI intensity table created by RFIO
 * 15/06/11  1.0.11     RPY   Added group addressing for PI protocol
 * 15/06/08  1.0.10     RPY   Added NMECH module type
 * 15/05/12  1.0.9      RPY   Fixed intensity calculation in PI protocol. 
 * 25/03/27  1.0.8      RPY   Added power fail monitoring. 
 * 15/02/27  1.0.7      RPY   Cleanup and merge of ELDP, BMW and PI protocol, added PWM config of v1.0.5a
 * 15/02/27  1.0.6      RPY   Prerelease of ELDP Protocol version  
 * 15/03/18  1.0.5a     RPY   Added defines for PWM polarity and output selection
 * 08/12/04  1.0.5      SBAI  Changed MUX order/pin assignment for new ELNA board layout
 * 10/11/04  1.0.4      SBAI  Fixed another bug in refresh rate calculation
 * 07/11/04  1.0.3      SBAI  Added support for Pacific Insight protocol
 *                            Added led fading
 *                            Fixed bug in refresh rate calculation
 *                            Improved refresh rate calculation (Max rate is 5000Hz)            
 * 04/11/04  1.0.2      SBAI  Fixed wrong mux gpio table in ELNA module
 * 28/10/04  1.0.1      SBAI  Fixed problems with interrupting SCI interrupts in PWM module
 * 24/10/04  1.0.0      SBAI  Initial project release
 * --------------------------------------------------------------------------------------------------------------- */

#endif
