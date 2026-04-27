cmake_minimum_required(VERSION 3.10)
project(dijkstra CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SHARED_SRCS src/road_types.cpp src/road_graph.cpp)

add_executable(ex1 ex1_priority_queue.cpp)
target_include_directories(ex1 PRIVATE include)

add_executable(ex2 ex2_dijkstra.cpp ${SHARED_SRCS})
target_include_directories(ex2 PRIVATE include)

add_executable(ex3 ex3_shortest_path.cpp ${SHARED_SRCS})
target_include_directories(ex3 PRIVATE include)

add_executable(integration integration.cpp ${SHARED_SRCS})
target_include_directories(integration PRIVATE include)
