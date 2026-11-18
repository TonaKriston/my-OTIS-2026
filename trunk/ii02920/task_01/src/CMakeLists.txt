cmake_minimum_required(VERSION 3.15)
project(ii02911_task_01_src LANGUAGES CXX)

add_executable(ii02911_task_01 main.cpp)
target_compile_features(ii02911_task_01 PRIVATE cxx_std_17)
set_target_properties(ii02911_task_01 PROPERTIES CXX_EXTENSIONS OFF)

if(MSVC)
    target_compile_options(ii02911_task_01 PRIVATE /W4 /utf-8)
else()
    target_compile_options(ii02911_task_01 PRIVATE -Wall -Wextra -Wpedantic)
endif()
