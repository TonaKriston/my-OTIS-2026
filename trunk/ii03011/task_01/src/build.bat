
@echo off
chcp 65001 > nul

cd /d "%~dp0"

echo.
echo Сборка проекта...
cmake -S src -B build

if errorlevel 1 (
    echo Ошибка CMake.
    pause
    exit /b 1
)

cmake --build build --config Debug

if errorlevel 1 (
    echo Ошибка сборки.
    pause
    exit /b 1
)

echo.
echo Запуск программы...
echo.

".\build\Debug\ii03011_task_01.exe"

if errorlevel 1 (
    echo.
    echo Ошибка C++ программы.
    pause
    exit /b 1
)

echo.
echo Запуск визуализации...
echo.

".\.venv\Scripts\python.exe" "src\visual.py"

if errorlevel 1 (
    echo.
    echo Ошибка Python.
    pause
    exit /b 1
)

pause

