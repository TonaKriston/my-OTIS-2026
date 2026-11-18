@echo off
title task_03 - build, tests, doxygen docs
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

set "GENERATOR="
where g++ >nul 2>nul
if %errorlevel% neq 0 goto use_default_gen
where mingw32-make >nul 2>nul
if %errorlevel% neq 0 goto use_default_gen
set GENERATOR=-G "MinGW Makefiles"
goto toolchain_ok
:use_default_gen
echo Toolchain: default generator (e.g. Visual Studio).
:toolchain_ok

echo =======================================
echo  [STEP 1/3] Compiling and Testing...
echo =======================================
cmake -S . -B build %GENERATOR%
if errorlevel 1 goto error

cmake --build build --config Release
if errorlevel 1 goto error

cd build
ctest --output-on-failure > ..\\test_report.txt 2>&1
set RESULT=%errorlevel%
type ..\\test_report.txt
cd ..
if %RESULT% neq 0 goto error

echo.
echo =======================================
echo  [STEP 2/3] Generating Doxygen Docs...
echo =======================================
where doxygen >nul 2>nul
if %errorlevel% neq 0 (
    echo  doxygen not found in PATH - docs skipped.
    echo  Install: winget install -e --id DimitriVanHeesch.Doxygen
    goto docs_done
)
pushd ..
doxygen task_03\Doxyfile
set DOXL=%errorlevel%
popd
if %DOXL% neq 0 goto error
echo  Docs ready: doc\html\index.html
:docs_done

echo.
echo =======================================
echo  [STEP 3/3] Execution Finished Successfully!
echo =======================================
if %RESULT% neq 0 goto error
echo.
echo  Tests: report in test_report.txt
echo  Docs:  doc\html\index.html
pause
exit /b 0

:error
echo.
echo *** FAILED! See output above. ***
pause
exit /b 1
