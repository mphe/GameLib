#ifndef GAMELIB_ENGINE_HPP
#define GAMELIB_ENGINE_HPP

#include "gamelib/core/GameState.hpp"
#include "core/rendering/Scene.hpp"
#include "core/geometry/CollisionSystem.hpp"
#include "core/res/ResourceManager.hpp"
#include "core/ecs/EntityManager.hpp"
#include "core/ecs/EntityFactory.hpp"
#include "core/update/UpdateSystem.hpp"
#include "editor/editor/Editor.hpp"

namespace gamelib
{
    class Engine : public GameState
    {
        public:
            Engine();

            auto init(Game* game) -> bool;
            auto quit()           -> void;

            auto update(float elapsed)            -> void;
            auto render(sf::RenderTarget& target) -> void;

        public:
            Scene scene;
            CollisionSystem colsys;
            ResourceManager resmgr;
            EntityManager entmgr;
            EntityFactory entfactory;
            UpdateSystem updatesystem;
    };
}

#endif
