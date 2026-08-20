@echo off
setlocal
chcp 65001 >nul
set "TOOL_DIR=%~dp0"
set "PROJECT_CFG=%TOOL_DIR%..\..\04_BSW_COM"

if "%~1"=="" (
    set /p "LDF_FILE=请输入LDF完整路径: "
) else (
    set "LDF_FILE=%~1"
)

if "%~2"=="" (
    call "%TOOL_DIR%运行生成器.cmd" "%LDF_FILE%" --output "%PROJECT_CFG%" --backup
) else (
    call "%TOOL_DIR%运行生成器.cmd" "%LDF_FILE%" --node "%~2" --output "%PROJECT_CFG%" --backup
)

if errorlevel 1 (
    echo.
    echo 应用失败，工程配置没有更新。
) else (
    echo.
    echo 已更新工程04_BSW_COM中的Com_Cfg.c/.h，请重新编译。
)
pause
