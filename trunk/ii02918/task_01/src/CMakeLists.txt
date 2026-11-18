cmake_minimum_required(VERSION 3.15)

project(ii02918_task_01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ii02918_task_01
    main.cpp
    DynamicModel.h
    StateDelayModel.h
    SaturationModel.h
    QuadraticDecayODE.h
    SignalGenerator.h
    ModelCreator.h
)