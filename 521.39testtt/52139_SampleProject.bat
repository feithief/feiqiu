@echo off
@set JTAG_PORT_ADDR=COM4
@set JTAG_PORT_TYPE=1W_PP
@set JTAG_FREQUENCY=1000000
@set JTAG_DEVICE_ID=E52139B

start /d "C:\Program Files (x86)\IAR Systems\Embedded Workbench 7.2\common\bin" /b IarIdePm.exe "%cd%\52139_SampleProject.eww"
