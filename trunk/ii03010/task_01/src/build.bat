@echo off
setlocal

set "SRC_DIR=%~dp0."
set "BUILD_DIR=%~dp0..\..\..\..\..\build-ii03010-task01"

cmake -S "%SRC_DIR%" -B "%BUILD_DIR%"
if errorlevel 1 exit /b 1

cmake --build "%BUILD_DIR%" --config Debug
if errorlevel 1 exit /b 1

echo Build completed successfully.
endlocal
