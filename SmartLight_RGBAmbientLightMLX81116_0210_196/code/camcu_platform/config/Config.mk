# @file
# @brief Global configurations for the build process
# @internal
#
# @copyright (C) 2017-2018 Melexis N.V.
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
# @ingroup platform
#
# @details Global configurations for the build process on the platform level.
#

ifndef PRODUCT
$(error 'Error: Variable PRODUCT is not defined')
endif

# Identify the root directory of the SW Platform (escape spaces in the path)
empty :=
space := $(empty) $(empty)
ROOTDIR  := $(subst $(space),\$(space),$(dir $(filter %/Config.mk, $(MAKEFILE_LIST)))..)

#--- Include product specific variables ---------------------------------------
include $(ROOTDIR)/config/$(PRODUCT).mk

#--- Assemble the full chip name ----------------------------------------------
CHIP_FULL_NAME = $(PRODUCT)$(CHIP_REVISION)
CHIP_PATH_NAME = $(PRODUCT)$(PROJECT_PATH_EXT)

#--- Common directories -------------------------------------------------------
LIBDIR   := $(ROOTDIR)/lib
LDDIR    := $(LIBDIR)/ldscripts
OBJDIR   := obj
APP_MOD_DIR := $(ROOTDIR)/src

# --- Set default debug info output level ---
ifndef DEBUG
DEBUG = 2
endif

ifeq ($(DEBUG), 3)
HIDE_CMD =
NO_OUTPUT =
else
HIDE_CMD = @
NO_OUTPUT = > /dev/null
endif

#--- Common compilation flags -------------------------------------------------
CPPFLAGS  = -I .

OPTIMIZATION = -Os

CFLAGS  = $(OPTIMIZATION)
CFLAGS += -g
CFLAGS += -std=gnu99

# Tweaks
CFLAGS += -mram-align-word
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
# Use -mdpage-explicit to have global variables in nodp by default
CFLAGS += -mdpage-explicit

# Warnings
CFLAGS += -Wcast-align
CFLAGS += -Wall # all
CFLAGS += -Wextra # extra basic warnings!
#CFLAGS += -Werror # promote warnings to errors
CFLAGS += -Wstrict-prototypes # warn if function argument types are not defined
CFLAGS += -Wundef # warn of use of undefined macros
# CFLAGS += -Winline # warn when functions requested as inline are not inlined
CFLAGS += -Wno-packed-bitfield-compat # false warning on bitfields

ASFLAGS += $(OPTIMIZATION) -gdwarf-2

# Workaround for ITC bug (see MULAN-5 and MMC16-26 on Jira), aready fixed in HW
#CFLAGS  += -mwith-itc-bug-workaround
#ASFLAGS += -mwith-itc-bug-workaround

#--- Common linker flags ------------------------------------------------------
LDFLAGS := -N
LDFLAGS += -L $(LIBDIR)/$(CHIP_PATH_NAME)
LDFLAGS += -L $(LDDIR)
LDFLAGS += $(foreach library,$(PLTF_LIBS),-l$(library))
LDFLAGS += -T $(CHIP_PATH_NAME).ld -T $(CHIP_PATH_NAME)-rom-defs.ld
LDFLAGS += -Wl,-Map,$(TARGET).map
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--warn-common
LDFLAGS += -Wl,--discard-none

INSTSET := -mlx16-fx

#--- Product specific configuration -------------------------------------------
PLTF_DEFS += USE_ZERO_IT_SUM_CHECK=1

# Flash BIST configurations
FLASH_PAGE_SIZE = 0x0080
FLASH_CS_END = $(shell echo $$(( $(FLASH_CS_START) + ($(BIST_PAGE_COUNT) * $(FLASH_PAGE_SIZE)) )))

PLTF_DEFS += FLASH_CS_START=$(FLASH_CS_START)
PLTF_DEFS += FLASH_CS_END=$(FLASH_CS_END)
PLTF_DEFS += BIST_PAGE_COUNT=$(BIST_PAGE_COUNT)

# FPLL is visible in all source files (CPU clock in kHz)
FPLL := $(FW_FREQ)000

# Extra defines from profile
PLTF_DEFS += PROJECT_ID=$(PROJECT_ID)

PLTF_DEFS += __MLX$(PRODUCT)__
PLTF_DEFS += PRODUCT_STR=\"$(CHIP_FULL_NAME)\"
PLTF_DEFS += __MLX$(CHIP_FULL_NAME)__

CPPFLAGS += $(foreach folder,$(INC_DIRS),-I$(folder))
CPPFLAGS += $(foreach define,$(PLTF_DEFS),-D$(define))

#--- Product ROM specific configuration ---------------------------------------
# this will set all relevant variables to default if not defined before so this
# is included last. 
include $(ROOTDIR)/config/$(CHIP_PATH_NAME)-cpp-flags.mk
CPPFLAGS += $(PLTF_CPPFLAGS)

#
# Placeholder for command-line defines:
#   gmake APP_OPTIONS="-Dxxx"
#
CPPFLAGS += $(APP_OPTIONS)

#--- Toolchain and utilities --------------------------------------------------

# GCC programs
AS   = mlx16-as
AR   = mlx16-ar
CC   = mlx16-gcc
ODP  = mlx16-objdump
OCP  = mlx16-objcopy
NM   = mlx16-nm
SIZE = mlx16-size

# Standard utilities (MUST be in the environment PATH)
ECHO  = echo
RM    = rm -fr
CP    = cp -f
MKDIR = mkdir -p
DIFF  = diff -Nq
UNCRUSTIFY = uncrustify

# Additional tools
ifeq ($(OS),Windows_NT)
	LDF_NODEGEN ?= $(ROOTDIR)/bin/ldf_nodegen.exe
	MLX_CRC  ?= $(ROOTDIR)/bin/pymlxhex.exe
else
	LDF_NODEGEN ?= ldf_nodegen
	PYTHON3 ?= python3
	MLX_CRC ?= pymlxhex
endif

#--- Application modules ------------------------------------------------------
include $(APP_MOD_DIR)/AppModules.mk

#--- Default search paths are included in the last term ---------------------- 
INC_DIRS += $(ROOTDIR)/include
INC_DIRS += $(ROOTDIR)/include/$(CHIP_PATH_NAME)