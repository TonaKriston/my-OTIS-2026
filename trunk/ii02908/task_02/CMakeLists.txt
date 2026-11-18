cmake_minimum_required(VERSION 4.4)
project(task_02_tests CXX)

set(CMAKE_CXX_STANDARD 20)

# Принудительная генерация PDB-файлов для всех конфигураций (включая Release)
# эти файлы нужны для анализа покрытия
if (MSVC)
    add_compile_options(/Zi)
    add_link_options(/DEBUG)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Visual Studio 2022 (MSVC) ----
if (NOT MSVC)
    message(FATAL_ERROR "This project is configured for MSVC (Visual Studio 2022).")
endif()

# ---- GoogleTest из локальной папки ----
set(GTEST_ROOT "C:/libs/googletest-master/googletest-main" CACHE PATH "Path to googletest source")
add_subdirectory(${GTEST_ROOT} ${CMAKE_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)

# ---- Исходники проекта (без main.cpp!) ----
set(SRC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

add_library(models_lib STATIC
    "${SRC_DIR}/models.cpp"
)
target_include_directories(models_lib PUBLIC "${SRC_DIR}")

# ---- Тестовый исполняемый файл ----
add_executable(tests
    "${TEST_DIR}/test_models_parametrized.cpp"
    "${TEST_DIR}/test_run_mockmodel.cpp"
    "${TEST_DIR}/test_stability_signal.cpp"
)

target_compile_options(tests PRIVATE /Zc:twoPhase-)

target_link_libraries(tests PRIVATE
    models_lib
    gtest
    gtest_main
    gmock
    gmock_main
)

# ---- Рабочая директория = папка src (там лежат .gp и ожидаемые csv) ----
set_target_properties(tests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${SRC_DIR}"
)

# ---- Копируем gtest/gmock DLL рядом с exe, если они собираются как shared ----
enable_testing()
add_test(NAME all_tests COMMAND tests WORKING_DIRECTORY "${SRC_DIR}")
