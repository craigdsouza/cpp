CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include

all: ex1.exe ex2.exe ex3.exe integration.exe

ex1.exe: ex1_types.cpp src/road_types.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

ex2.exe: ex2_graph.cpp src/road_types.cpp src/road_graph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

ex3.exe: ex3_bfs.cpp src/road_types.cpp src/road_graph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

integration.exe: integration.cpp src/road_types.cpp src/road_graph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.exe

.PHONY: all clean
