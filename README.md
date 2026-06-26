# Maze Generation & Pathfinding Visualizer

A C++ console application that generates random mazes and computes optimal
paths using four classic graph-traversal algorithms.

---

## Features

| Feature | Details |
|---|---|
| Maze Generation | Recursive Backtracker (DFS carving) with optional weighted terrain |
| BFS | Shortest path (unweighted) via queue |
| DFS | Depth-first exploration via stack |
| Dijkstra | Minimum-cost path (weighted terrain) via priority queue |
| A\* | Fastest pathfinder using Manhattan-distance heuristic |
| Comparison Table | Side-by-side time, visited nodes, and path length |

---

## Project Structure

```
MazeSolver/
├── include/
│   ├── Maze.h          – Maze class declaration
│   ├── BFS.h           – BFS solver
│   ├── DFS.h           – DFS solver
│   ├── Dijkstra.h      – Dijkstra solver
│   └── AStar.h         – A* solver
│
├── src/
│   ├── Maze.cpp        – Maze generation, loading, printing
│   ├── BFS.cpp
│   ├── DFS.cpp
│   ├── Dijkstra.cpp
│   ├── AStar.cpp
│   └── main.cpp        – Interactive menu & comparison framework
│
├── mazes/
│   └── sample.txt      – Pre-built sample maze
│
├── Makefile
└── README.md
```

---

## Build & Run

```bash
# Build
make

# Run
./MazeSolver

# Or in one step
make run

# Clean build artefacts
make clean
```

Requires **g++ 7+** with C++17 support.

---

## Map Legend

| Symbol | Meaning |
|---|---|
| `S` | Start |
| `E` | End |
| `#` | Wall |
| `.` | Open path (cost 1) |
| `M` | Mud (cost 3) |
| `W` | Water (cost 5) |
| `*` | Solution path |

---

## Algorithm Summary

### BFS (Breadth-First Search)
- Uses a **queue** (FIFO)
- Guarantees **shortest path** in unweighted grids
- Explores level by level

### DFS (Depth-First Search)
- Uses a **stack** (LIFO) – iterative implementation
- Does **not** guarantee shortest path
- Explores as deep as possible first

### Dijkstra's Algorithm
- Uses a **min-heap priority queue**
- Guarantees **minimum-cost path** in weighted grids
- Optimal for heterogeneous terrain (Mud / Water)

### A\* Search
- Uses a **min-heap** with `f(n) = g(n) + h(n)`
- `g(n)` = actual cost from start
- `h(n)` = Manhattan distance to goal
- **Fastest** practical pathfinder; optimal when heuristic is admissible

---

## Sample Output

```
Generated Maze (15×25):
#########################
S.....#.....#...........#
#.###.#.###.#.#########.#
#.#...#.#...#.........#.#
#.#.###.#.#############.#
#.#.....#...............#
#.#####.###############.#
#.......#...............#
#######.#.#############.#
#.......#.............#.#
#.#####.#############.#.#
#.#.....#.............#.#
#.#.#####.############..#
#...#.................#.E
#########################

Algorithm Comparison:
Algorithm   Time (ms)     Nodes         Path Len
--------------------------------------------------
DFS         0.041         183           89
BFS         0.018          67           27
Dijkstra    0.024          58           27
A*          0.009          35           27
```

---

## Concepts Demonstrated

**Data Structures:** Queue · Stack · Priority Queue · Vector · Hash Map

**Algorithms:** BFS · DFS · Dijkstra · A\*

**OOP:** Classes · Encapsulation · Abstraction · Modular design
