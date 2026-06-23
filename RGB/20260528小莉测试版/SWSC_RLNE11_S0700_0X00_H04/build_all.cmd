@echo off
setlocal

set "UV4=C:\Keil_v5\UV4\UV4.exe"
if not exist "%UV4%" (
  echo Keil UV4 was not found at %UV4%
  exit /b 1
)

pushd "%~dp0"

echo Rebuilding expanded source application with external AA configuration...
start "" /wait "%UV4%" -r SWSC_Application.uvprojx -t SWSC_Application -j0 -o build_application.log
if not exist "Objects\SWSC_Application.hex" (
  echo Application build failed. See build_application.log.
  popd
  exit /b 1
)

echo Build completed successfully.
popd
exit /b 0
