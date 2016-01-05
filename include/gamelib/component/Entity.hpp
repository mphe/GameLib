#ifndef GAMELIB_ENTITY_HPP
#define GAMELIB_ENTITY_HPP

#include <vector>
#include "Component.hpp"
#include "gamelib/Identifier.hpp"
#include "gamelib/Identifiable.hpp"

namespace gamelib
{
    class Entity
    {
        public:
            // Add a component to the list. This doesn't replace other components with the same ID.
            void add(ComponentPtr ptr);

            // Remove the first component found with the given ID.
            void remove(ComponentID id);

            // Return a pointer to the first component found with the given ID.
            Component* get(ComponentID id) const;

            // Only works when T inherits in some way from "Identifier"
            // Returns a pointer to the first component of the given type.
            template <class T>
            T* get() const
            {
                static_assert(isIdentifiable<T>::value, "T must inherit from \"Identifier\".");
                return static_cast<T*>(get(T::id));
            }

            // Same as above but for remove
            template <class T>
            void remove()
            {
                static_assert(isIdentifiable<T>::value, "T must inherit from \"Identifier\".");
                remove(T::id);
            }

            void clear();
            size_t size() const;

        private:
            std::vector<ComponentPtr> _components;
    };
}

#endif
