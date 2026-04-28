cmake_minimum_required(VERSION 3.14)
project(015_astar CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# nlohmann/json is installed system-wide via:
#   sudo apt install nlohmann-json3-dev
# The header lands at /usr/include/nlohmann/json.hpp — no special linking needed.

set(SHARED_SRCS src/road_types.cpp src/road_graph.cpp)
set(INCLUDES include)

add_executable(ex1 ex1_heuristic.cpp ${SHARED_SRCS})
target_include_directories(ex1 PRIVATE ${INCLUDES})

add_executable(ex2 ex2_astar.cpp ${SHARED_SRCS})
target_include_directories(ex2 PRIVATE ${INCLUDES})

add_executable(ex3 ex3_benchmark.cpp ${SHARED_SRCS})
target_include_directories(ex3 PRIVATE ${INCLUDES})

add_executable(integration integration.cpp ${SHARED_SRCS})
target_include_directories(integration PRIVATE ${INCLUDES})
