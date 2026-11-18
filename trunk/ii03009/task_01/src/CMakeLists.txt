cmake_minimum_required(VERSION 3.15)
project(lab1_ii03009 CXX) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab1_ii03009 
    main.cpp
    linear_model_1_9.cpp
    nonlinear_model_2_9.cpp
    diff_eq_3_9.cpp
)
