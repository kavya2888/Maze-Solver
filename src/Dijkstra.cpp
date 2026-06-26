#include "Dijkstra.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <climits>
#include <chrono>
#include <algorithm>

static const int DR[] = {-1, 1,  0, 0};
static const int DC[] = { 0, 0, -1, 1};

static inline int encode(int r, int c, int cols) { return r * cols + c; }

// Min-heap entry: {cost, encoded_position}
using Entry = std::pair<int,int>;

SolverResult Dijkstra::solve(const Maze& maze) {
    auto t0 = std::chrono::high_resolution_clock::now();

    SolverResult result;
    auto [sr, sc] = maze.getStart();
    auto [er, ec] = maze.getEnd();
    int rows = maze.getRows();
    int cols = maze.getCols();

    int total = rows * cols;
    std::vector<int> dist(total, INT_MAX);
    std::vector<int> parent(total, -1);

    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    int startKey = encode(sr, sc, cols);
    int endKey   = encode(er, ec, cols);

    dist[startKey] = 0;
    pq.push({0, startKey});

    bool found = false;

    while (!pq.empty() && !found) {
        auto [cost, cur] = pq.top(); pq.pop();

        if (cost > dist[cur]) continue;   // stale entry
        ++result.visitedNodes;

        if (cur == endKey) { found = true; break; }

        int r = cur / cols;
        int c = cur % cols;

        for (int d = 0; d < 4; ++d) {
            int nr = r + DR[d];
            int nc = c + DC[d];
            if (!maze.inBounds(nr, nc) || maze.isWall(nr, nc)) continue;

            int nk  = encode(nr, nc, cols);
            int w   = maze.getWeight(nr, nc);
            int newCost = cost + w;

            if (newCost < dist[nk]) {
                dist[nk]   = newCost;
                parent[nk] = cur;
                pq.push({newCost, nk});
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
