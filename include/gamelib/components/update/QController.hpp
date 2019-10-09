#ifndef GAMELIB_QCONTROLLER_HPP
#define GAMELIB_QCONTROLLER_HPP

#include "gamelib/components/UpdateComponent.hpp"

namespace gamelib
{
    class QPhysics;

    constexpr unsigned int input_left      = 1;
    constexpr unsigned int input_right     = 1 << 1;
    constexpr unsigned int input_up        = 1 << 2;
    constexpr unsigned int input_userbegin = 1 << 3;

    class QController : public UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("QController");

        public:
            QController();

            auto update(float elapsed) -> void final override;

            auto setInput(unsigned int input) -> void;
            auto getInput() const             -> unsigned int;

        private:
            auto _refresh(RefreshType type, Component* comp) -> void final override;
            auto _released(unsigned int button) const -> bool;

        public:
            float accelerate;
            float airAccelerate;
            float maxspeed;
            float jumpspeed;
            float jumpDecelerate;
            ComponentReference<QPhysics> phys;
            bool slopejumps;
            bool handleInput;

        private:
            unsigned int _input;
            unsigned int _oldinput;
            bool _canjump;
            bool _jumping;
    };
}

#endif
