@echo off
cmake -B build -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="D:/VSCODE/ucrt64/bin/mingw32-make.exe"
cmake --build build
pause