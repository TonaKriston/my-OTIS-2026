@echo off
REM ============================================================
REM  Automated build script for ii02910 / task_01
REM  Usage: build.bat
REM  Output: ii02910_task_01.exe inside build/
REM ============================================================

setlocal

cd /d "%~dp0"

echo ============================================
echo  Building ii02910 / task_01
echo ============================================
echo.

REM --- Check CMake is available ---
set CMAKE_BIN=C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe

if not exist "%CMAKE_BIN%" (
    echo [ERROR] CMake not found at:
    echo   %CMAKE_BIN%
    echo Please install CMake or update the path in this script.
    pause
    exit /b 1
)

REM --- Prepare build directory ---
if not exist "build" mkdir build
cd build

REM --- Configure via local CMakeLists.txt in task_01 ---
echo [1/2] Configuring with CMake...
"%CMAKE_BIN%" .. -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

REM --- Build ---
echo.
echo [2/2] Building...
"%CMAKE_BIN%" --build . --config Release
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