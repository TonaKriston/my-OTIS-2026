@echo off
setlocal

echo ============================================================
echo   RUNNING TESTS
echo ============================================================
echo.

set "ROOT=%~dp0"
set "EXE=%ROOT%build\Debug\tests.exe"

if not exist "%EXE%" (
    echo [ERROR] Test executable not found:
    echo         %EXE%
    echo Please run build_tests.bat first.
    echo.
    pause
    exit /b 1
)

echo [INFO] Executable found: %EXE%
echo [INFO] Working directory: %ROOT%src
echo.

pushd "%ROOT%src"
"%EXE%" --gtest_color=yes
set "RC=%ERRORLEVEL%"
popd

echo.
if "%RC%"=="0" (
    echo [OK] All tests PASSED.
) else (
    echo [ERROR] Some tests FAILED ^(exit code %RC%^).
)

echo.
pause
endlocal
exit /b %RC%