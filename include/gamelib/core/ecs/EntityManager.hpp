#ifndef GAMELIB_ENTITYMANAGER_HPP
#define GAMELIB_ENTITYMANAGER_HPP

#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/Subsystem.hpp"

namespace gamelib
{
    constexpr const char* root_entity_name = "__root__";

    auto findEntity(const std::string& name) -> EntityReference;


    class EntityManager : public Subsystem<EntityManager>
    {
        public:
            ASSIGN_NAMETAG("EntityManager");

        public:
            EntityManager();

            auto add(const std::string& name = "unknown") -> EntityReference;
            auto getRoot() const                          -> EntityReference;
            auto find(const std::string& name) const      -> EntityReference;
            auto clear()                                  -> void;

            // Iterate over the hierachy.
            // Returns the entity breaked at, otherwise null.
            // Return true to break loop, otherwise false.
            // Signature: (EntityReference) -> bool
            template <typename F>
            auto foreach(F f) const -> EntityReference
            {
                return _root.iterSubtree(f);
            }

        private:
            Entity _root;
    };
}

#endif
