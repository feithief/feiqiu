@echo off

LIN-LDFMaster.exe \\.\COM6 E52139B0_SampleProject.ldf EXCHANGE_RED
LIN-BMMFlash.exe flash_prog E52139B_SampleProject.hex

pause