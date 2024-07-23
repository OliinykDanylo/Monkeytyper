#include "cmake-build-debug/game.h"
#include "cmake-build-debug/start.h"
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "MonkeyTyper");
    Game game(window);
    Start start(game);
    start.update();
    return 0;
}