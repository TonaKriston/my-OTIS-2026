cmake_minimum_required(VERSION 3.14)
project(task_02 CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_subdirectory(src)   # программа не собирается: тесты от main не зависят
enable_testing()
add_subdirectory(test)