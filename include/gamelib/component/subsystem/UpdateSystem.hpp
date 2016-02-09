#ifndef GAMELIB_UPDATE_SYSTEM_HPP
#define GAMELIB_UPDATE_SYSTEM_HPP

#include <vector>
#include "Subsystem.hpp"
#include "gamelib/Identifier.hpp"

/*
 * (Base) Class for update based subsystems.
 * Simply stores raw pointers to UpdateComponent objects in a std::vector and
 * iterates over them to update them (by calling their update function).
 * UpdateSystem is not responsible for allocating or freeing components!
 */

namespace gamelib
{
    class UpdateComponent;

    class UpdateSystem : public Identifier<0x9da718b9, Subsystem>
    {
        public:
            UpdateSystem();
            virtual ~UpdateSystem() {}

            void add(UpdateComponent* comp);
            void remove(UpdateComponent* comp);

            void update(float fps);
            void destroy();
            size_t size() const;

        private:
            struct Data
            {
                UpdateComponent* comp;
                bool remove;

                Data(UpdateComponent* c) : comp(c), remove(false) {}

                bool operator==(const Data& d) const
                {
                    return comp == d.comp && remove == d.remove;
                }
            };

        private:
            size_t _remove;
            std::vector<Data> _comps;
    };
}

#endif
