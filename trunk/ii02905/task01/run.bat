@echo off
call build.bat
if errorlevel 1 exit /b 1

echo.
echo ===== Running program =====
build\Release\task_01.exe
pause
