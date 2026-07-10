@echo off
setlocal
cd /d "%~dp0"
call "%~dp0scripts\env.bat"
if errorlevel 1 exit /b 1

if not exist "%BUILD_EXE%" (
    echo Please run build.bat or scripts\build-release.bat first.
    pause
    exit /b 1
)

start "" "%BUILD_EXE%"
endlocal
