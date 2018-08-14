#include "gamelib/Engine.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/core/res/resources.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/components/geometry/PixelCollision.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/components/update/QController.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/components/update/CameraTracker.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/components/editor/BrushComponent.hpp"

namespace gamelib
{
    void registerComponents(EntityFactory& factory)
    {
        factory.addComponent<Polygon>(Polygon::name);
        factory.addComponent<AABB>(AABB::name);
        factory.addComponent<PixelCollision>(PixelCollision::name);
        factory.addComponent<QPhysics>(QPhysics::name);
        factory.addComponent<QConfig>(QConfig::name);
        factory.addComponent<QController>(QController::name);
        factory.addComponent<SpriteComponent>(SpriteComponent::name);
        factory.addComponent<CameraTracker>(CameraTracker::name);
        factory.addComponent<PolygonShape>(PolygonShape::name);
        factory.addComponent<BrushComponent>(BrushComponent::name);
    }


    Engine::Engine(bool printstatus) :
        _game(nullptr),
        _printstatus(printstatus)
    {
        registerComponents(entfactory);
        registerPredefLoaders(resmgr);
    }

    bool Engine::init(Game* game)
    {
        _game = game;
        inputsys.setWindow(game->getWindow());
        return true;
    }

    void Engine::quit()
    {
        _game = nullptr;
        entmgr.clear();
        scene.destroy();
        colsys.destroy();
        updatesystem.destroy();
        resmgr.clear();
        entfactory.clear();
        evmgr.clear();
        inputsys.beginFrame();
    }


    void Engine::update(float elapsed)
    {
        updatesystem.update(elapsed);
        scene.update(elapsed);
        evmgr.update();
    }

    void Engine::render(sf::RenderTarget& target)
    {
        auto numrendered = scene.render(target);

        if (_printstatus)
            // Insert blanks after \r for clean overwriting
            LOG_RAW("\rRendered ", numrendered, " objects with ",
                    scene.getCameraCount(), " camera(s) at ",
                    std::round(1.f / _game->getFrametime()), " FPS");
    }
}
