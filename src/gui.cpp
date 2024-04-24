#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>

#include "board.h"
#include "button.h"
#include "gui.h"

void load_texture(sf::Texture& texture, std::string path) {
    if (!texture.loadFromFile(path))
        throw std::runtime_error("Image didn't load");
}

Assets load_assets() {
    Assets a;
    load_texture(a.digits, "images/digits.png");
    load_texture(a.mine, "images/mine.png");
    load_texture(a.flag, "images/flag.png");
    load_texture(a.digits, "images/digits.png");
    load_texture(a.tile_hidden, "images/tile_hidden.png");
    load_texture(a.tile_revealed, "images/tile_revealed.png");
    load_texture(a.mine, "images/mine.png");
    load_texture(a.face_win, "images/face_win.png");
    load_texture(a.face_lose, "images/face_lose.png");
    load_texture(a.face_happy, "images/face_happy.png");
    load_texture(a.debug, "images/debug.png");
    load_texture(a.play, "images/play.png");
    load_texture(a.pause, "images/pause.png");
    load_texture(a.leaderboard, "images/leaderboard.png");
    for (int i = 0; i < 8; i++) {
        std::stringstream ss;
        ss <<  "images/number_" << i+1 << ".png";
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

void render_menu(sf::RenderWindow& window, const std::string& name) {
    sf::Font font;
    if (!font.loadFromFile("font.ttf"))
        throw std::runtime_error("Font didn't load.");
    sf::Text welcome, name_label, name_select;

    welcome.setFont(font);
    name_label.setFont(font);
    name_select.setFont(font);

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

bool game_actions(sf::RenderWindow& window, Board& board, const GameButtons& btns) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            throw ClosedWindow();
        }
    }
    return true;
}

void render_game(sf::RenderWindow& window, const Board& board, const GameButtons& btns) {
    window.clear(sf::Color::White);
    btns.happy.display(window);
    btns.leaderboard.display(window);
    btns.play.display(window);
    btns.pause.display(window);
    btns.debug.display(window);
    window.display();
}
