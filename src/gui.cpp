#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "board.h"
#include "button.h"
#include "gui.h"
#include "util.h"

void load_texture(sf::Texture& texture, std::string path) {
    if (!texture.loadFromFile(path))
        throw std::runtime_error("Image didn't load");
}

Assets load_assets() {
    Assets a;
    if (!a.font.loadFromFile("./files/font.ttf"))
        throw std::runtime_error("Font didn't load.");
    load_texture(a.digits, "./files/images/digits.png");
    load_texture(a.mine, "./files/images/mine.png");
    load_texture(a.flag, "./files/images/flag.png");
    load_texture(a.digits, "./files/images/digits.png");
    load_texture(a.tile_hidden, "./files/images/tile_hidden.png");
    load_texture(a.tile_revealed, "./files/images/tile_revealed.png");
    load_texture(a.mine, "./files/images/mine.png");
    load_texture(a.face_win, "./files/images/face_win.png");
    load_texture(a.face_lose, "./files/images/face_lose.png");
    load_texture(a.face_happy, "./files/images/face_happy.png");
    load_texture(a.debug, "./files/images/debug.png");
    load_texture(a.play, "./files/images/play.png");
    load_texture(a.pause, "./files/images/pause.png");
    load_texture(a.leaderboard, "./files/images/leaderboard.png");
    for (int i = 0; i < 8; i++) {
        std::stringstream ss;
        ss <<  "./files/images/number_" << i+1 << ".png";
        load_texture(a.numbers[i], ss.str());
    }
    return a;
}

GameButtons get_buttons(const Board& board, const Assets& a) {
    int cols = board.cols;
    int rows = board.rows;
    GameButtons btns;

    btns.happy.set_texture(a.face_happy);
    btns.happy.set_pos(cols/2.f * 32.f - 32.f, 32.f * (rows + 0.5f));

    btns.win.set_texture(a.face_win);
    btns.win.set_pos(cols/2.f * 32.f - 32.f, 32.f * (rows + 0.5f));

    btns.lose.set_texture(a.face_lose);
    btns.lose.set_pos(cols/2.f * 32.f - 32.f, 32.f * (rows + 0.5f));

    btns.debug.set_texture(a.debug);
    btns.debug.set_pos(cols * 32.f - 304.f, 32.f * (rows + 0.5f));

    btns.play.set_texture(a.play);
    btns.play.set_pos(cols * 32.f - 240.f, 32.f * (rows + 0.5f));

    btns.pause.set_texture(a.pause);
    btns.pause.set_pos(cols * 32.f - 240.f, 32.f * (rows + 0.5f));

    btns.leaderboard.set_texture(a.leaderboard);
    btns.leaderboard.set_pos(cols * 32.f - 176.f, 32.f * (rows + 0.5f));

    return btns;
}

GameElements get_elements(const Board& board, const Assets& a) {
    GameElements elements;
    elements.btns = get_buttons(board, a);
    for (int i = 0; i < 11; i++) {
        elements.digits[i].setTexture(a.digits);
        elements.digits[i].setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
    }
    for (int i = 0; i < 8; i++) {
        elements.numbers[i].setTexture(a.numbers[i]);
    }
    elements.flag.setTexture(a.flag);
    elements.mine.setTexture(a.mine);
    elements.tile_hidden.setTexture(a.tile_hidden);
    elements.tile_revealed.setTexture(a.tile_revealed);
    return elements;
}


void set_text_center(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

bool menu_actions(sf::RenderWindow& window, std::string& name) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            throw ClosedWindow();
        } else if (event.type == sf::Event::KeyPressed) {
            // std::cout << "Keypress detected\n";
            if (event.key.code == sf::Keyboard::Enter) {
                if (name.size() == 0) continue;
                return false;
            } else if (event.key.code == sf::Keyboard::BackSpace) {
                if (name.size()) name.pop_back();
            }
        } else if (event.type == sf::Event::TextEntered) {
            if (!std::isalpha(event.text.unicode)) continue;
            if (name.size() >= 10) continue;
            char c = static_cast<char>(event.text.unicode);
            name.push_back(name.size() ? std::tolower(c) : std::toupper(c));
        }
    }
    return true;
}

