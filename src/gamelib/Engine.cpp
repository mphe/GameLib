#include "gamelib/Engine.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/core/res/resources.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/components/geometry/PixelCollision.hpp"
#include "gamelib/components/geometry/AABBMask.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/components/update/QController.hpp"
#include "gamelib/components/update/CameraTracker.hpp"
#include "gamelib/components/update/ActorComponent.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/components/editor/PolygonBrushComponent.hpp"
#include "gamelib/components/editor/LineBrushComponent.hpp"

namespace gamelib
{
    void registerComponents(EntityFactory& factory)
    {
#define _REGISTER_COMPONENT(c) factory.addComponent<c>(c::name);
        _REGISTER_COMPONENT(PolygonCollider);
        _REGISTER_COMPONENT(AABB);
        _REGISTER_COMPONENT(PixelCollision);
        _REGISTER_COMPONENT(AABBMask);
        _REGISTER_COMPONENT(QPhysics);
        _REGISTER_COMPONENT(QConfig);
        _REGISTER_COMPONENT(QController);
        _REGISTER_COMPONENT(SpriteComponent);
        _REGISTER_COMPONENT(CameraTracker);
        _REGISTER_COMPONENT(PolygonShape);
        _REGISTER_COMPONENT(PolygonBrushComponent);
        _REGISTER_COMPONENT(LineBrushComponent);
        _REGISTER_COMPONENT(ActorComponent);
#undef _REGISTER_COMPONENT
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
        rendersystem.destroy();
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
        numrendered += rendersystem.render(target);

        if (_printstatus)
            // Insert blanks after \r for clean overwriting
            LOG_RAW("\rRendered ", numrendered, " objects with ",
                    // scene.getNumCameras(), " camera(s) at ",
                    std::round(1.f / _game->getFrametime()), " FPS");
    }
}
