#include "gamelib/Engine.hpp"
#include "gamelib/core/res/resources.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/components/geometry/PixelCollision.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/components/update/QController.hpp"
#include "gamelib/components/update/SpriteComponent.hpp"
#include "gamelib/components/update/CameraTracker.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"

namespace gamelib
{
    Engine::Engine()
    {
        entfactory.addComponent<Polygon>(Polygon::name);
        entfactory.addComponent<AABB>(AABB::name);
        entfactory.addComponent<PixelCollision>(PixelCollision::name);
        entfactory.addComponent<QPhysics>(QPhysics::name);
        entfactory.addComponent<QController>(QController::name);
        entfactory.addComponent<SpriteComponent>(SpriteComponent::name);
        entfactory.addComponent<CameraTracker>(CameraTracker::name);
        entfactory.addComponent<PolygonShape>(PolygonShape::name);

        registerPredefLoaders(resmgr);
    }

    bool Engine::init(Game* game)
    {
        return true;
    }

    void Engine::quit()
    {
        entmgr.clear();
        scene.destroy();
        colsys.destroy();
        updatesystem.destroy();
        resmgr.clear();
        entfactory.clear();
    }


    void Engine::update(float elapsed)
    {
        updatesystem.update(elapsed);
        scene.update(elapsed);
    }

    void Engine::render(sf::RenderTarget& target)
    {
        scene.render(target);
    }
}