void render_menu(sf::RenderWindow& window, const std::string& name, const Assets& a) {
    sf::Text welcome, name_label, name_select;

    welcome.setFont(a.font);
    name_label.setFont(a.font);
    name_select.setFont(a.font);

    welcome.setFillColor(sf::Color::White);
    name_label.setFillColor(sf::Color::White);
    name_select.setFillColor(sf::Color::White);

    welcome.setCharacterSize(24);
    name_label.setCharacterSize(20);
    name_select.setCharacterSize(18);

    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    name_label.setStyle(sf::Text::Bold);
    name_select.setStyle(sf::Text::Bold);

    welcome.setString("WELCOME TO MINESWEEPER!");
    name_label.setString("Enter your name:");
    name_select.setString(name + '|');

    float x_c = window.getSize().x / 2.0;
    float y_c = window.getSize().y / 2.0;
    set_text_center(welcome, x_c, y_c - 150.0);
    set_text_center(name_label, x_c, y_c - 75.0);
    set_text_center(name_select, x_c, y_c - 45.0);

    window.clear(sf::Color::Blue);

    window.draw(welcome);
    window.draw(name_label);
    window.draw(name_select);

    window.display();
}

void game_actions(sf::RenderWindow& window, Board& board, GameElements& elements, GameState& state) {
    auto btns = elements.btns;
    // bool update = false;
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            throw ClosedWindow();
        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // update = true;
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            // std::cout << "Clicked (" << pos.x << ", " << pos.y << ")\n";
            if (state.mode == GameMode::Active && btns.debug.is_hovered(pos)) {
                std::cout << "DEBUG\n";
                state.debug = state.debug ? false : true;
            } else if (state.mode == GameMode::Active && btns.pause.is_hovered(pos)) {
                if (state.paused) {
                    std::cout << "PLAY\n";
                    state.paused = false;
                } else {
                    std::cout << "PAUSE\n";
                    state.paused = true;
                }
            } else if (btns.leaderboard.is_hovered(pos)) {
                std::cout << "LEADERBOARD\n";
                state.leaderboard_open = true;
            } else if (btns.happy.is_hovered(pos)) {
                std::cout << "RESET\n";
                board.reset();
                state.start_time = std::chrono::steady_clock::now();
                state.mode = GameMode::Active;
            } else if (state.mode == GameMode::Active) { // possible board click
                if (pos.x > board.cols*32 || pos.y > board.rows*32)
                    continue;
                int col = pos.x / 32;
                int row = pos.y / 32;
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!board.click(row, col)) {
                        std::cout << "BOOOOOM\n";
                        state.mode = GameMode::Lost;
                        render_game(window, board, elements, state);
                    }
                    if (board.is_complete()) {
                        std::cout << "Congratulations\n";
                        state.mode = GameMode::Won;
                        render_game(window, board, elements, state);
                        state.leaderboard_open = true;
                    }
                } else if (event.mouseButton.button == sf::Mouse::Right) {
                    board.flag(row, col);
                }
            }
        }
    }
    // return update;
}

void draw_tiles(sf::RenderWindow& window, const Board& board, GameElements& elements, GameState& state) {
    for (int r = 0; r < board.rows; r++) {
        for (int c = 0; c < board.cols; c++) {
            uint32_t x = c * 32;
            uint32_t y = r * 32;
            sf::Sprite* bottom = nullptr;
            sf::Sprite* top = nullptr;
            if (state.paused) {
                bottom = &elements.tile_revealed;
            } else if (board.at(r, c)->revealed) {
                bottom = &elements.tile_revealed;
                int count = board.count(r, c);
                if (count != 0)
                    top = &elements.numbers[count-1];
            } else {
                bottom = &elements.tile_hidden;
                if (board.at(r, c)->flagged)
                    top = &elements.flag;
                if ((state.debug || state.mode == GameMode::Lost) && board.at(r, c)->is_mine)
                    top = &elements.mine;
            }
            bottom->setPosition(x, y);
            window.draw(*bottom);
            if (top) {
                top->setPosition(x, y);
                window.draw(*top);
            }
        }
    }
}

