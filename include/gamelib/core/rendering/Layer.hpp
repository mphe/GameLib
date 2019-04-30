#ifndef GAMELIB_SCENE_LAYER_HPP
#define GAMELIB_SCENE_LAYER_HPP

#include "SceneData.hpp"
#include "gamelib/utils/SlotMap.hpp"

namespace gamelib
{
    typedef SlotKeyShort LayerHandle;

    class Layer : public SceneData
    {
        friend class Scene;

        public:
            typedef SlotKeyShort Handle;

        public:
            Layer(int depth = 0, float parallax = 1, unsigned int flags = 0);
            Layer(const std::string& name, int depth = 0, float parallax = 1, unsigned int flags = 0);

            auto getName() const -> const std::string&;

        private:
            std::string _name;  // unique id
    };
}

#endif
