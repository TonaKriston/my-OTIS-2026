@echo off
title Build task_1

echo Configure project (1/4)
cmake -S src -B build
if %errorlevel% neq 0 goto error

echo.
echo Building project (2/4)
cmake --build build --config Release
if %errorlevel% neq 0 goto error

echo.
echo Running simulation (3/4)
if exist build\Release\ii03018_task.1.exe (
build\Release\ii03018_task.1.exe
) else (
build\ii03018_task.1.exe
)

echo.
echo Graph
python plot.py
if %errorlevel% neq 0 (
    echo [!] Python error. Check if python and matplotlib are installed.
)

echo.
pause
exit /b 0

:error
echo.
echo BUILD ERROR
pause
exit /b 1