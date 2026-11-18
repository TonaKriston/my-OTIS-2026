@echo off
setlocal

if not exist "%~dp0CMakeLists.txt" (
    echo ERROR CMakeLists not found in this folder
    pause
    exit /b 1
)

cd /d "%~dp0"

rem Use the GCC/MinGW toolchain installed on this computer.
set "OTIS_GCC_BIN=C:\Users\user\gcc\bin"
set "PATH=%OTIS_GCC_BIN%;%PATH%"

echo STARTING BUILD PROCESS

echo STAGE 1 Configuration
rem Remove the old NMake cache before configuring with GNU Make.
if exist "build\CMakeCache.txt" rmdir /s /q "build"
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_CXX_COMPILER="%OTIS_GCC_BIN%\g++.exe" -DCMAKE_MAKE_PROGRAM="%OTIS_GCC_BIN%\make.exe"
if errorlevel 1 goto error

echo STAGE 2 Compilation
cmake --build build --config Release
if errorlevel 1 goto error

echo SUCCESS Build completed successfully
echo Executable file: build\src\otis_task_01.exe
pause
goto end

:error
echo ERROR Build failed
pause
exit /b 1

:end
endlocal
