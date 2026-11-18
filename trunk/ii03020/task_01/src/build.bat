@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion
title OTIS Lab #1 - Build & Run

REM ============================================================
REM  Путь к папке, где лежит сам .bat (работает из любой точки)
REM ============================================================
set "ROOT=%~dp0"
cd /d "%ROOT%"

echo ============================================================
echo   ЛАБОРАТОРНАЯ РАБОТА №1 «Моделирование температуры объекта» 
echo   СБОРКА И ЗАПУСК
echo ============================================================
echo   Каталог проекта: %ROOT%
echo.

REM ============================================================
REM  ШАГ 1. Проверка CMake
REM ============================================================
echo [1/5] Проверка CMake...
where cmake >nul 2>&1
if errorlevel 1 (
    echo.
    echo   ОШИБКА: CMake не найден в PATH.
    echo   Установите CMake с https://cmake.org/download/
    echo   или выполните: winget install Kitware.CMake
    echo   После установки перезапустите это окно.
    echo.
    pause
    exit /b 1
)
for /f "delims=" %%v in ('cmake --version ^| findstr /r "^cmake version"') do set "CMAKE_VER=%%v"
echo   OK: !CMAKE_VER!

REM ============================================================
REM  ШАГ 2. Проверка компилятора (g++ или cl)
REM ============================================================
echo.
echo [2/5] Проверка компилятора...
where g++ >nul 2>&1
if not errorlevel 1 (
    for /f "tokens=1-3" %%a in ('g++ --version') do (
        set "GXX_VER=%%a %%b %%c"
        goto :gxx_found
    )
    :gxx_found
    echo   OK: !GXX_VER!
    set GENERATOR=-G "MinGW Makefiles"
    goto :compiler_done
)

where cl >nul 2>&1
if not errorlevel 1 (
    echo   OK: MSVC (cl.exe найден^)
    set "GENERATOR="
    goto :compiler_done
)

echo.
echo   ОШИБКА: не найден ни g++ (MinGW), ни cl.exe (MSVC^).
echo.
echo   Вариант A - установить MinGW-w64:
echo     1. Скачать с https://winlibs.com/
echo     2. Распаковать, например, в C:\mingw64
echo     3. Добавить C:\mingw64\bin в PATH
echo.
echo   Вариант B - открыть этот .bat из
echo     "x64 Native Tools Command Prompt for VS"
echo     (тогда cl.exe уже будет в PATH^)
echo.
pause
exit /b 1

:compiler_done

REM ============================================================
REM  ШАГ 3. Проверка CMakeLists.txt
REM ============================================================
echo.
echo [3/5] Проверка структуры проекта...
if not exist "CMakeLists.txt" (
    echo   ОШИБКА: CMakeLists.txt не найден в "%ROOT%"
    echo   Положите build.bat рядом с CMakeLists.txt.
    pause
    exit /b 1
)

set "SRC_COUNT=0"
for %%f in (main.cpp InputGenerator.cpp Linear_Model.cpp ^
            Non_Linear_Model.cpp Simple_Differential_Equation.cpp ^
            TableOutput.cpp Plotter.cpp) do (
    if exist "%%f" (
        set /a SRC_COUNT+=1
    ) else (
        echo   ОШИБКА: отсутствует файл %%f
        pause
        exit /b 1
    )
)
echo   OK: CMakeLists.txt найден, все !SRC_COUNT! .cpp-файлов на месте.

REM ============================================================
REM  ШАГ 4. Конфигурация и сборка
REM ============================================================
echo.
echo [4/5] Конфигурация CMake...
if exist "build\CMakeCache.txt" (
    echo   Кэш предыдущей сборки найден - используются старые настройки.
    echo   Для полной пересборки удалите папку build.
)

cmake -S . -B build %GENERATOR%
if errorlevel 1 (
    echo.
    echo   ОШИБКА на этапе конфигурации. Смотрите сообщения CMake выше.
    pause
    exit /b 1
)
echo   OK: конфигурация завершена.

echo.
echo [5/5] Компиляция...
cmake --build build --config Release
if errorlevel 1 (
    echo.
    echo   ОШИБКА на этапе компиляции. Смотрите сообщения выше.
    pause
    exit /b 1
)
echo   OK: сборка завершена.

REM Найти собранный .exe (MSVC кладёт в build\Release, MinGW - в build\)
set "EXE="
if exist "build\sim.exe"         set "EXE=build\sim.exe"
if exist "build\Release\sim.exe" set "EXE=build\Release\sim.exe"

if "!EXE!"=="" (
    echo.
    echo   ОШИБКА: собранный sim.exe не найден ни в build\, ни в build\Release\.
    pause
    exit /b 1
)

echo.
echo ============================================================
echo   СБОРКА УСПЕШНА
echo   Исполняемый файл: %ROOT%!EXE!
echo ============================================================
echo.

REM ============================================================
REM  ЗАПУСК ПРОГРАММЫ
REM ============================================================
echo Запуск программы...
echo.
"!EXE!"

if errorlevel 1 (
    echo.
    echo   Программа завершилась с кодом ошибки %errorlevel%.
    pause
    exit /b %errorlevel%
)

REM ============================================================
REM  ИТОГОВЫЙ ОТЧЁТ
REM ============================================================
echo.
echo ============================================================
echo   ГОТОВО. Созданные файлы в "%ROOT%":
echo ============================================================
for %%f in (linear_model.csv nonlinear_model.csv sde_model.csv ^
            linear_model.png nonlinear_model.png sde_model.png ^
            comparison.png) do (
    if exist "%%f" (
        echo   [+] %%f
    ) else (
        echo   [-] %%f  ^(не создан^)
    )
)
for %%f in (linear_model.plt nonlinear_model.plt sde_model.plt comparison.plt) do (
    if exist "%%f" echo   [+] %%f  ^(скрипт gnuplot^)
)

echo.
echo Открыть папку с результатами? ^(Y - да, N - нет^)
set /p "OPEN_DIR=Выбор: "
if "!OPEN_DIR!"=="Y" (
    start "" explorer "%ROOT%"
) else if "!OPEN_DIR!"=="y" (start "" explorer "%ROOT%")

echo.
pause
endlocal