@echo off
setlocal
cd /d "%~dp0"
echo Configuring and building Task 02...
cmake -S src -B build -G "MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
if errorlevel 1 exit /b 1
cmake --build build --target Task02
if errorlevel 1 exit /b 1
echo Starting the application...
build\Task02.exe
endlocal
