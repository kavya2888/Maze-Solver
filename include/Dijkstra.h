#pragma once
#include "Maze.h"
#include "BFS.h"
#include <vector>
#include <utility>

class Dijkstra {
public:
    // Solve weighted maze using Dijkstra's algorithm (minimum-cost path)
    static SolverResult solve(const Maze& maze);
};
