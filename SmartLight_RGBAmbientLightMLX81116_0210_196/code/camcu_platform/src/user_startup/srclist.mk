# @file srclist.mk
# @brief Application module sources list
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
# @details List of application sources to be build with the firmware application.
#
# @note The following defined variables are standard and shall not be changed:
#       - VPATH
#       - APP_MODULES_SRCS
#       - INC_DIRS


#
# SEARCH PATHES
#
VPATH += $(APP_MOD_DIR)/user_startup/$(PRODUCT)


#
# SOURCE FILES LIST
#
APP_MODULES_SRCS += fw_vectors.S
APP_MODULES_SRCS += fw_premain.c
APP_MODULES_SRCS += fw_startup.c
APP_MODULES_SRCS += fw_ram_section_init.c
APP_MODULES_SRCS += fw_ints.c


#
# HEADER SEARCH PATHES
#
INC_DIRS += $(APP_MOD_DIR)/user_startup/$(PRODUCT)


#
# MODULE SPECIFIC DEFS
#
HAS_MLS_API ?= 0 # by default is disabled
HAS_MLS_DEVICE_ID ?= 0 # by default is disabled
APP_OPTIONS += -DHAS_MLS_API=$(HAS_MLS_API)
APP_OPTIONS += -DHAS_MLS_DEVICE_ID=$(HAS_MLS_DEVICE_ID)
