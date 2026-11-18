@echo off
title task_03 - PID simulation (interactive)
setlocal

set "EXE=build\src\Release\task3_ii03022.exe"
if not exist "%EXE%" set "EXE=build\src\task3_ii03022.exe"

if exist "%EXE%" goto run_program

echo  EXE not found - building the program first...
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo  *** CMake not found in PATH! ***
    echo  Install: winget install -e --id Kitware.CMake
    goto error
)

set "GENERATOR="
where g++ >nul 2>nul
if %errorlevel% neq 0 goto use_default_gen
where mingw32-make >nul 2>nul
if %errorlevel% neq 0 goto use_default_gen
set GENERATOR=-G "MinGW Makefiles"
goto build
:use_default_gen
echo Toolchain: default generator (e.g. Visual Studio).

:build
cmake -S . -B build %GENERATOR%
if errorlevel 1 goto error
cmake --build build --config Release
if errorlevel 1 goto error

set "EXE=build\src\Release\task3_ii03022.exe"
if not exist "%EXE%" set "EXE=build\src\task3_ii03022.exe"
if not exist "%EXE%" (
    echo  *** task3_ii03022.exe not found after build! ***
    goto error
)

:run_program
echo.
echo  Answer the questions: plant, PID, setpoint w, mode A/B.
echo.
"%EXE%"
if errorlevel 1 goto end

if exist pid_result.csv (
    where python >nul 2>nul
    if %errorlevel% equ 0 python plot.py
)

:end
echo.
pause
exit /b 0

:error
echo.
echo *** FAILED! See output above. ***
pause
exit /b 1
