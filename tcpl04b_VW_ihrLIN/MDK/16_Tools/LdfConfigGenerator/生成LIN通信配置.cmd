@echo off
setlocal
chcp 65001 >nul
set "TOOL_DIR=%~dp0"

if "%~1"=="" (
    set /p "LDF_FILE=请输入LDF完整路径: "
) else (
    set "LDF_FILE=%~1"
)

if "%~2"=="" (
    call "%TOOL_DIR%运行生成器.cmd" "%LDF_FILE%"
) else (
    call "%TOOL_DIR%运行生成器.cmd" "%LDF_FILE%" --node "%~2"
)

if errorlevel 1 (
    echo.
    echo 生成失败，请查看上面的错误。
) else (
    echo.
    echo 生成结果位于: %TOOL_DIR%Generated
)
pause
