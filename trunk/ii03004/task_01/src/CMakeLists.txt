cmake_minimum_required(VERSION 3.15)

project(OTIS_Task_01_Variant_4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Уникальное имя цели, чтобы не конфликтовать с работами других студентов
# при сборке всего репозитория из корня.
set(TARGET_NAME ii03004_task_01)

add_executable(${TARGET_NAME}
    main.cpp
    Model.cpp
    LinearModels.cpp
    NonLinearModels.cpp
    DiffEquations.cpp
    InputSignal.cpp
    Simulator.cpp
)

if(MSVC)
    # Исходники в UTF-8 (русские строки в консоли)
    target_compile_options(${TARGET_NAME} PRIVATE /utf-8 /W4)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra)
endif()

# Программа и plot.py кладутся в одну папку build/bin
# (для любого генератора, в том числе Visual Studio).
set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/bin>"
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/plot.py
               ${CMAKE_BINARY_DIR}/bin/plot.py COPYONLY)
