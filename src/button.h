#ifndef __BUTTON__
#define __BUTTON__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

class Button {
private:
    float x, y;
    sf::Sprite sprite;

public:
    Button() : x(0), y(0), sprite() {}

    void set_pos(float x, float y) {
        // std::cout << "setting button position\n";
        this->x = x;
        this->y = y;
        sprite.setPosition(x, y);
    }

    void set_texture(const sf::Texture& texture) {
        // std::cout << "setting button texture\n";
        sprite.setTexture(texture);
    }

    void display(sf::RenderWindow& window) const {
        // std::cout << "displaying button\n";
        window.draw(sprite);
    }

    bool is_hovered(const sf::Vector2i& pos) const {
        return sprite.getGlobalBounds().contains(
            static_cast<sf::Vector2f>(pos)
        );
    }
};

#endif // __BUTTON__
