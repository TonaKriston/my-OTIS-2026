cmake_minimum_required(VERSION 3.10)
project(OTIS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)

add_executable(lab01
    main.cpp
)

target_include_directories(lab01 
PRIVATE 
${CMAKE_CURRENT_LIST_DIR})