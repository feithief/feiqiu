@echo off
setlocal
set "LDF=LDF\EEA2.0_LIN_Matrix_V2.0.1_20230320_LIN10.ldf"
set "BASE=profiles\_lin10_generation_base.profile.json"
set "PRESETS=profiles\lin10_xy_256.presets.json"
set "GENOUT=%TEMP%\LIN10_ColorPalette_%RANDOM%%RANDOM%"

python tools\validate_signal_presets.py.txt "%PRESETS%"
if errorlevel 1 exit /b %errorlevel%

mkdir "%GENOUT%"
python tools\ldf_profile_gen.py.txt generate --ldf "%LDF%" --overlay "%BASE%" --overlay "%PRESETS%" --output "%GENOUT%"
if errorlevel 1 exit /b %errorlevel%

python tools\ldf_profile_gen.py.txt check --ldf "%LDF%" --overlay "%BASE%" --overlay "%PRESETS%" --output "%GENOUT%"
if errorlevel 1 exit /b %errorlevel%

copy /Y "%GENOUT%\linprofile_generated.cpp" "generated\linprofile_generated.cc" >nul
copy /Y "%GENOUT%\linprofile_generated.h" "generated\linprofile_generated.h" >nul
copy /Y "%GENOUT%\linprofile_report.json" "generated\linprofile_report.json" >nul
rmdir /S /Q "%GENOUT%"
exit /b %errorlevel%
