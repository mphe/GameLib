#include "gamelib/Engine.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/editor/Editor.hpp"
#include <cstring>

using namespace gamelib;

int main(int argc, char *argv[])
{
    Game game;
    GameStatePtr engineState(new Engine(false));
    auto engine = static_cast<Engine*>(engineState.get());

    engine->resmgr.loadFromFile("assets/res.json");

    auto& cam = engine->scene.addCamera();
    cam.loadFromFile("assets/editorcam.json");
    engine->scene.setDefaultCamera(cam.getID());

    game.loadFromFile("assets/editor.json");
    game.init();
    game.pushState(std::move(engineState));
    game.pushState(GameStatePtr(new Editor()));

    if (argc > 1 && strlen(argv[1]) > 0)
        static_cast<Editor*>(game.pullState())->load(argv[1]);

    game.run();
    game.destroy();

    return 0;
}
