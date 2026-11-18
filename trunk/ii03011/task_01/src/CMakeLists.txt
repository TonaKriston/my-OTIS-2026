cmake_minimum_required(VERSION 3.15)

project(ii03011_task_01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ii03011_task_01
    main.cpp
    Model.cpp
    Input.cpp
)

target_include_directories(ii03011_task_01 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)