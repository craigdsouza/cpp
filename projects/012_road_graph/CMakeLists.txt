cmake_minimum_required(VERSION 3.15)
project(day12)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

add_executable(ex1
    ex1_types.cpp
    src/road_types.cpp
)
target_include_directories(ex1 PRIVATE include)

add_executable(ex2
    ex2_graph.cpp
    src/road_types.cpp
    src/road_graph.cpp
)
target_include_directories(ex2 PRIVATE include)

add_executable(ex3
    ex3_bfs.cpp
    src/road_types.cpp
    src/road_graph.cpp
)
target_include_directories(ex3 PRIVATE include)

add_executable(integration
    integration.cpp
    src/road_types.cpp
    src/road_graph.cpp
)
target_include_directories(integration PRIVATE include)
