@echo off
REM ============================================================
REM  Автоматическая сборка проекта ii02909 / task_01
REM  Использование: build.bat
REM  Результат: собранный exe внутри build/
REM ============================================================

setlocal

REM --- Переход в директорию, где лежит этот .bat файл ---
cd /d "%~dp0"

echo ============================================
echo  Building ii02909 / task_01
echo ============================================
echo.

REM --- Проверка наличия CMake ---
where cmake >nul 2>nul
if errorlevel 1 (
    echo [ERROR] CMake not found in PATH. Please install CMake first.
    pause
    exit /b 1
)

REM --- Создание папки сборки ---
if not exist "build" mkdir build
cd build

REM --- Конфигурация: используем наш собственный CMakeLists.txt в task_01 ---
echo [1/2] Configuring with CMake...
cmake .. 
if errorlevel 1 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

REM --- Сборка ---
echo.
echo [2/2] Building...
cmake --build . --config Release
if errorlevel 1 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo.
echo ============================================
echo  Build succeeded!
echo ============================================
pause

endlocal