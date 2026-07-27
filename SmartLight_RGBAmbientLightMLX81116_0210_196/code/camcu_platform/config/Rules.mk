# @file
# @brief Global rules for the build process
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
# @details Global rules for the build process on the platform level.
#

# default rules
$(OBJDIR)/%.o:%.c | $(OBJDIR)
	@$(ECHO) --- Processing: $<
	$(HIDE_CMD)$(CC) -c -MMD $(INSTSET) $(CFLAGS) $(CPPFLAGS) $< -o $@

$(OBJDIR)/%.o:%.S | $(OBJDIR)
	@$(ECHO) --- Processing: $<
	$(HIDE_CMD)$(CC) -c -MMD $(INSTSET) $(ASFLAGS) $(CPPFLAGS) $< -o $@

$(OBJDIR):
	-$(HIDE_CMD)$(MKDIR) $(OBJDIR)

# flash bist crc calculation
flash_crc = $(shell $(MLX_CRC) $(TARGET)_nocrc.hex --rom_bist --start=$(FLASH_CS_START) --end=$(FLASH_CS_END))

# create hex file to be able to calculate all interrupt vector crc's
$(TARGET)_nocrc.elf: $(OBJS)
	@$(ECHO) "Link application with dummy CRC"
	$(HIDE_CMD)$(CC) $(OBJS) -o $@ $(INSTSET) $(LDFLAGS) -Wl,--defsym,FL_SIGN=0x0000

# create final hex file to program to the chip
$(TARGET).elf: $(TARGET)_nocrc.hex $(OBJS)
	@$(ECHO) "Linking ->" $@
	$(HIDE_CMD)$(CC) $(OBJS) -o $@ $(INSTSET) $(LDFLAGS) -Wl,--defsym,FL_SIGN=$(flash_crc)
	$(HIDE_CMD)$(OCP) $@ $@ --gap-fill 0xFF $(NO_OUTPUT)
	@$(ECHO) "Cleaning temp files."
	-$(HIDE_CMD)$(RM) $(TARGET)_nocrc.hex
	-$(HIDE_CMD)$(RM) $(TARGET)_nocrc.elf

# create hex / bin files from ELF output file
%.hex: %.elf
	@$(ECHO) --- Generating: $@
	$(HIDE_CMD)$(OCP) -O ihex $< $@

%.bin: %.elf
	@$(ECHO) --- Generating: $@
	$(HIDE_CMD)$(OCP) -O binary $< $@

# create extended listing file from ELF output file
%.lss: %.elf
	@$(ECHO) --- Generating: $@
	$(HIDE_CMD)$(CC) --version > $@
	$(HIDE_CMD)$(ODP) -h -S -z $< >> $@

# create a symbol table from ELF output file
%.sym: %.elf
	@$(ECHO) --- Generating: $@
	$(HIDE_CMD)$(NM) -n $< > $@

# create assembler files from C source files.
%.s: %.c
	@$(ECHO) --- Generating: $@
	$(HIDE_CMD)$(CC) -S $(INSTSET) $(CFLAGS) $(CPPFLAGS) $< -o $@

# create preprocessed source
%.i: %.c
	@$(ECHO) --- Generating: $@
	$(HIDE_CMD)$(CC) -E $(INSTSET) $(CFLAGS) $(CPPFLAGS) $< -o $@
