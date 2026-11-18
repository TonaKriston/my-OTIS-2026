@echo off
chcp 65001 >nul
title Лаба 2 - тесты
setlocal

REM ----------------------------------------------------------
REM  Шаг 1: проверяем, что установлен CMake
REM  Шаг 2: если есть MinGW (g++ + mingw32-make), собираем
REM         тесты с метками gcov (опция TEST_COVERAGE)
REM  Шаг 3: собираем, прогоняем ctest, печатаем сводку gcovr
REM ----------------------------------------------------------

where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo.
    echo  [ОШИБКА] CMake не найден в PATH.
    echo  Установите:  winget install -e --id Kitware.CMake
    pause
    exit /b 1
)

set USE_MINGW=0
where g++ >nul 2>nul && where mingw32-make >nul 2>nul && set USE_MINGW=1

echo.
echo ============== [1 из 3] Сборка тестов (CMake + Google Test) ==============
if "%USE_MINGW%"=="1" (
    echo  Инструменты: MinGW GCC, тесты будут собраны с метками gcov.
    cmake -S . -B build -G "MinGW Makefiles" -DTEST_COVERAGE=ON
) else (
    echo  Инструменты: генератор по умолчанию, покрытие gcov пропустим.
    cmake -S . -B build
)
if errorlevel 1 goto fail
cmake --build build --config Release
if errorlevel 1 goto fail

echo.
echo ============== [2 из 3] Прогон всех тестов (ctest) ==============
cd build
ctest --output-on-failure > ..\test_report.txt 2>&1
set RC=%errorlevel%
type ..\test_report.txt
cd ..

echo.
echo ============== [3 из 3] Покрытие кода (gcovr) ==============
if "%USE_MINGW%" neq "1" goto coverage_done
where gcovr >nul 2>nul
if %errorlevel% equ 0 (
    gcovr -r src build --print-summary --exclude ".*main\.cpp"
) else (
    echo  gcovr не найден в PATH - пропуск.
    echo  Установите:  pip install gcovr
)
:coverage_done

if %RC% neq 0 goto fail
echo.
echo  ВСЕ ТЕСТЫ ПРОШЛИ. Отчёт сохранён: test_report.txt
echo.
pause
exit /b 0

:fail
echo.
echo  [ОШИБКА] сборка или тесты упали, смотрите вывод выше.
pause
exit /b 1