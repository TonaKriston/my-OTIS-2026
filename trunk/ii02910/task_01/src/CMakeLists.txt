cmake_minimum_required(VERSION 3.10)

project(ii02910_task_01_src LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ii02910_task_01
    main.cpp
    LinearARXModel.cpp
    SaturationModel.cpp
    CubicODE.cpp
    InputGenerator.cpp
    SystemFactory.cpp
)
target_compile_options(ii02910_task_01 PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
)