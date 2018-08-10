#ifndef GAMELIB_QCONTROLLER_HPP
#define GAMELIB_QCONTROLLER_HPP

#include "gamelib/core/ecs/UpdateComponent.hpp"

namespace gamelib
{
    class QPhysics;

    class QController : public UpdateComponent
    {
        public:
            constexpr static const char* name = "QController";

        public:
            QController();

            auto update(float elapsed) -> void;

        private:
            auto _refresh() -> void;

        public:
            float accelerate;
            float airAccelerate;
            float maxspeed;
            float jumpspeed;
            float jumpDecelerate;
            QPhysics* phys;
            bool slopejumps;

        private:
            bool _canjump;
            bool _jumping;
    };
}

#endif
