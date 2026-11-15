cmake_minimum_required(VERSION 3.15)
project(lab1_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to GoogleTest (can be overridden via -DGTEST_DIR=...)
if(NOT DEFINED GTEST_DIR)
    set(GTEST_DIR "C:/libs/googletest")
endif()

if(NOT EXISTS "${GTEST_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "GoogleTest not found at: ${GTEST_DIR}\n"
                        "Set path via -DGTEST_DIR=<path_to_googletest>")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()

# Main library (code under test). main.cpp is NOT included here.
add_library(models STATIC
    src/models.cpp
)
target_include_directories(models PUBLIC src)

# Test executables
set(TEST_SOURCES
    test/test_models_parametrized.cpp
    test/test_run_mockmodel.cpp
    test/test_stability_signal.cpp
)

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE
        models
        gtest
        gmock
        gtest_main
        gmock_main
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()