set(TASK01_SRC "../../task_01/src")

add_executable(ii03025_task_02
    main.cpp
    "${TASK01_SRC}/model19.cpp"
    "${TASK01_SRC}/model23.cpp"
    "${TASK01_SRC}/model37.cpp"
    stability.cpp
    simulation.cpp
)
