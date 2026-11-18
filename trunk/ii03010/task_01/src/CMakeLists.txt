cmake_minimum_required(VERSION 3.10)

project(ii03010_task_01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ii03010_task_01
    main.cpp
)

configure_file(plot_config.h.in "${CMAKE_CURRENT_BINARY_DIR}/plot_config.h" @ONLY)
target_include_directories(ii03010_task_01 PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

if(WIN32)
    target_link_libraries(ii03010_task_01 PRIVATE shell32)
endif()
