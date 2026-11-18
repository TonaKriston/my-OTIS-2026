@echo off
setlocal
cd /d "%~dp0"

echo ============================================
echo Building ii02911 / task_01 / variant 25
echo ============================================

where cmake >nul 2>nul
if errorlevel 1 (
    echo [ERROR] CMake not found in PATH.
    pause
    exit /b 1
)

echo [1/2] Configuring with CMake...
cmake -S . -B build
if errorlevel 1 goto error

echo [2/2] Compiling...
cmake --build build --config Release
if errorlevel 1 goto error

echo [OK] Build succeeded.
echo Run build\src\ii02911_task_01.exe for MinGW,
echo or build\src\Release\ii02911_task_01.exe for Visual Studio.
pause
exit /b 0

:error
echo [ERROR] Build failed. Check CMake and your C++ compiler.
pause
exit /b 1
