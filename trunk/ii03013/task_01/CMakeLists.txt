cmake_minimum_required(VERSION 3.20)

project(task_01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(task_01
    src/main.cpp
    src/Model1_10.cpp
    src/Model2_2.cpp
    src/Model3_4.cpp
    src/InputSignals.cpp
)

target_include_directories(task_01 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(task_01 PRIVATE /W4)
else()
    target_compile_options(task_01 PRIVATE -Wall -Wextra -Wpedantic)
endif()