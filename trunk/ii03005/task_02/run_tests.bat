@echo off
setlocal
cd /d "%~dp0"
echo Configuring and building Task 02...
cmake -S src -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
if errorlevel 1 exit /b 1
cmake --build build
if errorlevel 1 exit /b 1
del /s /q build\CMakeFiles\*.gcda >nul 2>nul
echo Running Google Test...
ctest --test-dir build --output-on-failure
if errorlevel 1 exit /b 1
where gcov >nul 2>nul
if errorlevel 1 (
    echo ERROR: gcov is not installed or not available in PATH.
    exit /b 1
)
py -m gcovr --version >nul 2>nul
if errorlevel 1 (
    echo ERROR: gcovr is not installed.
    exit /b 1
)
echo Generating coverage reports...
py -m gcovr --root . --object-directory build --gcov-executable gcov --filter "src" --exclude "src/.*\.h$" --exclude "build/.*" --exclude-branches-by-pattern ".*" --gcov-ignore-errors=no_working_dir_found --markdown build\coverage.md
if errorlevel 1 exit /b 1
echo Tests completed successfully.
endlocal
