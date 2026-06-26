#include "AStar.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <climits>
#include <cstdlib>
#include <chrono>
#include <algorithm>

static const int DR[] = {-1, 1,  0, 0};
static const int DC[] = { 0, 0, -1, 1};

static inline int encode(int r, int c, int cols) { return r * cols + c; }

int AStar::heuristic(int r, int c, int goalR, int goalC) {
    return std::abs(r - goalR) + std::abs(c - goalC);   // Manhattan distance
}

// Min-heap entry: {f(n) = g(n)+h(n), encoded_position}
using Entry = std::pair<int,int>;

SolverResult AStar::solve(const Maze& maze) {
    auto t0 = std::chrono::high_resolution_clock::now();

    SolverResult result;
    auto [sr, sc] = maze.getStart();
    auto [er, ec] = maze.getEnd();
    int rows = maze.getRows();
    int cols = maze.getCols();

    int total = rows * cols;
    std::vector<int> gCost(total, INT_MAX);  // g(n): actual cost from start
    std::vector<int> parent(total, -1);

    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    int startKey = encode(sr, sc, cols);
    int endKey   = encode(er, ec, cols);

    gCost[startKey] = 0;
    int h0 = heuristic(sr, sc, er, ec);
    pq.push({h0, startKey});

    bool found = false;

    while (!pq.empty() && !found) {
        auto [f, cur] = pq.top(); pq.pop();

        int r = cur / cols;
        int c = cur % cols;

        // Check if f is consistent with current g (skip stale)
        int curG = gCost[cur];
        if (f > curG + heuristic(r, c, er, ec)) continue;

        ++result.visitedNodes;

        if (cur == endKey) { found = true; break; }

        for (int d = 0; d < 4; ++d) {
            int nr = r + DR[d];
            int nc = c + DC[d];
            if (!maze.inBounds(nr, nc) || maze.isWall(nr, nc)) continue;

            int nk    = encode(nr, nc, cols);
            int w     = maze.getWeight(nr, nc);
            int newG  = curG + w;

            if (newG < gCost[nk]) {
                gCost[nk]  = newG;
                parent[nk] = cur;
                int fNew   = newG + heuristic(nr, nc, er, ec);
                pq.push({fNew, nk});
            }
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
