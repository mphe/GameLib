#ifndef GAMELIB_RENDERSTRUCTS_HPP
#define GAMELIB_RENDERSTRUCTS_HPP

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include "gamelib/utils/BatchAllocator.hpp"
#include "gamelib/utils/SlotMap.hpp"
#include "math/geometry/AABB.hpp"

namespace gamelib
{
    class Component;

    typedef SlotKeyShort NodeHandle;
    typedef SlotKeyShort LayerHandle;

    // Don't include sf::Transform here because it is updated and cached separately
    class RenderOptions
    {
        public:
            RenderOptions();

            auto inherit(const RenderOptions& parent) -> RenderOptions&;
            auto inherited(const RenderOptions& parent) const -> RenderOptions;

        public:
            unsigned int flags;
            float parallax;
            sf::BlendMode blendMode;
            const sf::Texture* texture;
            const sf::Shader* shader;

        public:
            static const RenderOptions defaultOptions;
    };

    struct RenderLayer
    {
        int depth;
        RenderOptions options;
        sf::Transform transform;
        std::string name;

        RenderLayer();
    };

    struct Mesh
    {
        BatchHandle handle;
        sf::PrimitiveType primitiveType;
        size_t size; // amount of vertices to actually use
        math::AABBf bbox;

        Mesh();
    };

    class SceneNode
    {
        friend class RenderSystem;

        public:
            SceneNode();

            RenderOptions options;
            sf::Transform transform;
            Mesh mesh;
            LayerHandle layer;
            int depth;
            Component* owner;

        private:
            math::AABBf _globalBBox;
            sf::Transform _globalTransform;

            bool _globaldirty;    // used for transform updates
    };
}

#endif
