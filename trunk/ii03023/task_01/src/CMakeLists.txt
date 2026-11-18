cmake_minimum_required(VERSION 3.10)
project(task_01_ii03021 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Строго локальный поиск файлов только в текущей папки src (никаких ../)
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

# Создаем уникальный таргет для себя
add_executable(task_01_ii03021 ${SOURCES} ${HEADERS})

if(MSVC)
    target_compile_options(task_01_ii03021 PRIVATE /utf-8)
endif()