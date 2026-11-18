@echo off
setlocal
cd /d "%~dp0"

if "%~1"=="" (set STEPS=40) else (set STEPS=%~1)
if "%~2"=="" (set DT=0.1) else (set DT=%~2)

echo ===============================================
echo Lab 1, variant 12 - CMake configure
echo ===============================================
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 goto failed

echo.
echo ===============================================
echo Build C++ sources
echo ===============================================
cmake --build build
if errorlevel 1 goto failed

echo.
echo ===============================================
echo Run automated model tests
echo ===============================================
ctest --test-dir build --output-on-failure
if errorlevel 1 goto failed

echo.
echo ===============================================
echo Select a model in the console, N=%STEPS%, dt=%DT%
echo ===============================================
"build\lab1.exe" --steps %STEPS% --dt %DT% --out results
if errorlevel 1 goto failed

echo.
echo Completed. Only selected models produced outputs in src\results\.
pause
exit /b 0

:failed
echo.
echo ERROR: Check the message above. Make sure CMake, Ninja and G++ are in PATH.
pause
exit /b 1
