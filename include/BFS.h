#pragma once
#include "Maze.h"
#include <vector>
#include <utility>

struct SolverResult {
    std::vector<std::pair<int,int>> path;   // ordered path from S to E
    int visitedNodes = 0;
    double timeMs    = 0.0;
};

class BFS {
public:
    // Solve the maze using Breadth-First Search (shortest path, unweighted)
    static SolverResult solve(const Maze& maze);
};
