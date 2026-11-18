@echo off
title task_01 build
chcp 65001 >nul


echo ============================================
echo  [1/4] Prepare build (cmake configure)
echo ============================================
cmake -S src -B build -G "MinGW Makefiles"
if %errorlevel% neq 0 goto error

echo.
echo ============================================
echo  [2/4] Compile the program (cmake build)
echo ============================================
cmake --build build --config Release
if %errorlevel% neq 0 goto error

echo.
echo ============================================
echo  [3/4] Launch task1_ii03007.exe
echo ============================================
if exist build\Release\task1_ii03007.exe (
    build\Release\task1_ii03007.exe
) else (
    build\task1_ii03007.exe
)

echo.
echo ============================================
echo  [4/4] Draw graph (python plot.py)
echo ============================================
if exist result.csv (
    python plot.py
    if %errorlevel% neq 0 (
        echo  Python is not on PATH - no graph. Open result.csv in Excel.
    )
) else (
    echo  result.csv was not found - no graph.
)

echo.
pause
exit /b 0

:error
echo.
echo *** SOMETHING WENT WRONG. Read the message above. ***
pause
exit /b 1