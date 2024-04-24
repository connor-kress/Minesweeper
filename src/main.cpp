#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "board.h"
#include "gui.h"
#include "util.h"

int main() {
    GameConfig config = load_config("config.cfg");
    Assets a = load_assets();

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
    std::string name;
    try {

    while (menu_actions(window, name))
        render_menu(window, name);

    Board board(config);
    GameButtons btns = get_buttons(board, a);

    while (game_actions(window, board, btns)) {
        // sf::Sprite thing;
        // sf::Texture t;
        // if (!t.loadFromFile("images/mine.png"))
        //     throw 6l;
        // thing.setTexture(a.mine);
        // window.clear(sf::Color::White);
        // window.draw(thing);
        // window.display();
        render_game(window, board, btns);
    }

    } catch (ClosedWindow&) {
        window.close();
    }

    return 0;
}
