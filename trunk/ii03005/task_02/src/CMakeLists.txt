cmake_minimum_required(VERSION 3.16)

project(Task02 LANGUAGES CXX)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
message(STATUS "Python for plotting: ${Python3_EXECUTABLE}")

file(TO_CMAKE_PATH "${Python3_EXECUTABLE}" PYTHON_PATH)
file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/plot.py" PLOT_PATH)
file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}" OUTPUT_PATH)

configure_file(plot.py "${CMAKE_CURRENT_BINARY_DIR}/plot.py" COPYONLY)

add_library(models
    models.cpp
    stability_analyzer.cpp
)
target_compile_features(models PUBLIC cxx_std_20)
target_compile_options(models PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(models PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(models PRIVATE
        $<$<CONFIG:Debug>:-fprofile-arcs -ftest-coverage>
    )
endif()

add_executable(Task02 main.cpp)
target_compile_features(Task02 PRIVATE cxx_std_20)
target_link_libraries(Task02 PRIVATE models)
target_compile_definitions(Task02 PRIVATE
    PYTHON_EXECUTABLE="${PYTHON_PATH}"
    PLOT_SCRIPT="${PLOT_PATH}"
    OUTPUT_DIRECTORY="${OUTPUT_PATH}"
)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(Task02Tests
    ../test/models_test.cpp
)
target_compile_features(Task02Tests PRIVATE cxx_std_20)
target_link_libraries(Task02Tests PRIVATE models GTest::gtest_main)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(Task02 PRIVATE
        $<$<CONFIG:Debug>:-fprofile-arcs -ftest-coverage>
    )
    target_compile_options(Task02Tests PRIVATE
        $<$<CONFIG:Debug>:-fprofile-arcs -ftest-coverage>
    )
    target_link_options(Task02Tests PRIVATE
        $<$<CONFIG:Debug>:-fprofile-arcs -ftest-coverage>
    )
endif()

include(GoogleTest)
gtest_discover_tests(Task02Tests)
