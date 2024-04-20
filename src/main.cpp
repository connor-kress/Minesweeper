#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

void set_text_center(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
    textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

bool render_menu(sf::RenderWindow& window, std::string& name) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            std::cout << "Keypress detected\n";
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
    name_select.setString(name);

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
    return true;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
    std::string name;
    while (window.isOpen()) {
        if (!render_menu(window, name)) {
            break;
        }
    }

    return 0;
}
