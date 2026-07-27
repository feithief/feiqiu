# @file
# @brief Translate REVISION to PROJECT_ID
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
# @ingroup config
#
# @details This file assigns the dependence between chip's revision and the project id's
#

ifeq ($(CHIP_REVISION), xAA)
  PROJECT_ID = 0x16AA
  ROM_BASED_LOADER = Melibu
else
ifeq ($(CHIP_REVISION), xAC)
  PROJECT_ID = 0x1002
  ROM_BASED_LOADER = Melibu
else
  $(error 'Error: Wrong chip revision selected ($(CHIP_REVISION)).')
endif
endif

PROJECT_PATH_EXT = $(CHIP_REVISION)

#
#--- Flash BIST configuration ---
#

# @note Can be overwritten by application Makefile
FLASH_CS_START ?= 0x5800

# Flash bist page count definition
# @note Number of pages from start of flash to be checked by flash bist (< 0x0100)
#       Can be overwritten by application Makefile
BIST_PAGE_COUNT ?= 0x0100
