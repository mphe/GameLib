#ifndef GAMELIB_RENDERSYSTEM_HPP
#define GAMELIB_RENDERSYSTEM_HPP

#include <vector>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/utils/BatchAllocator.hpp"
#include "RenderStructs.hpp"
#include "gamelib/core/Subsystem.hpp"

// NOTE:
// Layers are basically parents, but I don't want to build and support
// a true render hierachy at this point, because it includes a lot of effort
// for basically nothing. The engine does not even support entity hierachies
// yet and if it would, there would be two hierachy systems.
// So, until it is absolutely necessary to have a render hierachy, layers are
// the only parents a node can have.


namespace gamelib
{
    constexpr unsigned int vertex_position = 1;
    constexpr unsigned int vertex_uv       = 1 << 1;
    constexpr unsigned int vertex_color    = 1 << 2;
    constexpr unsigned int vertex_pos_uv   = vertex_position | vertex_uv;
    constexpr unsigned int vertex_all      = vertex_pos_uv | vertex_color;

    class RenderSystem : public Subsystem<RenderSystem>
    {
        public:
            RenderSystem();

            auto clear()   -> void; // Removes all nodes
            auto destroy() -> void; // Removes everything (nodes, layers, options)

            auto createNode(Component* owner)                 -> NodeHandle;
            auto removeNode(NodeHandle handle)                -> void;
            auto getNode(NodeHandle handle) const             -> const SceneNode*;
            auto getNodeGlobalBBox(NodeHandle handle) const   -> const math::AABBf*;
            auto getNodeGlobalMatrix(NodeHandle handle) const -> const sf::Transform*;

            auto setNodeOwner(NodeHandle handle, Component* owner)                   -> void;
            auto setNodeDepth(NodeHandle handle, int depth)                          -> void;
            auto setNodeLayer(NodeHandle handle, LayerHandle layer)                  -> void;
            auto setNodeTransform(NodeHandle handle, const sf::Transform& transform) -> void;
            auto setNodeOptions(NodeHandle handle, const RenderOptions& options)     -> void;
            auto setNodeOptions(
                    NodeHandle handle,
                    const unsigned int* flags,
                    const float* parallax = nullptr,
                    const sf::BlendMode* blendMode = nullptr,
                    const sf::Texture* texture = nullptr,
                    const sf::Shader* shader = nullptr)
                -> void;

            auto createNodeMesh(NodeHandle handle, size_t size, sf::PrimitiveType type) -> void;
            auto getNodeMesh(NodeHandle handle, size_t offset) const                    -> const sf::Vertex*;
            auto setNodeMeshType(NodeHandle handle, sf::PrimitiveType type)             -> void;
            auto setNodeMeshSize(NodeHandle handle, size_t size)                        -> void;
            auto updateNodeMesh(NodeHandle handle, const sf::Vertex* vertices, size_t size,
                    size_t offset = 0, bool updateSize = true, unsigned int copyflags = vertex_all)
                -> void;

            auto setRootOptions(const RenderOptions& options) -> void;
            auto getRootOptions() const                       -> const RenderOptions&;

            auto createLayer(const std::string& name)                                  -> LayerHandle;
            auto removeLayer(LayerHandle handle)                                       -> void;
            auto getLayer(LayerHandle handle) const                                    -> const RenderLayer*;
            auto findLayer(const std::string& name) const                              -> LayerHandle;
            auto setLayerName(LayerHandle handle, const std::string& name)             -> bool;
            auto setLayerOptions(LayerHandle handle, const RenderOptions& options)     -> void;
            auto setLayerDepth(LayerHandle handle, int depth)                          -> void;
            auto setLayerTransform(LayerHandle handle, const sf::Transform& transform) -> void;

            auto forceUpdate() -> void;
            auto render(sf::RenderTarget& target, const math::AABBf* rect = nullptr) -> size_t;

            auto getComponentAtPosition(const math::Point2f& pos) const -> Component*;
            auto getNumObjectsRendered() const                          -> size_t;

        private:
            auto _updateDirty()                           -> void;
            auto _updateNodeTransform(NodeHandle handle)  -> void;
            auto _updateNodeGlobalBBox(NodeHandle handle) -> void;
            auto _updateMeshBBox(NodeHandle handle)       -> void;

            auto _freeMesh(NodeHandle handle)           -> void;
            auto _markTransformDirty(NodeHandle handle) -> void;
            auto _updateQueue()                         -> void;

        private:
            BatchAllocator<sf::Vertex> _vertices;
            RenderOptions _root;
            SlotMapShort<RenderLayer> _layers;
            SlotMapShort<SceneNode> _nodes;
            std::vector<NodeHandle> _renderqueue;
            size_t _numrendered;

            std::vector<NodeHandle> _dirtylist; // used for transform updates
            bool _orderdirty;    // used to sort and filter render list
    };
}

#endif
