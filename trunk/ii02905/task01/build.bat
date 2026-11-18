@echo off
setlocal

echo ===== CMake configure =====
if not exist build mkdir build

cmake -S . -B build
if errorlevel 1 (
    echo CMake configure FAILED
    pause
    exit /b 1
)

echo ===== CMake build =====
cmake --build build --config Release
if errorlevel 1 (
    echo Build FAILED
    pause
    exit /b 1
)

echo ===== Build completed successfully =====
pause
