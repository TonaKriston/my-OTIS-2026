#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
cmake -S . -B build -DOTIS_BUILD_TESTS=ON -DOTIS_ENABLE_COVERAGE=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure --output-junit build/test-results.xml
if command -v gcovr >/dev/null 2>&1; then
  # root = trunk/ii03017, чтобы в отчёт попал и код моделей из task_01/src
  gcovr --root .. --filter '.*/task_0[12]/src/.*' --exclude '.*/task_02/src/main.cpp' --xml-pretty --xml coverage.xml --html-details coverage.html build
else
  echo "gcovr not installed; install it with: python -m pip install gcovr"
fi
