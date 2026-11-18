cmake_minimum_required(VERSION 3.14)
project(Lab2_Testing VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

add_library(src_lib STATIC
    src/LinearModel13.cpp
    src/NonlinearModel2_10.cpp
    src/DiffEqModel3_2.cpp
    src/StabilityAnalyzer.cpp
)
target_include_directories(src_lib PUBLIC src)

add_executable(Lab1_Modeling src/main.cpp)
target_link_libraries(Lab1_Modeling PRIVATE src_lib)

enable_testing()
add_subdirectory(test)