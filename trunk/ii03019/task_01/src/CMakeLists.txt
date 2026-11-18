cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME "ii03019_task_01")

add_executable(${TARGET_NAME}
    main.cpp
    InputSignal.cpp
    InputSignal.h
    Model.h
    Model1_1.cpp
    Model1_1.h
    Model2_5.cpp
    Model2_5.h
    Model3_9.cpp
    Model3_9.h
)

set_target_properties(${TARGET_NAME} PROPERTIES 
    OUTPUT_NAME "task_01"
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)