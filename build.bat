@echo off
setlocal
cd /d "%~dp0"
call "%~dp0scripts\env.bat"
if errorlevel 1 exit /b 1

call "%~dp0scripts\build-release.bat"
if errorlevel 1 exit /b 1

echo.
echo Tip: run scripts\deploy.bat to create distributable folder.
pause
endlocal
