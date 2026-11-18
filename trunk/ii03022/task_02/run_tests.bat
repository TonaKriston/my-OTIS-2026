@echo off
title task_02 - build and run unit tests
setlocal

where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo.
    echo  *** CMake not found in PATH! ***
    echo  Install: winget install -e --id Kitware.CMake
    echo.
    pause
    exit /b 1
)

REM --- MinGW берем ТОЛЬКО если есть и g++, и mingw32-make ---
REM (g++ без make = ошибка "CMAKE_MAKE_PROGRAM is not set")
set "GENERATOR="
set "COV_FLAGS="
where g++ >nul 2>nul
if %errorlevel% neq 0 goto use_default_gen
where mingw32-make >nul 2>nul
if %errorlevel% neq 0 goto use_default_gen
set GENERATOR=-G "MinGW Makefiles"
set COV_FLAGS=-DCMAKE_CXX_FLAGS="-g -O0 --coverage"
echo Toolchain: MinGW GCC - tests will be built with gcov coverage flags
goto toolchain_ok
:use_default_gen
echo Toolchain: default generator (e.g. Visual Studio).
echo g++ or mingw32-make not found in PATH - gcov coverage will be SKIPPED.
:toolchain_ok

echo =======================================
echo  [1/3] Configure + build (Google Test)
echo =======================================
cmake -S . -B build %GENERATOR% %COV_FLAGS%
if errorlevel 1 goto error

cmake --build build --config Release
if errorlevel 1 goto error

echo.
echo =======================================
echo  [2/3] Run tests with ctest
echo =======================================
cd build
ctest --output-on-failure > ..\\test_report.txt 2>&1
set RESULT=%errorlevel%
type ..\\test_report.txt
cd ..

echo.
echo =======================================
echo  [3/3] Coverage report (gcovr, optional)
echo =======================================
if not defined COV_FLAGS goto cov_no_gcc
where gcovr >nul 2>nul
if %errorlevel% equ 0 (
    gcovr -r src build --print-summary --exclude ".*main\.cpp"
) else (
    echo  gcovr not found in PATH - coverage skipped.
    echo  Install: pip install gcovr , затем добавь папку Python\Scripts в PATH
)
goto cov_done
:cov_no_gcc
echo  Skipped: сборка не GCC, gcov-файлов нет.
:cov_done

if %RESULT% neq 0 goto error
echo.
echo  All tests passed! Report saved to test_report.txt
pause
exit /b 0

:error
echo.
echo *** FAILED! See output above. ***
pause
exit /b 1
