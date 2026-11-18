@echo off
setlocal
cd /d "%~dp0"
if not exist "build\Release\otis_variant2.exe" if not exist "build\otis_variant2.exe" (
  echo Executable not found. Run build.bat first.
  pause
  exit /b 1
)
if exist "build\Release\otis_variant2.exe" (
  "build\Release\otis_variant2.exe" %*
) else (
  "build\otis_variant2.exe" %*
)
echo.
echo Results and CSV files are saved under the results folder.
pause
