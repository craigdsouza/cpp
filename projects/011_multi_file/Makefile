CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include

all: main_ex1.exe main_ex2.exe integration.exe

main_ex1.exe: main_ex1.cpp src/gps_waypoint.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

main_ex2.exe: main_ex2.cpp src/gps_waypoint.cpp src/waypoint_loader.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

integration.exe: integration.cpp src/map_tile.cpp src/tile_index.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.exe

.PHONY: all clean
