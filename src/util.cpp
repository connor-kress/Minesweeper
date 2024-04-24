#include <fstream>
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
