# @file
# @brief Application modules definitions
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
# @details Extension Makefile to the application Makefile.
#          It contains list of modules available for the application.
#


#
# Supported application modules
#
USER_STARTUP = user_startup
MLS_API = mls_api
MLS_DEVICE_ID = mls_device_id
STD_LIN_API = std_lin_api
LIN_AUTO_ADDRESSING = lin_auto_addressing
FW_PLTF_VERSION = fw_pltf_version


#
# Getting definitions from each application module
#
LIBS_DIRS = $(patsubst %,$(PLTF_DIR)/src/%,$(APP_MODULES))
include $(patsubst %,%/srclist.mk,$(LIBS_DIRS))
