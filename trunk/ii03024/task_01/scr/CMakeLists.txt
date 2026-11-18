cmake_minimum_required(VERSION 3.10)
project(task_01_sim)

set(CMAKE_CXX_STANDARD 17)

# Перечисляем все наши файлы исходного кода
add_executable(task_01_sim 
    main.cpp 
    model11.cpp 
    model28.cpp 
    model36.cpp
)