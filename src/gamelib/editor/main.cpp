#include "gamelib/core/Game.hpp"
#include "gamelib/editor/editor/Editor.hpp"

using namespace gamelib;

int main(int argc, char *argv[])
{
    Game game;
    game.init();
    game.loadFromFile("assets/game.json");
    game.pushState(GameStatePtr(new Editor()));
    game.run();
    game.destroy(); // optional, thanks to RAII

    return 0;
}
