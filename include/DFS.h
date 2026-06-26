#pragma once
#include "Maze.h"
#include "BFS.h"   // reuse SolverResult
#include <vector>
#include <utility>

class DFS {
public:
    // Solve the maze using Depth-First Search (not guaranteed shortest path)
    static SolverResult solve(const Maze& maze);
};
