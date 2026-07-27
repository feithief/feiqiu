# @file
# @brief Application sources list
# @internal
#
# @copyright (C) 2018 Melexis N.V.
#
# Melexis N.V. is supplying this code for use with Melexis N.V. processor based microcontrollers only.
#
# THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
# INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.  MELEXIS N.V. SHALL NOT IN ANY CIRCUMSTANCES,
# BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
#
# @endinternal
#
# @ingroup application
#
# @details List of application sources to be build into the firmware application.
#

#
# EXTRA SOURCES DIRS
#
SRCS_APP_DIRS  += Algorithm
SRCS_APP_DIRS  += Modules
SRCS_APP_DIRS  += system
#SRCS_APP_DIRS += another_subfolder


#
# SOURCE FILES LIST
#
SRCS_APP += main.c
SRCS_APP += Algorithm.c
SRCS_APP += FloatLib.c
SRCS_APP += ModuleAdc.c
SRCS_APP += ModuleComm.c
SRCS_APP += ModuleFlash.c
SRCS_APP += ModulePwm.c
SRCS_APP += ModulePower.c
SRCS_APP += ModuleWatchdog.c
SRCS_APP += ModuleTemprature.c
SRCS_APP += ModuleBattery.c
SRCS_APP += SystemMain.c
SRCS_APP += SystemStatus.c
SRCS_APP += SystemControl.c

#
# EXTRA PLATFORM MODULES TO COMPILE IN
#
APP_MODULES = $(USER_STARTUP)


#
# PLATFORM LIBRARIES USED BY THIS APPLICATION
#
PLTF_LIBS := $(PRODUCT)
PLTF_LIBS += amalthea_$(PRODUCT)
PLTF_LIBS += crc_$(PRODUCT)
#PLTF_LIBS += dsp_$(PRODUCT)
#PLTF_LIBS += filter_$(PRODUCT)
PLTF_LIBS += math_$(PRODUCT)
#PLTF_LIBS += colorlib
