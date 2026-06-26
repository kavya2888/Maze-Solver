#pragma once
#include <vector>
#include <string>
#include <utility>

// Cell types
enum CellType {
    EMPTY  = 0,
    WALL   = 1,
    ROAD   = 2,   // weight 1
    MUD    = 3,   // weight 3
    WATER  = 4    // weight 5
};

struct Cell {
    int row, col;
    CellType type;
    int weight;   // movement cost for Dijkstra
};

class Maze {
public:
    Maze();

    // Generate a random maze of given dimensions
    void generate(int rows, int cols, double wallDensity = 0.30, bool weighted = false);

    // Load maze from a text file
    bool loadFromFile(const std::string& filename);

    // Save maze to a text file
    bool saveToFile(const std::string& filename) const;

    // Accessors
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }

    bool isWall(int r, int c) const;
    bool inBounds(int r, int c) const;
    int  getWeight(int r, int c) const;   // movement cost

    std::pair<int,int> getStart() const { return start_; }
    std::pair<int,int> getEnd()   const { return end_;   }

    void setStart(int r, int c) { start_ = {r, c}; }
    void setEnd  (int r, int c) { end_   = {r, c}; }

    // Print maze; optionally overlay a path
    void print(const std::vector<std::pair<int,int>>& path = {}) const;

    // Raw grid access
    const std::vector<std::vector<Cell>>& getGrid() const { return grid_; }

private:
    int rows_, cols_;
    std::vector<std::vector<Cell>> grid_;
    std::pair<int,int> start_, end_;

    void initGrid(int rows, int cols);
    void carvePath(int r, int c);           // recursive DFS maze carver
    char cellChar(int r, int c,
                  const std::vector<std::pair<int,int>>& path) const;
};
