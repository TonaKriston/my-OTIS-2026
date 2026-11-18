#!/bin/bash
set -e

cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

echo " Сборка проекта..."

rm -rf build
mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

echo " Запуск модульных тестов (Google Test)..."
./tests

echo " Отчёт о покрытии кода"
gcovr -r .. --gcov-executable "xcrun llvm-cov gcov" \
  --exclude "../test" --exclude ".*_deps.*" --exclude ".*main.cpp"

cd ..
