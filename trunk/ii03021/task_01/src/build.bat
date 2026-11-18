@echo off
setlocal
cd /d "%~dp0"
echo ==========================================
echo Building OTIS Lab 1 - Variant 2 with CMake
echo ==========================================

where cmake >nul 2>nul
if errorlevel 1 (
  echo ERROR: CMake was not found in PATH.
  echo Install CMake and enable "Add CMake to PATH", then retry.
  pause
  exit /b 1
)

rem The bundled GCC installation provides make.exe, rather than mingw32-make.exe.
rem Pick a generator that is actually available on the current machine.
where ninja >nul 2>nul
if not errorlevel 1 (
  set "GENERATOR=Ninja"
) else (
  where mingw32-make >nul 2>nul
  if not errorlevel 1 (
    set "GENERATOR=MinGW Makefiles"
  ) else (
    where make >nul 2>nul
    if not errorlevel 1 (
      set "GENERATOR=Unix Makefiles"
    ) else (
      echo ERROR: No supported build tool was found in PATH.
      echo Install Ninja, MinGW make, or GNU make, then retry.
      goto :failed
    )
  )
)

rem A CMake build directory is tied to both its generator and source path.
rem Discard only generated files when either value belongs to an old build.
set "SOURCE_DIR=%CD:\=/%"
if exist "build\CMakeCache.txt" (
  findstr /b /c:"CMAKE_GENERATOR:INTERNAL=%GENERATOR%" "build\CMakeCache.txt" >nul
  if errorlevel 1 (
    rmdir /s /q "build"
  ) else (
    findstr /b /c:"CMAKE_HOME_DIRECTORY:INTERNAL=%SOURCE_DIR%" "build\CMakeCache.txt" >nul
    if errorlevel 1 rmdir /s /q "build"
  )
)

echo Using CMake generator: %GENERATOR%
cmake -S . -B build -G "%GENERATOR%"

if errorlevel 1 goto :failed
cmake --build build --config Release
if errorlevel 1 goto :failed

echo.
echo Build completed successfully.
echo Run run.bat to start the simulation.
pause
exit /b 0

:failed
echo.
echo Build failed. Check the error messages above.
pause
exit /b 1
