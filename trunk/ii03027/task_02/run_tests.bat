@echo off
chcp 65001 >nul
echo ===================================================
echo Compiling project and running tests...
echo ===================================================

if not exist build mkdir build
cd build

cmake .. -G "MinGW Makefiles"
cmake --build . 

echo ===================================================
echo Running Google Test...
echo ===================================================
ctest --output-on-failure

echo ===================================================
echo Generating Code Coverage Report...
echo ===================================================
python -m gcovr -r .. --filter ".*src/.*" --exclude ".*test/.*" --exclude ".*_deps/.*" --exclude ".*main.cpp" --txt
python -m gcovr -r .. --filter ".*src/.*" --exclude ".*test/.*" --exclude ".*_deps/.*" --exclude ".*main.cpp" --html-details coverage.html

if exist coverage.html (
    echo.
    echo [УСПЕХ] Отчет о покрытии успешно сохранен в build/coverage.html
    start "" coverage.html
) else (
    echo.
    echo [ВНИМАНИЕ] Не удалось сгенерировать HTML отчет. Проверьте ошибки выше.
)

cd ..
pause