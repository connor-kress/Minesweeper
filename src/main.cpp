#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "board.h"
#include "gui.h"
#include "util.h"

bool did_quit(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event))
        if (event.type == sf::Event::Closed)
            return true;
    return false;
}

void handle_win(const GameState& state, std::vector<std::pair<std::string, int>>& leaderboard, std::string& name) {
    auto curr_time = std::chrono::steady_clock::now();
    int total_secs = std::chrono::duration_cast<std::chrono::seconds>(
        curr_time - state.start_time
    ).count();
    leaderboard.push_back({name + '*', total_secs});
    std::sort(leaderboard.begin(), leaderboard.end(),
              [](std::pair<std::string, int>& item1, std::pair<std::string, int>& item2) {
                  return item1.second < item2.second;
              });
    write_leaderboard(leaderboard, "./files/leaderboard.txt");
}

int main() {
    auto config = load_config("./files/config.cfg");
    auto assets = load_assets();
    auto leaderboard = load_leaderboard("./files/leaderboard.txt");

    sf::RenderWindow window(
            sf::VideoMode(32*config.cols, 32*config.rows + 100),
            "Minesweeper",
            sf::Style::Close
    );
    sf::RenderWindow lb_window(
            sf::VideoMode(16*config.cols, 16*config.rows + 50),
            "Leaderboard",
            sf::Style::Close
    );
    lb_window.setVisible(false);
    bool lb_window_visible = false;
    std::string name;
    try {

    while (menu_actions(window, name))
        render_menu(window, name, assets);

    Board board(config);
    auto elements = get_elements(board, assets);
    GameState state;
    bool just_won = true;
    while (true) {
        if (state.mode == GameMode::Won && just_won) {
            just_won = false;
            handle_win(state, leaderboard, name);
        }
        if (state.mode == GameMode::Active && !just_won) {
            just_won = true;
        }
        if (state.leaderboard_open) {
            if (!lb_window_visible) {
                lb_window.setVisible(true);
                lb_window_visible = true;
            }
            if (did_quit(window))
                throw ClosedWindow();
            if (did_quit(lb_window)) {
                lb_window.setVisible(false);
                lb_window_visible = false;
                state.leaderboard_open = false;
            }
            render_leaderboard(lb_window, leaderboard, assets);
        } else {
            game_actions(window, board, elements, state);
            render_game(window, board, elements, state);
        }
    }

    } catch (ClosedWindow&) {
        window.close();
        lb_window.close();
    }

    return 0;
}
