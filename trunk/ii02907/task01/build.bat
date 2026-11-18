@echo off
setlocal
cd /d "%~dp0"
echo ===== Configure CMake =====
cmake -S . -B build
if errorlevel 1 goto error
echo ===== Build =====
cmake --build build --config Release
if errorlevel 1 goto error
echo.
echo Build completed. Run build\Release\task_01.exe (or build\task_01.exe).
pause
exit /b 0
:error
echo.
echo Build failed. Read the error messages above.
pause
exit /b 1
