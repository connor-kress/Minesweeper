#ifndef __BOARD__
#define __BOARD__

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <tuple>
#include <set>
#include <utility>
#include <vector>

// template <typename T>
// inline bool contains(const std::vector<T>& vec, const T& item) {
//     return std::find(vec.begin(), vec.end(), item) != vec.end();
// }

struct Cell {
    bool revieled = false;
    bool is_mine = false;
};

class Board {
public:
    Board(int r, int c, int m) : rows(r), cols(c), mines(m) {
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

    int count(std::pair<int, int> idx) {
        int count = 0;
        for (auto& idx : adjacent(idx))
            if (at(idx)->is_mine)
                count++;
        return count;
    }

private:
    Cell** data;
    int rows;
    int cols;
    int mines;

    inline Cell* at(const std::pair<int, int>& idx) {
        return &data[idx.first][idx.second];
    }

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
        at(start)->revieled = true;
        while (curr.size()) {
            for (auto idx : curr) {
                for (auto adj : adjacent(idx)) {
                    if (at(adj)->revieled)
                        continue;
                    at(adj)->revieled = true;
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
