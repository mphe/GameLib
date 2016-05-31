#ifndef GAMELIB_GAME_OBJECT_HPP
#define GAMELIB_GAME_OBJECT_HPP

#include "Updatable.hpp"
#include "Renderable.hpp"

namespace gamelib
{
    class GameObject : public Updatable, public Renderable
    {
        public:
            virtual ~GameObject() {};
    };
}

#endif
