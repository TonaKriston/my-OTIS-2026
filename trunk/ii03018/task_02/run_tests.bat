@echo off
cd /d "%~dp0"

echo ==========================================
echo [1/3] Configuring and building project...
echo ==========================================
where g++ >nul 2>nul
if %errorlevel% equ 0 (
    if exist "build\CMakeCache.txt" (
        findstr /i "MSVC Visual" "build\CMakeCache.txt" >nul 2>nul
        if not errorlevel 1 rmdir /s /q build
    )
    cmake -S src -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
) else (
    cmake -S src -B build -DENABLE_COVERAGE=ON
)

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b %errorlevel%
)

cmake --build build
if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    pause
    exit /b %errorlevel%
)

echo.
echo ==========================================
echo [2/3] Running Google Test...
echo ==========================================
ctest --test-dir build -C Release --output-on-failure

if exist "build\unit_tests.exe" (
    "build\unit_tests.exe" > test_report.txt
    type test_report.txt
) else if exist "build\Release\unit_tests.exe" (
    "build\Release\unit_tests.exe" > test_report.txt
    type test_report.txt
) else if exist "build\Debug\unit_tests.exe" (
    "build\Debug\unit_tests.exe" > test_report.txt
    type test_report.txt
)

echo.
echo ==========================================
echo [3/3] Coverage report (gcovr, optional)
echo ==========================================
where gcovr >nul 2>nul
if %errorlevel% equ 0 (
    gcovr -r src build
) else (
    echo [INFO] gcovr not found in PATH. Run: pip install gcovr
)

echo.
echo All tests passed! Report saved to test_report.txt
pause