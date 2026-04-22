# Exercise 3 — Write a CMakeLists.txt that builds main_ex1 and main_ex2
# Then add integration as a third target in Exercise 4.

# cmake_minimum_required(VERSION 3.15)
# project(day11)

# set(CMAKE_CXX_STANDARD 17)
# add_compile_options(-Wall -Wextra)

# --- Target 1: main_ex1 ---
# add_executable(main_ex1
#     main_ex1.cpp
#     src/gps_waypoint.cpp
# )
# target_include_directories(main_ex1 PRIVATE include)

# --- Target 2: main_ex2 ---
# add_executable(main_ex2
#     main_ex2.cpp
#     src/gps_waypoint.cpp
#     src/waypoint_loader.cpp
# )
# target_include_directories(main_ex2 PRIVATE include)

# --- Target 3: integration (Exercise 4) ---
# add_executable(integration
#     integration.cpp
#     src/map_tile.cpp
#     src/tile_index.cpp
# )
# target_include_directories(integration PRIVATE include)
