#include "DFS.h"
#include <stack>
#include <unordered_map>
#include <chrono>
#include <algorithm>

static const int DR[] = {-1, 1,  0, 0};
static const int DC[] = { 0, 0, -1, 1};

static inline int encode(int r, int c, int cols) { return r * cols + c; }

SolverResult DFS::solve(const Maze& maze) {
    auto t0 = std::chrono::high_resolution_clock::now();

    SolverResult result;
    auto [sr, sc] = maze.getStart();
    auto [er, ec] = maze.getEnd();
    int cols = maze.getCols();

    std::stack<int> stk;
    std::unordered_map<int,int> parent;  // child -> parent

    int startKey = encode(sr, sc, cols);
    int endKey   = encode(er, ec, cols);

    stk.push(startKey);
    parent[startKey] = -1;

    bool found = false;

    while (!stk.empty() && !found) {
        int cur = stk.top(); stk.pop();
        ++result.visitedNodes;

        if (cur == endKey) { found = true; break; }

        int r = cur / cols;
        int c = cur % cols;

        for (int d = 0; d < 4; ++d) {
            int nr = r + DR[d];
            int nc = c + DC[d];
            if (!maze.inBounds(nr, nc) || maze.isWall(nr, nc)) continue;

            int nk = encode(nr, nc, cols);
            if (parent.count(nk)) continue;

            parent[nk] = cur;
            stk.push(nk);
        }
    }

    // Reconstruct path
    if (found) {
        int cur = endKey;
        while (cur != -1) {
            result.path.push_back({cur / cols, cur % cols});
            cur = parent[cur];
        }
        std::reverse(result.path.begin(), result.path.end());
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    result.timeMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
    return result;
}
