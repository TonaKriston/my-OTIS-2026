add_executable(LabWork1
    main.cpp
    InputSignal.cpp
    Model1_8.cpp
    Model2_10.cpp
    Model3_2.cpp
)

target_include_directories(LabWork1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(LabWork1 PRIVATE cxx_std_17)