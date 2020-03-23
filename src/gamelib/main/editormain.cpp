#include "gamelib/Engine.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/editor/Editor.hpp"
#include "gamelib/imgui/iconfont.hpp"
#include <cstring>

using namespace gamelib;

int main(int argc, char *argv[])
{
    Game game;

    game.loadFromFile("assets/editor.json");
    game.init();

    GameStatePtr engineState(new Engine(false));
    auto engine = static_cast<Engine*>(engineState.get());
    engine->resmgr.loadFromFile("assets/res.json");

    game.pushState(std::move(engineState));
    game.pushState(GameStatePtr(new Editor()));

    setupIconFont();

    if (argc > 1 && strlen(argv[1]) > 0)
        static_cast<Editor*>(game.pullState())->load(argv[1]);

    game.run();
    game.destroy();

    return 0;
}
