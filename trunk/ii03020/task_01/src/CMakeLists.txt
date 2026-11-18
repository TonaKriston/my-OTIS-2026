cmake_minimum_required(VERSION 3.15)
project(sim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sim
    main.cpp
    InputGenerator.cpp
    Linear_Model.cpp
    Non_Linear_Model.cpp
    Simple_Differential_Equation.cpp
    TableOutput.cpp
    Plotter.cpp
)

if(MSVC)
    target_compile_options(sim PRIVATE /W4)
else()
    target_compile_options(sim PRIVATE -Wall -Wextra -O2)
endif()