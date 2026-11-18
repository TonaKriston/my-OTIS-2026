@echo off
chcp 65001 >nul
setlocal

echo ============================================
echo   Сборка проекта OTIS Lab 1 (Variant 14)
echo ============================================
echo.

:: Проверка наличия CMakeLists.txt
if not exist "%~dp0CMakeLists.txt" (
    echo [ERROR] Файл CMakeLists.txt не найден в папке: %~dp0
    pause
    exit /b 1
)

:: Переход в папку со скриптом
cd /d "%~dp0"

:: Удаление старой папки build (опционально, для чистой сборки)
if exist "build" (
    echo [INFO] Удаление старой папки build...
    rmdir /s /q build
)

:: Шаг 1: Конфигурация CMake
echo [1/3] Конфигурация проекта...
cmake -S . -B build -G "MinGW Makefiles"
if errorlevel 1 (
    echo [ERROR] Ошибка конфигурации CMake!
    pause
    exit /b 1
)

:: Шаг 2: Сборка проекта
echo [2/3] Компиляция проекта...
cmake --build build --config Release
if errorlevel 1 (
    echo [ERROR] Ошибка компиляции!
    pause
    exit /b 1
)

:: Шаг 3: Копирование exe в папку src (для удобства)
echo [3/3] Копирование исполняемого файла...
if exist "build\lab01.exe" (
    copy /Y "build\lab01.exe" ".\lab01.exe" >nul
    echo [OK] Исполняемый файл скопирован в папку src
) else if exist "build\Release\lab01.exe" (
    copy /Y "build\Release\lab01.exe" ".\lab01.exe" >nul
    echo [OK] Исполняемый файл скопирован в папку src
) else (
    echo [WARNING] Исполняемый файл не найден в папке build
)

echo.
echo ============================================
echo   Сборка завершена успешно!
echo ============================================
echo.
echo Для запуска программы введите:
echo   lab01.exe
echo.
echo Для построения графиков введите:
echo   python plot.py
echo.

pause
endlocal