cmake_minimum_required(VERSION 4.4)

project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (otis 
    main.cpp
    models.cpp
    models.h
)
