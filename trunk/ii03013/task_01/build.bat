@echo off
setlocal

cd /d "%~dp0"

echo Configuring project...
cmake -S . -B build
if errorlevel 1 goto error

echo.
echo Building project...
cmake --build build --config Release
if errorlevel 1 goto error

echo.
echo Build completed successfully!
echo.
echo Starting program...
echo.

build\Release\task_01.exe

if not exist "results.csv" (
    echo.
    echo Error: results.csv was not found.
    pause
    exit /b 1
)

echo.
echo Building graph from results.csv...
echo.

python src\plot.py "results.csv"

if errorlevel 1 (
    echo.
    echo Error: Python graph script failed.
    pause
    exit /b 1
)

echo.
echo Graph created successfully!
echo.

if not exist "doc\graph.png" (
    echo.
    echo Error: graph.png was not created.
    pause
    exit /b 1
)

echo Opening graph...
start "" "%~dp0doc\graph.png"

echo.
echo Program and graph finished.
pause
exit /b 0

:error
echo.
echo Build failed. Check the error messages above.
pause
exit /b 1