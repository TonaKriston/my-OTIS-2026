cmake_minimum_required(VERSION 3.15)
project(OTIS_Lab2_Var19)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

add_compile_options(--coverage -O0 -g)
add_link_options(--coverage)

# Подключение исходников первой лабораторной
set(OTIS_TASK01_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../task_01/src")

# Общие исходники
set(CORE
    "${OTIS_TASK01_SRC}/model19.cpp"
    "${OTIS_TASK01_SRC}/model23.cpp"
    "${OTIS_TASK01_SRC}/model37.cpp"
    src/stability.cpp
    src/simulation.cpp
)

add_executable(task_02
    src/main.cpp
    ${CORE}
)

target_include_directories(task_02 PRIVATE
    src
    "${OTIS_TASK01_SRC}"
)

# Подключение Google Test
include(FetchContent)

FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

# Тесты
add_executable(tests
    test/stability_checks.cpp
    test/model_checks.cpp
    test/simulation_checks.cpp
    ${CORE}
)

target_include_directories(tests PRIVATE
    src
    "${OTIS_TASK01_SRC}"
)

target_link_libraries(tests GTest::gtest_main)
