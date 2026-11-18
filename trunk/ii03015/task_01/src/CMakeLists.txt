cmake_minimum_required(VERSION 3.15)
project(OTIS_Task_01_Variant_6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(otis_task_01_ii03015
    main.cpp
    Model1_6.cpp
    Model2_8.cpp
    Model3_10.cpp
    InputSignal.cpp
)
add_custom_command(TARGET otis_task_01_ii03015 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/plot.py"
        "$<TARGET_FILE_DIR:otis_task_01_ii03015>/plot.py"
)
