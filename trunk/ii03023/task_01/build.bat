@echo off
chcp 65001 > nul
echo [INFO] Создание папки сборки в task_01...
if not exist build mkdir build
cd build

echo [INFO] Запуск CMake...
cmake -G "Visual Studio 17 2022" -A x64 ../src
if %errorlevel% neq 0 (
    echo [ERROR] Ошибка конфигурации CMake!
    pause
    exit /b %errorlevel%
)

echo [INFO] Сборка проекта (Release)...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo [ERROR] Ошибка компиляции!
    pause
    exit /b %errorlevel%
)

echo [INFO] Сборка успешно завершена!
echo [INFO] Запуск C++ программы для расчета и выгрузки .csv...
cd Release
if exist task_01_ii03021.exe (
    task_01_ii03021.exe
) else (
    ..\task_01_ii03021.exe
)

:: Возвращаемся в корень task_01
cd ..\..

echo.
echo [INFO] Запуск визуализации графиков matplotlib...
if exist src\plot_results.py (
    python src\plot_results.py
) else (
    echo [ERROR] Файл src\plot_results.py не найден!
)

echo.
echo [INFO] Работа батника полностью завершена!
pause