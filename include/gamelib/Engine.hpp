#ifndef GAMELIB_ENGINE_HPP
#define GAMELIB_ENGINE_HPP

#include "gamelib/core/GameState.hpp"
#include "core/rendering/Scene.hpp"
#include "core/geometry/CollisionSystem.hpp"
#include "core/res/ResourceManager.hpp"
#include "core/ecs/EntityManager.hpp"
#include "core/ecs/EntityFactory.hpp"
#include "core/update/UpdateSystem.hpp"
#include "core/event/EventManager.hpp"
#include "core/input/InputSystem.hpp"

namespace gamelib
{
    class Engine : public GameState
    {
        public:
            Engine(bool printstatus = true);

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
            EventManager evmgr;
            InputSystem inputsys;

        private:
            Game* _game;
            bool _printstatus;
    };
}

#endif
