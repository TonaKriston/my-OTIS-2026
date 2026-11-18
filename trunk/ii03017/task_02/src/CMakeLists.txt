# Модели (Model*.cpp) и входные сигналы (InputSignal.cpp) НЕ копируются:
# task_02 использует код из соседнего каталога task_01/src.
# Каталог можно переопределить: -DOTIS_TASK01_SRC=<путь>
set(OTIS_TASK01_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../task_01/src"
    CACHE PATH "Каталог с исходным кодом задания 1")
get_filename_component(OTIS_TASK01_SRC "${OTIS_TASK01_SRC}" ABSOLUTE)

if(NOT EXISTS "${OTIS_TASK01_SRC}/Model.h")
    message(FATAL_ERROR
        "Не найден код задания 1: '${OTIS_TASK01_SRC}/Model.h'. "
        "Ожидается структура trunk/ii0xxyy/task_01/src рядом с task_02.")
endif()

# Ядро: код task_01 + новые модули task_02.
add_library(otis_task_02_core
    "${OTIS_TASK01_SRC}/Model1_3.cpp"
    "${OTIS_TASK01_SRC}/Model2_5.cpp"
    "${OTIS_TASK01_SRC}/Model3_7.cpp"
    "${OTIS_TASK01_SRC}/InputSignal.cpp"
    StabilityAnalyzer.cpp
    Simulation.cpp
    ApplicationLogic.cpp
)
target_include_directories(otis_task_02_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${OTIS_TASK01_SRC}"
)
target_compile_features(otis_task_02_core PUBLIC cxx_std_17)

add_executable(otis_task_02 main.cpp)
target_link_libraries(otis_task_02 PRIVATE otis_task_02_core)

# Скрипт графика тоже берётся из task_01 и кладётся рядом с exe,
# потому что results.csv пишется в рабочий каталог программы.
configure_file("${OTIS_TASK01_SRC}/plot.py" plot.py COPYONLY)
