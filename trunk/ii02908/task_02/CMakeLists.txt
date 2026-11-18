cmake_minimum_required(VERSION 3.20)
project(task_02_tests CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Принудительная генерация PDB-файлов для всех конфигураций (включая Release)
if (MSVC)
    add_compile_options(/Zi)
    add_link_options(/DEBUG)
endif()

# ---- Visual Studio 2022 (MSVC) ----
if (NOT MSVC)
    message(FATAL_ERROR "This project is configured for MSVC (Visual Studio 2022).")
endif()

# ---- GoogleTest ----
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.zip
)
# Для Windows: предотвращает переопределение настроек компилятора родительского проекта
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ---- Исходники проекта ----
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
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

set_target_properties(tests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${SRC_DIR}"
)

enable_testing()
add_test(NAME all_tests COMMAND tests WORKING_DIRECTORY "${SRC_DIR}")
