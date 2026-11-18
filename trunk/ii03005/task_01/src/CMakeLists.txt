cmake_minimum_required(VERSION 3.16)

project(Task01 LANGUAGES CXX)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
message(STATUS "Python for plotting: ${Python3_EXECUTABLE}")

file(TO_CMAKE_PATH "${Python3_EXECUTABLE}" PYTHON_PATH)
file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/plot.py" PLOT_PATH)
file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}" OUTPUT_PATH)

configure_file(plot.py "${CMAKE_CURRENT_BINARY_DIR}/plot.py" COPYONLY)

add_executable(Task01 main.cpp)
target_compile_features(Task01 PRIVATE cxx_std_20)

target_compile_definitions(Task01 PRIVATE
    PYTHON_EXECUTABLE="${PYTHON_PATH}"
    PLOT_SCRIPT="${PLOT_PATH}"
    OUTPUT_DIRECTORY="${OUTPUT_PATH}"
)
