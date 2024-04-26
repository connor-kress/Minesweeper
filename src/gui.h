#ifndef __GUI__
#define __GUI__

#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>
#include <stdexcept>
#include <string>

#include "board.h"
#include "button.h"
#include "util.h"

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

struct ClosedWindow {};

enum GameMode {
    Active,
    Won,
    Lost,
};

struct GameState {
    bool debug = false;
    bool paused = false;
    bool leaderboard_open = false;
    GameMode mode = GameMode::Active;
    TimePoint start_time;

    GameState() {
        start_time = std::chrono::steady_clock::now();
    }
};

struct Assets {
    sf::Font font;
    sf::Texture mine, tile_hidden, tile_revealed,
        face_happy, face_win, face_lose, digits,
        debug, pause, play, leaderboard, flag;
    std::array<sf::Texture, 8> numbers;
};

Assets load_assets();

struct GameButtons {
    Button happy, win, lose, debug, pause, play, leaderboard;
};

struct GameElements {
    GameButtons btns;
    std::array<sf::Sprite, 11> digits;
    std::array<sf::Sprite, 8> numbers;
    sf::Sprite flag, mine, tile_hidden, tile_revealed;
};

GameElements get_elements(const Board& board, const Assets& a);

void set_text_center(sf::Text &text, float x, float y);

bool menu_actions(sf::RenderWindow& window, std::string& name);

void render_menu(sf::RenderWindow& window, const std::string& name, const Assets& a);

void game_actions(sf::RenderWindow& window, Board& board, GameElements& elements, GameState& state);

void render_game(sf::RenderWindow& window, const Board& board, GameElements& elements, GameState& state);

void render_leaderboard(sf::RenderWindow& window, const std::vector<std::pair<std::string, int>>& leaderboard, const Assets& a);

#endif // __GUI__
