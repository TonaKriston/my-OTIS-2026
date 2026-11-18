#!/usr/bin/env bash
# Аналог build.bat для macOS / Linux.
# Запуск: ./build.sh   (если "Permission denied": chmod +x build.sh)

readonly SEPARATOR="======================================="
readonly LINE="---------------------------------------"

cd "$(dirname "$0")" || exit 1

echo "$SEPARATOR"
echo " OTIS-2026  Task 01  Variant 4"
echo " Build process started"
echo "$SEPARATOR"

if ! command -v cmake > /dev/null 2>&1; then
    echo "[ERROR] CMake not found. Install it: brew install cmake" >&2
    exit 1
fi

echo
echo "[1/3] Configuration (cmake)"
echo "$LINE"
if ! cmake -S . -B build -DCMAKE_BUILD_TYPE=Release; then
    echo "[ERROR] Configuration failed" >&2
    exit 1
fi

echo
echo "[2/3] Compilation"
echo "$LINE"
if ! cmake --build build --config Release; then
    echo "[ERROR] Build failed" >&2
    exit 1
fi

echo
echo "[3/3] Running the program"
echo "$LINE"
echo "SUCCESS: build completed, executable is in build/bin"
echo
(cd build/bin && ./ii03004_task_01)

echo
echo "$SEPARATOR"
echo " Done"
echo "$SEPARATOR"
exit 0
