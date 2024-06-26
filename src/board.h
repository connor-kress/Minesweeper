#ifndef __BOARD__
#define __BOARD__

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <tuple>
#include <set>
#include <utility>
#include <vector>

#include "util.h"
// template <typename T>
// inline bool contains(const std::vector<T>& vec, const T& item) {
//     return std::find(vec.begin(), vec.end(), item) != vec.end();
// }

struct Cell {
    bool revealed = false;
    bool flagged = false;
    bool is_mine = false;
};

class Board {
public:
    int rows;
    int cols;

    Board(GameConfig c) : rows(c.rows), cols(c.cols), mines(c.mines) {
        data = new Cell*[rows];
        for (int i = 0; i < rows; i++)
            data[i] = new Cell[cols];
        populate();
    }

    ~Board() {
        for (int i = 0; i < rows; i++)
            delete[] data[i];
        delete[] data;
    }

    void reset() {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                data[r][c].revealed = false;
                data[r][c].flagged = false;
                data[r][c].is_mine = false;
            }
        }
        populate();
    }

    bool is_complete() const {
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                if (!data[r][c].is_mine && !data[r][c].revealed)
                    return false;
        return true;
    }

    int flags_remaining() const {
        int count = mines;
        for (int r = 0; r < rows; r++)
            for (int c = 0; c < cols; c++)
                if (data[r][c].flagged)
                    count--;
        return count;
    }

    std::set<std::pair<int, int>> adjacent(std::pair<int, int> idx) const {
        int row, col;
        std::tie(row, col) = idx;
        if (row < 0 || row >= rows || col < 0 || col >= cols)
            throw std::runtime_error("Out of bounds error.");
        auto is_valid = [=](int r, int c) {
            if (r == 0 && c == 0) return false;
            r += row; c += col;
            return !(r < 0 || r >= rows || c < 0 || c >= cols);
        };
        std::set<std::pair<int, int>> adjacents;
        for (int r = -1; r <= 1; r++)
            for (int c = -1; c <= 1; c++)
                if (is_valid(r, c))
                    adjacents.insert({ row+r, col+c });
        return adjacents;
    }

    int count(std::pair<int, int> idx) const {
        int count = 0;
        for (auto& idx : adjacent(idx))
            if (at(idx)->is_mine)
                count++;
        return count;
    }

    int count(int row, int col) const {
        return count({row, col});
    }

    inline Cell* at(const std::pair<int, int>& idx) {
        return &data[idx.first][idx.second];
    }

    inline const Cell* at(const std::pair<int, int>& idx) const {
        return &data[idx.first][idx.second];
    }

    inline Cell* at(int row, int col) {
        return &data[row][col];
    }

    inline const Cell* at(int row, int col) const {
        return &data[row][col];
    }

    void flag(int row, int col) {
        data[row][col].flagged = data[row][col].flagged ? false : true;
    }

   bool click(int row, int col) {
        if (at(row, col)->revealed || at(row, col)->flagged)
            return true;
        if (at(row, col)->is_mine)
            return false;
        if (count(row, col) == 0)
            propogate({row, col});
        else
            at(row, col)->revealed = true;
        return true;
    }

private:
    Cell** data;
    int mines;

    void populate() {
        int remaining = mines;
        while (remaining) {
            int r = std::rand() % rows;
            int c = std::rand() % cols;
            if (data[r][c].is_mine) continue;
            data[r][c].is_mine = true;
            remaining--;
        }
    }

    void propogate(std::pair<int, int> start) {
        std::set<std::pair<int, int>> curr, next;
        curr.insert(start);
        at(start)->revealed = true;
        while (curr.size()) {
            for (auto idx : curr) {
                for (auto adj : adjacent(idx)) {
                    if (at(adj)->revealed || at(adj)->is_mine)
                        continue;
                    at(adj)->revealed = true;
                    if (count(adj) == 0)
                        next.insert(adj);
                }
            }
            curr = next;
            next.clear();
        }
    }
};

#endif // __BOARD__
