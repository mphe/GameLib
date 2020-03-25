#ifndef GAMELIB_EXPORT_HPP
#define GAMELIB_EXPORT_HPP

#include <string>
#include "json/json.h"
#include "gamelib/core/ecs/ecsmeta.hpp"

namespace gamelib
{
    // Calculates deltas and writes the entity and the entire sub hierachy to json.
    auto writeHierachyToJson(Json::Value* out, const Entity& ent) -> void;

    // Loads the entity and the entire sub hierachy to/from json.
    // If direct is set, it loads entities directly as they are defined in json.
    // Otherwise, entities are created using the factory and get deltas from json.
    auto createHierachyFromJson(const Json::Value& node, bool direct = false) -> EntityReference;

    // Calculate deltas and write/load all entities in EntityManager to/from json
    void writeEntityManagerToJson(Json::Value* node);
    bool loadEntityManagerFromJson(const Json::Value& node, bool clear, bool direct = false);


    // Save/load the game including all necessary subsystems
    bool save(const std::string& fname);
    bool saveToJson(Json::Value* node);
    bool loadSave(const std::string& fname, bool direct = false);
    bool loadSaveFromJson(const Json::Value& node, bool direct = false);
}

#endif
