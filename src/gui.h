#ifndef __GUI__
#define __GUI__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <stdexcept>
#include <string>

#include "board.h"
#include "button.h"
#include "util.h"

struct ClosedWindow {};

struct Assets {
    sf::Texture mine, tile_hidden, tile_revealed,
        face_happy, face_win, face_lose, digits,
        debug, pause, play, leaderboard, flag;
    std::array<sf::Texture, 8> numbers;
};

Assets load_assets();

struct GameButtons {
    Button happy, win, lose, debug, pause, play, leaderboard;
};

GameButtons get_buttons(const Board& board, const Assets& a);

void set_text_center(sf::Text &text, float x, float y);

bool menu_actions(sf::RenderWindow& window, std::string& name);

void render_menu(sf::RenderWindow& window, const std::string& name);

bool game_actions(sf::RenderWindow& window, Board& board, const GameButtons& btns);

void render_game(sf::RenderWindow& window, const Board& board, const GameButtons& btns);

#endif // __GUI__
