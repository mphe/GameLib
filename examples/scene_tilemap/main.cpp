#include "gamelib/Game.hpp"
#include "MainState.hpp"

using namespace gamelib;

int main(int argc, char *argv[])
{
    Game game;
    game.init();
    game.loadFromFile("assets/game.json");
    game.pushState(GameStatePtr(new MainState()));
    game.run();
    game.destroy(); // optional, thanks to RAII

    return 0;
}
