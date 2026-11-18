# Google Test скачивается при первой сборке (нужен интернет один раз).
include(FetchContent)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Флаги --coverage ставятся только на тестовый exe (не на main.cpp),
# поэтому в отчёт попадания попадают только тестируемые модули.
option(TEST_COVERAGE "Собрать тесты с покрытием gcov" OFF)

add_executable(task2_ii03007_tests
    test_models.cpp
    test_stability.cpp
    test_signal.cpp)

target_include_directories(task2_ii03007_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(task2_ii03007_tests PRIVATE GTest::gtest_main)

if(TEST_COVERAGE)
    target_compile_options(task2_ii03007_tests PRIVATE -g -O0 --coverage)
    target_link_options(task2_ii03007_tests PRIVATE --coverage)
endif()

# Каждый тест (включая копии TEST_P) регистрируется в ctest отдельно.
include(GoogleTest)
gtest_discover_tests(task2_ii03007_tests)
