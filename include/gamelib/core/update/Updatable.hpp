#ifndef GAMELIB_UPDATABLE_HPP
#define GAMELIB_UPDATABLE_HPP

namespace gamelib
{
    class Updatable
    {
        public:
            virtual ~Updatable() {}
            virtual void update(float elapsed) = 0;
    };
}

#endif
