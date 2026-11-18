cmake_minimum_required(VERSION 3.16)
project(Lab1Variant12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(lab_models STATIC models.cpp simulation.cpp)
target_include_directories(lab_models PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(lab1 main.cpp)
target_link_libraries(lab1 PRIVATE lab_models)
if(WIN32)
    target_link_libraries(lab1 PRIVATE shell32)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(model_tests tests/test_models.cpp)
    target_link_libraries(model_tests PRIVATE lab_models)
    add_test(NAME variant12_model_tests COMMAND model_tests)
endif()
