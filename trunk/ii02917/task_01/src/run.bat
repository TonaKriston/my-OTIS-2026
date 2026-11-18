@echo off
chcp 65001 >nul

:: Сначала собираем
call build.bat

:: Потом запускаем
echo.
echo Запуск программы...
echo.
lab01.exe

:: После завершения программы строим график
echo.
echo Построение графиков...
python plot.py

pause