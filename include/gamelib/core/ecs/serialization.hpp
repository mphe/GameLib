#ifndef GAMELIB_ENTITY_SERIALIZATION_HPP
#define GAMELIB_ENTITY_SERIALIZATION_HPP

#include "Entity.hpp"
#include "gamelib/utils/string.hpp"
#include "gamelib/json/json-transformable.hpp"

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

    // Writes entity state to json, not including children
    auto writeToJson(Json::Value& node, const Entity& ent) -> void;

    // Extracts the component id and removes the id part from the string
    auto extractID(std::string* name)                           -> unsigned int;
    auto generateName(const std::string& name, unsigned int id) -> std::string;

    // Normalizes config and returns true if the config has no errors / warnings, otherwise false.
    // This does not mean that the given config equals the normalized config!
    auto normalizeConfig(const Json::Value& node, Json::Value* out, EntityFactory& factory) -> bool;

    // Calculates the delta between the entity and its template
    auto getConfigDelta(const Entity& ent, Json::Value* out, EntityFactory& factory)        -> bool;
    auto getConfigDelta(const Entity& ent, const Json::Value& normalized, Json::Value* out) -> bool;

    // Shortcut for createComponent()->writeToJson()
    auto getDefaultComponentConfig(const std::string& name, Json::Value* out, EntityFactory& factory) -> void;
}


#endif
