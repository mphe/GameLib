#ifndef GAMELIB_QCONTROLLER_HPP
#define GAMELIB_QCONTROLLER_HPP

#include "gamelib/ecs/UpdateComponent.hpp"

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

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

        private:
            auto _refresh() -> void;

        public:
            float accelerate;
            float airAccelerate;
            float maxspeed;
            float jumpspeed;
            QPhysics* qphys;

        private:
            bool _canjump;
    };
}

#endif
