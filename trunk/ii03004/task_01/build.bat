@echo off
chcp 65001 > nul
setlocal

cd /d "%~dp0"

if not exist "CMakeLists.txt" (
    echo [ERROR] CMakeLists.txt not found in %CD%
    pause
    exit /b 1
)

echo =======================================
echo  OTIS-2026  Task 01  Variant 4
echo  Build process started
echo =======================================

where cmake > nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake not found. Install CMake and add it to PATH.
    pause
    exit /b 1
)

rem MinGW if g++ is available, otherwise the default generator (Visual Studio)
set "GENERATOR="
where g++ > nul 2>&1
if not errorlevel 1 set GENERATOR=-G "MinGW Makefiles"

echo.
echo [1/3] Configuration (cmake)
echo ---------------------------------------
cmake -S . -B build %GENERATOR%
if errorlevel 1 goto error

echo.
echo [2/3] Compilation
echo ---------------------------------------
cmake --build build --config Release
if errorlevel 1 goto error

echo.
echo [3/3] Running the program
echo ---------------------------------------
echo SUCCESS: build completed, executable is in build\bin
echo.
cd build\bin
ii03004_task_01.exe
cd /d "%~dp0"

echo.
echo =======================================
echo  Done
echo =======================================
pause
goto end

:error
echo.
echo [ERROR] Build failed
pause
exit /b 1

:end
endlocal
