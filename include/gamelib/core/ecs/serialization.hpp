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
    // Clears the entity and recreates directly from json (like state loading)
    bool loadFromJson(const Json::Value& node, Entity& ent);

    // Keeps the entity and overwrites from json.
    // If createMissing is set, missing components will be created.
    bool extendFromJson(const Json::Value& node, Entity& ent, bool createMissing = false);

    void writeToJson(Json::Value& node, const Entity& ent);

    // Calls a callback for each component about to be serialized.
    // The callback handles the serialization (or not).
    // Signature: bool(Json::Value&, Component&)
    template <typename F>
    void writeToJson(Json::Value& node, const Entity& ent, F callback)
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
                    comps[join(i.ptr->getName(), "#", i.id)] = comp;
            }
        }
    }
}


#endif
