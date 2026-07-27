@echo off

LIN-LDFMaster.exe \\.\COM6 E52139B0_SampleProject.ldf EXCHANGE_RED
LIN-BMMFlash.exe info_prog --start-page 0x80 --page-count 0x40 E52139B0_ROM_CustomConfig_NoAPPCRCCheck_CRC.hex

pause