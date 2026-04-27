CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRCS = $(wildcard *.cpp)
TARGETS = $(SRCS:.cpp=.exe)

all: $(TARGETS)

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
