#ifndef OBJECT_SET_HPP
#define OBJECT_SET_HPP

#include <memory>
#include <map>
#include "Tile.hpp"
#include "utils/Factory.hpp"
#include "utils/JsonObject.hpp"

namespace engine
{
    typedef int ObjectID;
    typedef std::unique_ptr<Tile> TilePtr;
    typedef factory::Factory<Tile, ObjectID, const Tile*> TileFactory;

    class ObjectSet : private TileFactory, public Json::JsonObject
    {
        public:
            template <class Derive>
            static Tile* objectCreator(const Tile* tile)
            {
                // Call copy constructor if tile != 0
                if (tile)
                    return new Derive(*static_cast<const Derive*>(tile));
                return new Derive();
            }

        public:
            using TileFactory::del;
            using TileFactory::add;
            using TileFactory::size;

            template <class Derive>
            void add(ObjectID id)
            {
                add(id, objectCreator<Derive>);
            }

            bool loadFromJson(const Json::Value& node);
            void clear();

            TilePtr create(ObjectID id) const;

        private:
            std::map<ObjectID, TilePtr> _templates;
    };
}

/*
 * Config file structure:
 * {
 *     <object id>: {
 *         <object specific paramter>: <value>,
 *         ...
 *     },
 *     ...
 * }
 */

#endif
