#ifndef GAMELIB_SCENE_DATA_HPP
#define GAMELIB_SCENE_DATA_HPP

#include "gamelib/core/res/JsonSerializer.hpp"

namespace gamelib
{
    class Scene;

    class SceneData : public JsonSerializer
    {
        friend class Scene;

        public:
            SceneData(int depth = 0, float parallax = 1, unsigned int flags = 0);
            virtual ~SceneData() {}

            auto setDepth(int depth) -> void;
            auto getDepth() const    -> int;

            auto setParallax(float parallax) -> void;
            auto getParallax() const         -> float;

            // TODO: consider removing json stuff
            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

        public:
            unsigned int flags;

        protected:
            Scene* _scene;  // Set by Scene, needed to set dirty flag

        private:
            float _parallax;
            int _depth;
    };
}

#endif
