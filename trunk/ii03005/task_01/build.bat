@echo off

cmake -S src -B build -G "MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if %errorlevel% neq 0 (
    echo CMake configure error!
    pause
    exit /b 1
)

cmake --build build

if %errorlevel% neq 0 (
    echo Build error!
    pause
    exit /b 1
)

echo.
echo Build successful!
echo Starting program...
echo.

build\Task01.exe

pause