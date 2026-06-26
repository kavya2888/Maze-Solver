#include "Maze.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <climits>
#include <stdexcept>

// ─── helpers ────────────────────────────────────────────────────────────────

static const int DR[] = {-2,  2,  0,  0};
static const int DC[] = { 0,  0, -2,  2};

// Weight table for terrain types
static int terrainWeight(CellType t) {
    switch (t) {
        case MUD:   return 3;
        case WATER: return 5;
        default:    return 1;   // EMPTY / ROAD
    }
}

// ─── Maze ───────────────────────────────────────────────────────────────────

Maze::Maze() : rows_(0), cols_(0), start_{0,0}, end_{0,0} {}

void Maze::initGrid(int rows, int cols) {
    rows_ = rows;
    cols_ = cols;
    grid_.assign(rows, std::vector<Cell>(cols));
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            grid_[r][c] = {r, c, WALL, 1};
        }
}

// Recursive backtracker – carves passages in a grid of all-walls
void Maze::carvePath(int r, int c) {
    grid_[r][c].type = EMPTY;
    grid_[r][c].weight = 1;

    // Shuffle directions
    int order[] = {0, 1, 2, 3};
    for (int i = 3; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(order[i], order[j]);
    }

    for (int d : order) {
        int nr = r + DR[d];
        int nc = c + DC[d];
        if (nr >= 0 && nr < rows_ && nc >= 0 && nc < cols_ &&
            grid_[nr][nc].type == WALL)
        {
            // Carve wall between current and neighbour
            int wr = r + DR[d]/2;
            int wc = c + DC[d]/2;
            grid_[wr][wc].type = EMPTY;
            grid_[wr][wc].weight = 1;
            carvePath(nr, nc);
        }
    }
}

void Maze::generate(int rows, int cols, double wallDensity, bool weighted) {
    // Ensure odd dimensions for recursive-backtracker
    if (rows % 2 == 0) ++rows;
    if (cols % 2 == 0) ++cols;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    initGrid(rows, cols);

    // Carve maze from (1,1)
    carvePath(1, 1);

    // Optionally punch extra holes to increase open space / adjust density
    int extraHoles = static_cast<int>(rows * cols * (0.35 - wallDensity));
    for (int i = 0; i < extraHoles; ++i) {
        int r = 1 + std::rand() % (rows - 2);
        int c = 1 + std::rand() % (cols - 2);
        grid_[r][c].type = EMPTY;
        grid_[r][c].weight = 1;
    }

    // Place weighted terrain
    if (weighted) {
        CellType terrains[] = {MUD, WATER};
        for (int r = 0; r < rows_; ++r) {
            for (int c = 0; c < cols_; ++c) {
                if (grid_[r][c].type == EMPTY && std::rand() % 6 == 0) {
                    CellType t = terrains[std::rand() % 2];
                    grid_[r][c].type   = t;
                    grid_[r][c].weight = terrainWeight(t);
                }
            }
        }
    }

    // Force border to be walls (cleaner look)
    for (int r = 0; r < rows_; ++r) {
        grid_[r][0].type = WALL;
        grid_[r][cols_-1].type = WALL;
    }
    for (int c = 0; c < cols_; ++c) {
        grid_[0][c].type = WALL;
        grid_[rows_-1][c].type = WALL;
    }

    // Place start & end
    start_ = {1, 1};
    end_   = {rows_-2, cols_-2};

    // Ensure start and end are passable
    grid_[start_.first][start_.second].type   = EMPTY;
    grid_[start_.first][start_.second].weight = 1;
    grid_[end_.first][end_.second].type   = EMPTY;
    grid_[end_.first][end_.second].weight = 1;
}

bool Maze::loadFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) return false;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        if (!line.empty()) lines.push_back(line);
    }

    rows_ = static_cast<int>(lines.size());
    cols_ = rows_ > 0 ? static_cast<int>(lines[0].size()) : 0;
    grid_.assign(rows_, std::vector<Cell>(cols_));

    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_ && c < static_cast<int>(lines[r].size()); ++c) {
            char ch = lines[r][c];
            Cell& cell = grid_[r][c];
            cell.row = r; cell.col = c;
            switch (ch) {
                case '#': cell.type = WALL;  cell.weight = 0; break;
                case 'M': cell.type = MUD;   cell.weight = 3; break;
                case 'W': cell.type = WATER; cell.weight = 5; break;
                case 'S': cell.type = EMPTY; cell.weight = 1; start_ = {r, c}; break;
                case 'E': cell.type = EMPTY; cell.weight = 1; end_   = {r, c}; break;
                default:  cell.type = EMPTY; cell.weight = 1; break;
            }
        }
    }
    return true;
}

bool Maze::saveToFile(const std::string& filename) const {
    std::ofstream fout(filename);
    if (!fout.is_open()) return false;
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            if (std::make_pair(r,c) == start_)      fout << 'S';
            else if (std::make_pair(r,c) == end_)   fout << 'E';
            else {
                switch (grid_[r][c].type) {
                    case WALL:  fout << '#'; break;
                    case MUD:   fout << 'M'; break;
                    case WATER: fout << 'W'; break;
                    default:    fout << '.'; break;
                }
            }
        }
        fout << '\n';
    }
    return true;
}

bool Maze::isWall(int r, int c) const {
    return !inBounds(r, c) || grid_[r][c].type == WALL;
}

bool Maze::inBounds(int r, int c) const {
    return r >= 0 && r < rows_ && c >= 0 && c < cols_;
}

int Maze::getWeight(int r, int c) const {
    if (!inBounds(r, c)) return INT_MAX;
    return grid_[r][c].weight;
}

char Maze::cellChar(int r, int c,
                    const std::vector<std::pair<int,int>>& path) const {
    auto pos = std::make_pair(r, c);
    if (pos == start_) return 'S';
    if (pos == end_)   return 'E';

    // Is this cell on the path?
    for (auto& p : path)
        if (p == pos) return '*';

    switch (grid_[r][c].type) {
        case WALL:  return '#';
        case MUD:   return 'M';
        case WATER: return 'W';
        default:    return '.';
    }
}

void Maze::print(const std::vector<std::pair<int,int>>& path) const {
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c)
            std::cout << cellChar(r, c, path);
        std::cout << '\n';
    }
}
