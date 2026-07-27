open 1
set_pin 7 0
msleep 200
set_pin 7 1
ident
prog_file E52139B0_ROM_CustomConfig_NoAPPCRCCheck_CRC.hex
erase_main
prog_file E52139B_SampleProject.hex
close
exit
