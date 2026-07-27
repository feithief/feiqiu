# @file
# @brief Application Makefile
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
# @details Makefile to build the application
#

# 
# target dependent configurations
#
include Makefile.configure.mk

ifndef PRODUCT
$(error 'Error: Variable PRODUCT is not defined')
endif

include Makefile.srcs.mk

TARGET  = $(CHIP_FULL_NAME)_$(TARGET_EXT)

PROJ_DIR = ../..
CODE_DIR = $(PROJ_DIR)/code
PLTF_DIR = $(CODE_DIR)/camcu_platform
HIL_DIR  = $(PROJ_DIR)/verification/hil_tests
LINT_DIR = $(PROJ_DIR)/verification/lint
UNCR_DIR = $(PROJ_DIR)/verification/uncrustify

# do not use any variable in the following define!!!!
PLTF_DEFS_FILE = ../../verification/eclipse/pltf_defs.h
PLTF_DEFS_DIR = $(dir $(PLTF_DEFS_FILE))


# platform common configurations
include $(PLTF_DIR)/config/Config.mk


#
# HANDLE EXTRA SOURCES DIRECTORIES
#
VPATH += $(SRCS_APP_DIRS)
INC_DIRS += $(SRCS_APP_DIRS)

#
# CREATE TOTAL SOURCE/OBJ FILES LISTS
#
SRCS  = $(SRCS_APP)
SRCS += $(APP_MODULES_SRCS)

OBJS  = $(patsubst %.S, $(OBJDIR)/%.o, $(filter %.S, $(SRCS)))
OBJS += $(patsubst %.c, $(OBJDIR)/%.o, $(filter %.c, $(SRCS)))

# Include automatic dependencies if exist
OBJDEPS = $(OBJS:%.o=%.d)
-include $(OBJDEPS)

#
# RULES
#

# main rules
.PHONY: all
all: app
	$(HIDE_CMD)$(SIZE) -B $(TARGET).elf
	@$(ECHO) --- Project : $(TARGET) successfully created

.PHONY: app
app: $(TARGET).elf $(TARGET).hex $(TARGET).lss $(PLTF_DEFS_FILE) $(LDF_GEN_SRCS)

.PHONY: release
release: all
	$(HIDE_CMD)$(CP) $(TARGET).elf $(TARGET).hex $(TARGET).lss $(PROJ_DIR)/release

.PHONY: clean
clean:
	-$(HIDE_CMD)$(RM) $(OBJDIR)
	-$(HIDE_CMD)$(RM) $(TARGET).cod $(TARGET).elf.MDS
	-$(HIDE_CMD)$(RM) $(TARGET).map $(TARGET).elf
	-$(HIDE_CMD)$(RM) $(TARGET).hex $(TARGET).lss
	-$(HIDE_CMD)$(RM) $(TARGET).sym
	-$(HIDE_CMD)$(RM) $(TARGET)*_lint.txt
	@$(ECHO) --- Cleaning project $(TARGET) has been finished

# static code analysis
.PHONY: lint
lint:
# get correct gcc include directories
	@$(ECHO) $(subst /cygdrive/c,C:,$(foreach v,$(shell echo | mlx16-cpp -Wp,-v 2>&1),$(if $(findstring "/,$(v)), ,$(if $(findstring /,$(v)),-i\"$(v)\",)))) > $(LINT_DIR)/gcc-include-path.lnt
# get all include directories
	@$(ECHO) $(subst /cygdrive/c,C:,$(subst -I,,$(foreach v,$(CPPFLAGS),$(if $(findstring /,$(v):0:1),-i\"$(v)\",)))) > $(LINT_DIR)/project_include_path.lnt
	@$(ECHO) | $(CC) -E -dM $(INSTSET) $(CFLAGS) $(CPPFLAGS) - > $(LINT_DIR)/lint_cmac.h
	$(HIDE_CMD)$(LINT) -i$(LINT_DIR) common-options.lnt co-gcc.lnt project-options.lnt temp.lnt $(filter %.c, $(SRCS)) > $(TARGET)_lint.txt
	$(HIDE_CMD)$(LINT) -i$(LINT_DIR) common-options.lnt co-gcc.lnt au-misra2.lnt project-options.lnt temp.lnt $(filter %.c, $(SRCS)) > $(TARGET)_misra_lint.txt

# get all application c and h files to be uncrustified
SRCS_UNCRUSTIFY := $(filter-out $(LDF_GEN_SRCS), $(wildcard *.c) $(wildcard *.h))


# code style formatting
.PHONY: uncrustify
uncrustify:
	$(HIDE_CMD)$(UNCRUSTIFY) -c $(UNCR_DIR)/uncrustify.cfg --replace --no-backup -l C $(SRCS_UNCRUSTIFY)

.PHONY: uncrustify-check
uncrustify-check:
	$(HIDE_CMD)$(UNCRUSTIFY) -c $(UNCR_DIR)/uncrustify.cfg --check -l C $(SRCS_UNCRUSTIFY)


# defines file for eclipse
.PHONY: $(PLTF_DEFS_FILE)
$(PLTF_DEFS_FILE): clean_pltf_defs | $(PLTF_DEFS_DIR)
	$(foreach item,$(filter -D%,$(CPPFLAGS)),$(shell $(ECHO) $(subst -D, ,$(subst =, ,$(patsubst %,\#define %,$(item)))) >> $@))

.PHONY: clean_pltf_defs
clean_pltf_defs: | $(PLTF_DEFS_DIR)
	@$(ECHO) > $(PLTF_DEFS_FILE)

$(PLTF_DEFS_DIR):
	$(MKDIR) $(PLTF_DEFS_DIR)

# Platform common rules
include $(PLTF_DIR)/config/Rules.mk