void draw_counts(sf::RenderWindow& window, const Board& board, GameElements& elements, GameState& state) {
    int rows = board.rows;
    int cols = board.cols;
    int i = 0;
    for (char c : std::to_string(board.flags_remaining())) {
        if (c == '-') {
            elements.digits[10].setPosition(12.f, 32.f * (rows + 0.5f) + 16.f);
            window.draw(elements.digits[10]);
            continue;
        }
        int idx = c - '0';
        elements.digits[idx].setPosition(33.f + 21.f*i, 32.f * (rows + 0.5f) + 16.f);
        window.draw(elements.digits[idx]);
        i++;
    }

    auto curr_time = std::chrono::steady_clock::now();
    int total_secs = std::chrono::duration_cast<std::chrono::seconds>(
            curr_time - state.start_time
    ).count();
    // std::cout << "total_secs = " << total_secs << '\n'; 
    std::string mins = std::to_string(total_secs / 60);
    if (mins.size() == 1) mins = std::string("0") + mins;
    std::string secs = std::to_string(total_secs % 60);
    if (secs.size() == 1) secs = std::string("0") + secs;
    for (int i = 0; i <= 1; i++) {
        int idx = mins.at(i) - '0';
        elements.digits[idx].setPosition(32.f*cols - 97.f + 21.f*i, 32.f * (rows + 0.5) + 16);
        window.draw(elements.digits[idx]);
    }
    for (int i = 0; i <= 1; i++) {
        int idx = secs.at(i) - '0';
        elements.digits[idx].setPosition(32.f*cols - 54.f + 21.f*i, 32.f * (rows + 0.5) + 16);
        window.draw(elements.digits[idx]);
    }
}

void render_game(sf::RenderWindow& window, const Board& board, GameElements& elements, GameState& state) {
    auto btns = elements.btns;
    window.clear(sf::Color::White);
    if (state.mode == GameMode::Active) {
        btns.happy.display(window);
    } else if (state.mode == GameMode::Won) {
        btns.win.display(window);
    } else if (state.mode == GameMode::Lost) {
        btns.lose.display(window);
    }
    if (state.paused)
        btns.play.display(window);
    else
        btns.pause.display(window);
    btns.debug.display(window);
    btns.leaderboard.display(window);
    draw_counts(window, board, elements, state);
    draw_tiles(window, board, elements, state);
    window.display();
}

void render_leaderboard(sf::RenderWindow& window, const std::vector<std::pair<std::string, int>>& leaderboard, const Assets& a) {
    sf::Text title, row;
    title.setFont(a.font);
    row.setFont(a.font);

    title.setFillColor(sf::Color::White);
    row.setFillColor(sf::Color::White);

    title.setCharacterSize(20);
    row.setCharacterSize(18);

    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    row.setStyle(sf::Text::Bold);

    title.setString("LEADERBOARD");

    float x_c = window.getSize().x / 2.f;
    float y_c = window.getSize().y / 2.f;
    std::stringstream ss;
    int i = 1;
    for (const auto& item : leaderboard) {
        if (i > 5) break;
        ss << i << ".\t" << format_time(item.second)
           << '\t' << item.first << "\n\n";
        i++;
    }
    row.setString(ss.str());

    set_text_center(title, x_c, y_c - 120.f);
    set_text_center(row, x_c, y_c + 20.f);

    window.clear(sf::Color::Blue);

    window.draw(title);
    window.draw(row);

    window.display();
}
