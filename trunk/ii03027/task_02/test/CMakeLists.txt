cmake_minimum_required(VERSION 3.24)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(runTests test_models.cpp test_stability.cpp)
target_link_libraries(runTests PRIVATE GTest::gtest_main GTest::gmock)
target_link_libraries(runTests PRIVATE src_lib)

include(GoogleTest)
gtest_discover_tests(runTests)