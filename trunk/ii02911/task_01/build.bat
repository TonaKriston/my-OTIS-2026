@echo off
setlocal

set "MINGW=C:\msys64\ucrt64\bin"
set "PATH=%MINGW%;%PATH%"

echo ==========================================
echo        OTIS-2026 - BUILD
echo ==========================================
echo.

echo Checking C++ compiler...
"%MINGW%\g++.exe" --version

if errorlevel 1 (
    echo.
    echo ERROR: g++ cannot be started.
    pause
    exit /b 1
)

echo.
echo Checking Make...
"%MINGW%\mingw32-make.exe" --version

if errorlevel 1 (
    echo.
    echo ERROR: mingw32-make cannot be started.
    pause
    exit /b 1
)

echo.
echo Removing old CMake cache...

if exist build (
    rmdir /s /q build
)

echo.
echo Configuring CMake...

cmake -S . -B build -G "MinGW Makefiles" ^
 -DCMAKE_CXX_COMPILER="%MINGW%\g++.exe" ^
 -DCMAKE_MAKE_PROGRAM="%MINGW%\mingw32-make.exe"

if errorlevel 1 (
    echo.
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Building...

cmake --build build

if errorlevel 1 (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo ==========================================
echo     BUILD COMPLETED SUCCESSFULLY
echo ==========================================
echo.

pause