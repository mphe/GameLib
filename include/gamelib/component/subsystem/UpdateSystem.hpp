#ifndef GAMELIB_UPDATE_SYSTEM_HPP
#define GAMELIB_UPDATE_SYSTEM_HPP

#include <vector>
#include "Subsystem.hpp"
#include "gamelib/Identifier.hpp"

/*
 * (Base) Class for ordered, update based subsystems.
 * It stores raw pointers to UpdateComponents together with a depth in a
 * std::vector and iterates over them to update them (by calling their
 * update function).
 * Components are stored in ascending order, which means components with
 * smaller depths are updated first. The default depth is 0.
 * UpdateSystem is not responsible for allocating or freeing components!
 * It's safe to add and remove elements inside the update loop.
 * Removing a component or changing the depth takes O(n) time.
 * See declaration of UpdateSystem::Data for further information.
 */

namespace gamelib
{
    class UpdateComponent;

    class UpdateSystem : public Identifier<0x9da718b9, Subsystem>
    {
        public:
            UpdateSystem();
            virtual ~UpdateSystem() {}

            void add(UpdateComponent* comp, int depth = 0);
            void remove(UpdateComponent* comp);
            void setDepth(UpdateComponent* comp, int depth);

            void update(float fps);
            void destroy();
            size_t size() const;

        private:
            /*
             * depth and remove are stored outside of UpdateComponent because it's
             * more cache efficient.
             * Removing a component or changing its depth (which rarely occurrs)
             * requires O(n) time (n = number of registered components).
             * Since this system works by calling std::stable_sort whenever a component
             * was added or a depth changed, this is a good trade-off.
             * Storing remove inside UpdateComponent could also lead to segfaults
             * because the object could already be deleted.
             * TODO: Consider passing the index of the current component in the update
             *       method. Since the index stays valid until another update call,
             *       it can be used as temporary handle for O(1) deletion/depth change.
             */
            struct Data
            {
                UpdateComponent* comp;
                int depth;
                bool remove; // TODO: Use int for better alignment?

                Data(UpdateComponent* c, int d = 0);

                bool operator==(const Data& d) const;
                bool operator<(const Data& d) const;
                bool operator>(const Data& d) const;
            };

        private:
            std::vector<Data>::iterator _find(UpdateComponent* comp);

        private:
            size_t _remove;
            size_t _insert;
            std::vector<Data> _comps; // TODO: Use deque?
    };
}

#endif
