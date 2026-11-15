@echo off
REM ============================================================
REM  Build and run tests
REM ============================================================
setlocal EnableDelayedExpansion

set BUILD_DIR=build
set GTEST_DIR=C:\libs\googletest-master\googletest-main

echo ============================================================
echo  START: %DATE% %TIME%
echo  Directory: %CD%
echo ============================================================
echo.
pause

REM ------------------------------------------------------------
REM 0. Environment checks
REM ------------------------------------------------------------
echo [0/4] Checking environment...
echo   - Looking for CMakeLists.txt...
if not exist CMakeLists.txt (
    echo.
    echo [ERROR] CMakeLists.txt not found in %CD%
    echo         Run this script from the project root.
    pause
    exit /b 1
)
echo     OK

echo   - Looking for src\models.cpp...
if not exist src\models.cpp (
    echo.
    echo [ERROR] src\models.cpp not found.
    pause
    exit /b 1
)
echo     OK

echo   - Looking for test\ directory...
if not exist test (
    echo.
    echo [ERROR] test\ directory not found.
    pause
    exit /b 1
)
echo     OK

echo   - Checking cmake is in PATH...
where cmake >nul 2>nul
if errorlevel 1 (
    echo.
    echo [ERROR] cmake not found in PATH.
    echo         Add C:\...\CMake\bin to PATH.
    pause
    exit /b 1
)
echo     OK

echo   - Checking %GTEST_DIR%\CMakeLists.txt exists...
if not exist "%GTEST_DIR%\CMakeLists.txt" (
    echo.
    echo [ERROR] GoogleTest not found: %GTEST_DIR%\CMakeLists.txt
    echo         Fix GTEST_DIR at the top of this script.
    pause
    exit /b 1
)
echo     OK

echo.
echo [0/4] Environment OK.
echo.
pause

REM ------------------------------------------------------------
REM 1. CMake configure
REM ------------------------------------------------------------
echo ============================================================
echo [1/4] CMake configure...
echo ============================================================
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

cmake -S . -B %BUILD_DIR% ^
      -G "Visual Studio 17 2022" -A x64 ^
      -DGTEST_DIR=%GTEST_DIR%

if errorlevel 1 (
    echo.
    echo ============================================================
    echo [ERROR] CMake configure failed.
    echo         See CMake messages above.
    echo ============================================================
    pause
    exit /b 1
)

echo.
echo [1/4] Configure OK.
echo.
pause

REM ------------------------------------------------------------
REM 2. Build
REM ------------------------------------------------------------
echo ============================================================
echo [2/4] Build...
echo ============================================================
cmake --build %BUILD_DIR% --config Debug --parallel

if errorlevel 1 (
    echo.
    echo ============================================================
    echo [ERROR] Build failed.
    echo         See compiler messages above.
    echo ============================================================
    pause
    exit /b 1
)

echo.
echo [2/4] Build OK.
echo.
pause

REM ------------------------------------------------------------
REM 3. Run tests
REM ------------------------------------------------------------
echo ============================================================
echo [3/4] Run tests...
echo ============================================================

REM Important: For Visual Studio, explicitly set config "Debug"
REM and use --output-on-failure to see Google Test output.
ctest --test-dir %BUILD_DIR% -C Debug --output-on-failure

if errorlevel 1 (
    echo.
    echo ============================================================
    echo [WARN] Some tests failed (see output above).
    echo        This is a test result, not a build error.
    echo ============================================================
    pause
    exit /b 1
)

echo.
echo ============================================================
echo  DONE: all tests passed.
echo ============================================================
pause
endlocal
exit /b 0