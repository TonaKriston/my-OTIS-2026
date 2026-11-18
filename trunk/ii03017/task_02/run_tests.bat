@echo off
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0"

echo =========================================
echo  OTIS Task 02 - GCC Build, Tests, Coverage
echo =========================================
echo.

where cmake >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake was not found in PATH.
    goto error
)

REM Prefer the GCC installation used in this project, if it exists.
if exist "C:\Users\user\gcc\bin\g++.exe" (
    set "GCC_BIN=C:\Users\user\gcc\bin"
) else (
    where g++ >nul 2>&1
    if errorlevel 1 (
        echo ERROR: GCC compiler g++.exe was not found.
        echo Add the MinGW GCC bin directory to PATH or update GCC_BIN in this file.
        goto error
    )
    for %%I in (g++.exe) do set "GXX_PATH=%%~$PATH:I"
    for %%I in ("!GXX_PATH!") do set "GCC_BIN=%%~dpI"
)

REM Clean trailing slash from GCC_BIN if present
if "%GCC_BIN:~-1%"=="\" set "GCC_BIN=%GCC_BIN:~0,-1%"

set "PATH=%GCC_BIN%;%PATH%"
set "GXX=%GCC_BIN%\g++.exe"
set "GCOV=%GCC_BIN%\gcov.exe"

if not exist "%GXX%" (
    echo ERROR: GCC compiler not found: "%GXX%"
    goto error
)
if not exist "%GCOV%" (
    echo ERROR: gcov.exe not found beside g++.exe: "%GCOV%"
    echo The gcov version must match the selected GCC installation.
    goto error
)

python -m gcovr --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: gcovr is not installed.
    echo Install it with: python -m pip install gcovr
    goto error
)

echo Using GCC compiler:
echo   "%GXX%"
"%GXX%" --version | findstr /B /C:"g++"
echo.
echo Using coverage tool:
"%GCOV%" --version | findstr /B /C:"gcov"
echo.

REM Find a build tool compatible with GCC (mingw32-make, ninja, or make).
set "CMAKE_GENERATOR="
set "MAKE_PROGRAM="

if exist "%GCC_BIN%\mingw32-make.exe" (
    set "CMAKE_GENERATOR=MinGW Makefiles"
    set "MAKE_PROGRAM=%GCC_BIN%\mingw32-make.exe"
) else if exist "%GCC_BIN%\ninja.exe" (
    set "CMAKE_GENERATOR=Ninja"
    set "MAKE_PROGRAM=%GCC_BIN%\ninja.exe"
) else if exist "%GCC_BIN%\make.exe" (
    set "CMAKE_GENERATOR=MinGW Makefiles"
    set "MAKE_PROGRAM=%GCC_BIN%\make.exe"
) else (
    where mingw32-make >nul 2>&1
    if not errorlevel 1 (
        for %%I in (mingw32-make.exe) do set "MAKE_PROGRAM=%%~$PATH:I"
        set "CMAKE_GENERATOR=MinGW Makefiles"
    ) else (
        where ninja >nul 2>&1
        if not errorlevel 1 (
            for %%I in (ninja.exe) do set "MAKE_PROGRAM=%%~$PATH:I"
            set "CMAKE_GENERATOR=Ninja"
        ) else (
            where make >nul 2>&1
            if not errorlevel 1 (
                for %%I in (make.exe) do set "MAKE_PROGRAM=%%~$PATH:I"
                set "CMAKE_GENERATOR=MinGW Makefiles"
            )
        )
    )
)

if not defined CMAKE_GENERATOR (
    echo ERROR: No compatible build tool was found.
    echo GCC and gcov are installed in "%GCC_BIN%", but CMake also needs a build tool.
    echo.
    echo To fix this, do ONE of the following:
    echo   1. Скачайте ninja.exe и положите его в папку: "%GCC_BIN%"
    echo   2. Скачайте/найдите mingw32-make.exe и положите его в папку: "%GCC_BIN%"
    echo   3. Установите Ninja/Make и добавьте путь к ним в системную переменную PATH.
    goto error
)

echo Build generator: %CMAKE_GENERATOR%
echo Build tool: "%MAKE_PROGRAM%"
echo.

if exist build rmdir /s /q build
if not exist build mkdir build
if not exist build\coverage mkdir build\coverage

echo [1/4] Configuring CMake with the selected GCC compiler...
cmake -S . -B build -G "%CMAKE_GENERATOR%" -DCMAKE_MAKE_PROGRAM="%MAKE_PROGRAM%" -DCMAKE_CXX_COMPILER="%GXX%" -DOTIS_BUILD_TESTS=ON -DOTIS_ENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 goto error

echo.
echo [2/4] Building application and tests...
cmake --build build
if errorlevel 1 goto error

echo.
echo [3/4] Running Google Test...
ctest --test-dir build --output-on-failure --output-junit test-results.xml
set "TEST_RESULT=%ERRORLEVEL%"

echo.
echo [4/4] Generating coverage reports...
REM root = trunk\ii03017, so the coverage report includes task_01\src (models, signals) too.
REM Use plain "gcov" (found via PATH): gcovr breaks a full Windows path with backslashes.
python -m gcovr -r .. --gcov-executable gcov --filter ".*/task_0[12]/src/" --html-details --exclude-branches-by-pattern ".*" -o build\coverage\coverage.html build
if errorlevel 1 goto coverage_error
python -m gcovr -r .. --gcov-executable gcov --filter ".*/task_0[12]/src/" --xml-pretty --exclude-branches-by-pattern ".*" -o build\coverage\coverage.xml build
if errorlevel 1 goto coverage_error

echo.
echo =========================================
echo Reports:
echo   Test results:  build\test-results.xml
echo   HTML coverage: build\coverage\coverage.html
echo   XML coverage:  build\coverage\coverage.xml
echo =========================================
if exist "build\coverage\coverage.html" start "" "build\coverage\coverage.html"
if not "%TEST_RESULT%"=="0" goto test_error

echo.
echo SUCCESS: Tests passed and coverage reports were generated.
goto finish

:test_error
echo.
echo WARNING: One or more tests failed. Review the test output above.
goto error

:coverage_error
echo.
echo ERROR: Coverage report generation failed.
echo Check that tests generated .gcda files and that gcovr is compatible with GCC.
goto error

:error
echo.
echo ERROR: The process did not complete successfully. Review the messages above.

:finish
echo.
echo Press any key to close this window.
pause >nul
endlocal
