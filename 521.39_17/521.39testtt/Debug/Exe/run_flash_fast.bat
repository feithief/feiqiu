@echo off
@set JTAG_PORT_ADDR=COM4
@set JTAG_FREQUENCY=125000
@set JTAG_PORT_TYPE=1W_PP
@set JTAG_DEVICE_ID=E52139B

elmos_h430_jtag.exe -f flash_fast.cmd
pause