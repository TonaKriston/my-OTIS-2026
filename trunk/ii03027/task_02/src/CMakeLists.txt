add_library(src_lib STATIC
    LinearModel13.cpp
    NonlinearModel2_10.cpp
    DiffEqModel3_2.cpp
    StabilityAnalyzer.cpp
)
target_include_directories(src_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})