#ifndef GAMELIB_ENTITY_SERIALIZATION_HPP
#define GAMELIB_ENTITY_SERIALIZATION_HPP

#include "Entity.hpp"
#include "gamelib/utils/string.hpp"

/*
 * Config file structure:
 * {
 *     "name": <str>,
 *     "transform": {
 *         "pos": [ <float>, <float> ],
 *         "scale": [ <float>, <float> ],
 *         "angle": <float>,
 *     },
 *     "flags": <uint>,
 *
 *     "components": {
 *         "<ComponentName>#<subid>": {
 *             ...
 *         },
 *         "ExampleComponent#0": {
 *             ...
 *         },
 *         "ExampleComponent#1": {
 *             ...
 *         },
 *         ...
 *     }
 * }
 */

namespace gamelib
{
    class EntityFactory;

    // Clears the entity and recreates directly from json (like state loading)
    auto loadFromJson(const Json::Value& node, Entity& ent) -> bool;

    // Keeps the entity and overwrites from json.
    // If createMissing is set, missing components will be created.
    auto extendFromJson(const Json::Value& node, Entity& ent, bool createMissing = false) -> bool;

    auto writeToJson(Json::Value& node, const Entity& ent) -> void;

    // Extracts the id and removes the id part from the string
    auto extractID(std::string* name)                               -> unsigned int;
    auto generateName(const std::string& name, unsigned int id)     -> std::string;

    // Returns true if the config has no errors / warnings, otherwise false.
    // This does not mean that the given config equals the normalized config!
    auto normalizeConfig(const Json::Value& node, Json::Value* out, EntityFactory& factory) -> bool;

    // Calls a callback for each component about to be serialized.
    // The callback handles the serialization (or not).
    // Signature: bool(Json::Value&, Component&)
    template <typename F>
    auto writeToJson(Json::Value& node, const Entity& ent, F callback) -> void
    {
        node["name"] = ent.getName();
        node["flags"] = ent.flags;
        gamelib::writeToJson(node["transform"], ent.getTransform());

        auto& comps = node["components"];
        comps = Json::Value(Json::objectValue);

        for (const auto& i : ent._components)
        {
            if (i.ptr)
            {
                Json::Value comp;
                callback(comp, *i.ptr);
                if (!comp.isNull())
                    comps[generateName(i.ptr->getName(), i.id)] = comp;
            }
        }
    }
}


#endif
