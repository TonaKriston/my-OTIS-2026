@echo off
setlocal

echo ============================================================
echo   BUILDING TESTS (Visual Studio 2022 / MSVC)
echo ============================================================
echo.

set "ROOT=%~dp0"
cd /d "%ROOT%"

echo [1/2] Configuring CMake (Debug)...
cmake -S "%ROOT%." -B "%ROOT%build" -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 (
    echo.
    echo [ERROR] CMake configuration FAILED.
    pause
    exit /b 1
)
echo [OK] CMake configuration finished successfully.
echo.

echo [2/2] Building target 'tests' (Release)...
cmake --build "%ROOT%build" --config Debug --target tests
if errorlevel 1 (
    echo.
    echo [ERROR] Build FAILED.
    pause
    exit /b 1
)
echo.
echo [OK] Build finished successfully.
echo Executable: %ROOT%build\Debug\tests.exe
echo.

pause
endlocal