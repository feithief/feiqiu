@echo off
setlocal

cd /d "%~dp0"

powershell -ExecutionPolicy Bypass -File ".\sync_flat_source.ps1"
if errorlevel 1 (
    echo 同步失败！
    exit /b 1
)

cd /d "%~dp0flat_source"
cmd /c "gcc Algorithm.c FloatLib.c AlgoRatioK.c HostRatioKDebug.c host_saved_config.c ratio_k_test_main.c -o ratio_k_test.exe"
if errorlevel 1 (
    echo 编译失败！
    exit /b 1
)

echo 同步+编译成功！
endlocal