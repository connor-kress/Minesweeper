#ifndef __UTIL__
#define __UTIL__

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct GameConfig {
    uint32_t cols;    
    uint32_t rows;
    uint32_t mines;
};

GameConfig load_config(std::string path);

std::vector<std::pair<std::string, int>> load_leaderboard(std::string path);

void write_leaderboard(const std::vector<std::pair<std::string, int>>& data, std::string path);

std::string format_time(int total_secs);

#endif // __UTIL__
