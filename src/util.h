#ifndef __UTIL__
#define __UTIL__

#include <cstdint>
#include <string>

struct GameConfig {
    uint32_t cols;    
    uint32_t rows;
    uint32_t mines;
};

GameConfig load_config(std::string path);

#endif // __UTIL__
