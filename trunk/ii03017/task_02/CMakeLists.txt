cmake_minimum_required(VERSION 3.16)
project(OTIS_Task_02_Variant_3 LANGUAGES CXX)

include(CTest)
option(OTIS_BUILD_TESTS "Build Google Test unit tests" ON)
option(OTIS_ENABLE_COVERAGE "Enable GCC coverage instrumentation" OFF)
if(OTIS_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

if(OTIS_BUILD_TESTS AND BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        GIT_SHALLOW TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(otis_tests
        test/test_models.cpp
        test/test_stability.cpp
        test/test_application_logic.cpp
        test/test_main_input.cpp
    )
    # otis_task_02_core уже содержит код task_01 (модели, сигналы) и пути к его заголовкам.
    target_link_libraries(otis_tests PRIVATE otis_task_02_core GTest::gtest_main GTest::gmock)
    target_include_directories(otis_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

    include(GoogleTest)
    gtest_discover_tests(otis_tests)
endif()
