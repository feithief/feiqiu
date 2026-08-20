@echo off
setlocal
set "TOOL_DIR=%~dp0"
set "PYTHON3="

if exist "%USERPROFILE%\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe" (
    set "PYTHON3=%USERPROFILE%\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe"
)

if not defined PYTHON3 (
    for /f "delims=" %%P in ('where python3.exe 2^>nul') do if not defined PYTHON3 set "PYTHON3=%%P"
)

if not defined PYTHON3 (
    for /f "delims=" %%P in ('where python.exe 2^>nul') do (
        for /f "delims=" %%V in ('"%%P" -c "import sys; print(sys.version_info[0])" 2^>nul') do (
            if "%%V"=="3" if not defined PYTHON3 set "PYTHON3=%%P"
        )
    )
)

if not defined PYTHON3 (
    echo 找不到Python 3。请安装Python 3，或从Codex中运行本工具。
    exit /b 3
)

"%PYTHON3%" "%TOOL_DIR%ldf_to_autosar.py" %*
exit /b %errorlevel%
