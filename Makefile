CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude
TARGET   = MazeSolver
SRCDIR   = src
SRCS     = $(SRCDIR)/Maze.cpp \
           $(SRCDIR)/BFS.cpp  \
           $(SRCDIR)/DFS.cpp  \
           $(SRCDIR)/Dijkstra.cpp \
           $(SRCDIR)/AStar.cpp    \
           $(SRCDIR)/main.cpp
OBJS     = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
