cmake_minimum_required(VERSION 3.16)
project(OTIS_Variant2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(otis_variant2
    main.cpp
    models.cpp
    signals.cpp
    simulator.cpp
)

if(MSVC)
    target_compile_options(otis_variant2 PRIVATE /W4 /permissive-)
else()
    target_compile_options(otis_variant2 PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Make the plotting script available beside the executable after every configure.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/plot.py
               ${CMAKE_CURRENT_BINARY_DIR}/plot.py COPYONLY)
add_custom_command(TARGET otis_variant2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/plot.py
            $<TARGET_FILE_DIR:otis_variant2>/plot.py
    COMMENT "Copying plot.py next to the executable")
