#include "gamelib/Engine.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/editor/editor/Editor.hpp"

#include "gamelib/editor/components/BrushComponent.hpp"
#include "gamelib/editor/components/PolygonShape.hpp"

using namespace gamelib;

int main(int argc, char *argv[])
{
    Game game;
    GameStatePtr engineState(new Engine());
    auto engine = static_cast<Engine*>(engineState.get());

    engine->entfactory.addComponent<PolygonShape>(PolygonShape::name);
    engine->entfactory.addComponent<BrushComponent>(BrushComponent::name);

    engine->resmgr.loadFromFile("assets/res.json");

    engine->scene.addCamera();
    engine->scene.getCamera(0)->loadFromFile("assets/editorcam.json");
    engine->scene.setDefaultCamera(0);

    game.init();
    game.loadFromFile("assets/editor.json");
    game.pushState(std::move(engineState));
    game.pushState(GameStatePtr(new Editor()));
    game.run();
    game.destroy();

    return 0;
}
