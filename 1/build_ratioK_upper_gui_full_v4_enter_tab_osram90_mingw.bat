@echo off
setlocal

gcc ratioK_upper_gui_full_v4_enter_tab_osram90.c -o ratioK_upper_gui.exe -mwindows

if errorlevel 1 (
    echo Build failed.
    pause
    exit /b 1
)

echo Build success: ratioK_upper_gui.exe
pause
