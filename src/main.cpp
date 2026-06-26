#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>

#include "Maze.h"
#include "BFS.h"
#include "DFS.h"
#include "Dijkstra.h"
#include "AStar.h"

// ─── Helpers ────────────────────────────────────────────────────────────────

static void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static void pause() {
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

static void printSeparator(char c = '-', int w = 50) {
    std::cout << std::string(w, c) << '\n';
}

static void printHeader(const std::string& title) {
    printSeparator('=');
    std::cout << "  " << title << '\n';
    printSeparator('=');
}

// ─── Display a solver result ─────────────────────────────────────────────────

static void showResult(const Maze& maze, const SolverResult& res,
                       const std::string& algoName) {
    printHeader(algoName + " Result");
    if (res.path.empty()) {
        std::cout << "  No path found!\n";
    } else {
        maze.print(res.path);
        std::cout << '\n';
        std::cout << "  Visited Nodes : " << res.visitedNodes << '\n';
        std::cout << "  Path Length   : " << res.path.size()  << '\n';
        std::cout << "  Time          : " << std::fixed << std::setprecision(3)
                  << res.timeMs << " ms\n";
    }
}

// ─── Comparison table ────────────────────────────────────────────────────────

static void runComparison(const Maze& maze) {
    printHeader("Algorithm Comparison");

    struct Row { std::string name; SolverResult res; };
    std::vector<Row> rows = {
        {"DFS",      DFS::solve(maze)},
        {"BFS",      BFS::solve(maze)},
        {"Dijkstra", Dijkstra::solve(maze)},
        {"A*",       AStar::solve(maze)},
    };

    std::cout << std::left
              << std::setw(12) << "Algorithm"
              << std::setw(14) << "Time (ms)"
              << std::setw(14) << "Nodes"
              << std::setw(14) << "Path Len"
              << '\n';
    printSeparator('-', 54);

    for (auto& row : rows) {
        std::cout << std::left
                  << std::setw(12) << row.name
                  << std::setw(14) << std::fixed << std::setprecision(3) << row.res.timeMs
                  << std::setw(14) << row.res.visitedNodes
                  << std::setw(14) << (row.res.path.empty() ? 0 : (int)row.res.path.size())
                  << '\n';
    }
    printSeparator('-', 54);
    std::cout << "\nLegend: Time = wall-clock ms  |  Nodes = cells explored\n";
}

// ─── Legend ──────────────────────────────────────────────────────────────────

static void printLegend() {
    std::cout << "\nMap Legend:\n";
    std::cout << "  S  = Start\n";
    std::cout << "  E  = End\n";
    std::cout << "  #  = Wall\n";
    std::cout << "  .  = Open (cost 1)\n";
    std::cout << "  M  = Mud  (cost 3)\n";
    std::cout << "  W  = Water(cost 5)\n";
    std::cout << "  *  = Path\n\n";
}

// ─── Main menu ───────────────────────────────────────────────────────────────

int main() {
    Maze maze;
    bool mazeReady = false;

    while (true) {
        clearScreen();
        printHeader("MAZE GENERATION & PATHFINDING VISUALIZER");
        std::cout << "  1. Generate Random Maze (unweighted)\n";
        std::cout << "  2. Generate Random Maze (weighted: Mud / Water)\n";
        std::cout << "  3. Load Maze from File  (mazes/sample.txt)\n";
        std::cout << "  4. Print Current Maze\n";
        std::cout << "  5. Solve with BFS\n";
        std::cout << "  6. Solve with DFS\n";
        std::cout << "  7. Solve with Dijkstra\n";
        std::cout << "  8. Solve with A*\n";
        std::cout << "  9. Compare All Algorithms\n";
        std::cout << "  0. Exit\n";
        printSeparator();
        std::cout << "  Choice: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            std::cout << "\nBye!\n";
            break;
        }

        clearScreen();

        switch (choice) {
            // ── 1: Unweighted random maze ──────────────────────────────────
            case 1: {
                int rows = 15, cols = 25;
                std::cout << "Maze size (rows cols) [default 15 25]: ";
                std::string line; std::getline(std::cin >> std::ws, line);
                if (!line.empty()) {
                    std::istringstream ss(line);
                    ss >> rows >> cols;
                }
                maze.generate(rows, cols, 0.30, false);
                maze.saveToFile("mazes/sample.txt");
                mazeReady = true;
                printHeader("Generated Maze");
                printLegend();
                maze.print();
                pause();
                break;
            }
            // ── 2: Weighted random maze ────────────────────────────────────
            case 2: {
                int rows = 15, cols = 25;
                std::cout << "Maze size (rows cols) [default 15 25]: ";
                std::string line; std::getline(std::cin >> std::ws, line);
                if (!line.empty()) {
                    std::istringstream ss(line);
                    ss >> rows >> cols;
                }
                maze.generate(rows, cols, 0.30, true);
                maze.saveToFile("mazes/sample.txt");
                mazeReady = true;
                printHeader("Generated Weighted Maze");
                printLegend();
                maze.print();
                pause();
                break;
            }
            // ── 3: Load from file ──────────────────────────────────────────
            case 3: {
                std::string fn = "mazes/sample.txt";
                if (maze.loadFromFile(fn)) {
                    mazeReady = true;
                    printHeader("Loaded: " + fn);
                    printLegend();
                    maze.print();
                } else {
                    std::cout << "Error: could not open " << fn << '\n';
                }
                pause();
                break;
            }
            // ── 4: Print current maze ──────────────────────────────────────
            case 4: {
                if (!mazeReady) { std::cout << "No maze loaded yet.\n"; pause(); break; }
                printHeader("Current Maze");
                printLegend();
                maze.print();
                pause();
                break;
            }
            // ── 5-8: Individual solvers ────────────────────────────────────
            case 5: {
                if (!mazeReady) { std::cout << "No maze loaded yet.\n"; pause(); break; }
                showResult(maze, BFS::solve(maze), "BFS");
                pause();
                break;
            }
            case 6: {
                if (!mazeReady) { std::cout << "No maze loaded yet.\n"; pause(); break; }
                showResult(maze, DFS::solve(maze), "DFS");
                pause();
                break;
            }
            case 7: {
                if (!mazeReady) { std::cout << "No maze loaded yet.\n"; pause(); break; }
                showResult(maze, Dijkstra::solve(maze), "Dijkstra");
                pause();
                break;
            }
            case 8: {
                if (!mazeReady) { std::cout << "No maze loaded yet.\n"; pause(); break; }
                showResult(maze, AStar::solve(maze), "A*");
                pause();
                break;
            }
            // ── 9: Comparison ─────────────────────────────────────────────
            case 9: {
                if (!mazeReady) { std::cout << "No maze loaded yet.\n"; pause(); break; }
                runComparison(maze);
                pause();
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
                pause();
                break;
        }
    }

    return 0;
}

// Required for istringstream in main
#include <sstream>
