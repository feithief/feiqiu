# @file
# @brief Build Configuration File
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
# @details Build configuration file for the application.
#


#
# Target name
#
TARGET_EXT := AmbientLight6Leds


#
# MCU Type Nr
#
PRODUCT ?= 81116


#
# Chip Revision nr
#
#CHIP_REVISION ?= xAA
CHIP_REVISION ?= xAC


#
# Application CPU clock frequency (in MHz)
#
#FW_FREQ = 12
#FW_FREQ = 14
#FW_FREQ = 16
#FW_FREQ = 24
#FW_FREQ = 28
FW_FREQ = 32


#
# Flash bist page count definition
# @note Number of pages from start of flash to be checked by flash bist (< 0x0100)
#
BIST_PAGE_COUNT = 0x0100


#
# Special application options to be added to CPPFLAGS
#
APP_OPTIONS +=
