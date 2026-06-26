#pragma once
#include "Maze.h"
#include "BFS.h"
#include <vector>
#include <utility>

class AStar {
public:
    // Solve maze using A* with Manhattan distance heuristic
    static SolverResult solve(const Maze& maze);

private:
    // h(n) = Manhattan distance to goal
    static int heuristic(int r, int c, int goalR, int goalC);
};
