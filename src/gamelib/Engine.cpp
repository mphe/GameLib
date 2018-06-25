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

namespace gamelib
{
    Engine::Engine(bool printstatus) :
        _game(nullptr),
        _printstatus(printstatus)
    {
        entfactory.addComponent<Polygon>(Polygon::name);
        entfactory.addComponent<AABB>(AABB::name);
        entfactory.addComponent<PixelCollision>(PixelCollision::name);
        entfactory.addComponent<QPhysics>(QPhysics::name);
        entfactory.addComponent<QConfig>(QConfig::name);
        entfactory.addComponent<QController>(QController::name);
        entfactory.addComponent<SpriteComponent>(SpriteComponent::name);
        entfactory.addComponent<CameraTracker>(CameraTracker::name);
        entfactory.addComponent<PolygonShape>(PolygonShape::name);

        registerPredefLoaders(resmgr);
    }

    bool Engine::init(Game* game)
    {
        _game = game;
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
