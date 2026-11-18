@echo off
setlocal
cd /d "%~dp0"

echo ============================================
echo  Building ii02918 / task_01
echo ============================================
echo.

where cmake >nul 2>nul
if errorlevel 1 (
    echo [ERROR] CMake not found in PATH.
    pause
    exit /b 1
)

if not exist "build" mkdir build
cd build

echo [1/2] Configuring with CMake...
cmake ..\src
if errorlevel 1 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

echo.
echo [2/2] Building...
cmake --build . --config Release
if errorlevel 1 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo.
echo ============================================
echo  Build succeeded!
echo ============================================
pause

endlocal