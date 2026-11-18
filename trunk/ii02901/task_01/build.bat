@echo off
chcp 65001 >nul
echo ==========================================
echo   Автоматическая сборка и запуск проекта
echo ==========================================

if not exist build mkdir build
cd build

echo.
echo Конфигурация CMake...
echo ------------------------------------------
cmake ../src
if %errorlevel% neq 0 (
    echo Ошибка конфигурации CMake!
    pause
    exit /b 1
)

echo.
echo Компиляция проекта ...
echo ------------------------------------------
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Ошибка компиляции!
    pause
    exit /b 1
)

echo.
echo Запуск симуляции...
echo ------------------------------------------

if exist Release\ii02901_task01.exe (
    Release\ii02901_task01.exe
) else if exist Debug\ii02901_task01.exe (
    Debug\ii02901_task01.exe
) else if exist ii02901_task01.exe (
    ii02901_task01.exe
) else (
    echo Исполняемый файл не найден!
)

echo.
echo ==========================================
echo   Построение графиков ...
echo ==========================================
py ../src/plot.py
if %errorlevel% neq 0 (
    echo [!] Ошибка! Проверьте установленны ли python, matplotlib, pandas.
)

echo   Процесс успешно завершен!
pause