#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "util.h"

GameConfig load_config(std::string path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open config file.");
    GameConfig config;
    std::string cols_str, rows_str, mines_str;
    std::getline(file, cols_str);
    std::getline(file, rows_str);
    std::getline(file, mines_str);
    config.cols = std::stoi(cols_str);
    config.rows = std::stoi(rows_str);
    config.mines = std::stoi(mines_str);
    file.close();
    return config;
}

std::vector<std::pair<std::string, int>> load_leaderboard(std::string path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open leaderboard data file.");
    std::vector<std::pair<std::string, int>> data;
    std::string line, mins, secs, name;
    char x;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::getline(ss, mins, ':');
        std::getline(ss, secs, ',');
        ss.ignore(1);
        std::getline(ss, name);
        data.push_back({name, std::stoi(secs) + std::stoi(mins)*60});
    }
    file.close();
    return data;
}

void write_leaderboard(const std::vector<std::pair<std::string, int>>& data, std::string path) {
    std::ofstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open leaderboard data file.");
    for (const auto& item : data) {
        std::string name = item.first;
        if (name.at(name.size()-1) == '*')
            name = name.substr(0, name.size()-1);
        file << format_time(item.second) << ", " << name << '\n';
    }
    file.close();
}
std::string format_time(int total_secs) {
    std::string mins = std::to_string(total_secs / 60);
    if (mins.size() == 1) mins = std::string("0") + mins;
    std::string secs = std::to_string(total_secs % 60);
    if (secs.size() == 1) secs = std::string("0") + secs;
    return mins + ':' + secs;
}

