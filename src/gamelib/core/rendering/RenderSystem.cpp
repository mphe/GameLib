#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/utils/log.hpp"
#include "math/geometry/PointSet.hpp"
#include "gamelib/utils/conversions.hpp"

// Enables setting a breakpoint inside a macro by breaking here
template <typename... Args>
void _warn_macro(Args&&... args)
{
    LOG_WARN(args...);
}

#define ASSURE_VALID_RET(nodehandle, ret) \
if (!_nodes.isValid(nodehandle))    \
{   \
    _warn_macro("Trying to access invalid node handle");   \
    return ret; \
}

#define ASSURE_VALID(nodehandle) ASSURE_VALID_RET(nodehandle, )

#define CHECK_MESH_BOUNDS_RET(handle, offset, ret) \
if (!_nodes[handle].mesh.handle.isValid() || (offset) >= _nodes[handle].mesh.handle.size) \
{   \
    _warn_macro("Vertex index out of bounds"); \
    return ret; \
}

#define CHECK_MESH_BOUNDS(handle, offset) CHECK_MESH_BOUNDS_RET(handle, offset, )


namespace gamelib
{
    math::AABBf calculateBBox(const sf::Vertex* vertices, size_t size)
    {
        if (size < 2)
            return math::AABBf();

        math::Vec2f min = convert(vertices[0].position),
                    max = min;

        for (size_t i = 1; i < size; ++i)
        {
            const math::Vec2f& p = convert(vertices[i].position);
            for (int k = 0; k < 2; ++k)
            {
                min[k] = std::min(min[k], p[k]);
                max[k] = std::max(max[k], p[k]);
            }
        }

        return math::AABBf(min.asPoint(), max - min);
    }




	RenderSystem::RenderSystem() :
        _numrendered(0),
        _orderdirty(true)
	{ }


    auto RenderSystem::clear() -> void
    {
        LOG_DEBUG_WARN("Clearing all SceneNodes");
        _nodes.clear();
        _vertices.clear();
        _dirtylist.clear();
        _renderqueue.clear();
        _orderdirty = false;
    }

    auto RenderSystem::destroy() -> void
    {
        LOG_DEBUG_WARN("Destroying RenderSystem");
        clear();
        _layers.clear();
        _root = RenderOptions();
        _numrendered = 0;
    }


    auto RenderSystem::createNode(Component* owner) -> NodeHandle
    {
        NodeHandle handle = _nodes.acquire();
        _nodes[handle].owner = owner;
        // don't add to queue, because there's no mesh yet
        return handle;
    }

    auto RenderSystem::removeNode(NodeHandle handle) -> void
    {
        ASSURE_VALID(handle);
        _nodes.destroy(handle);
        _orderdirty = true;
    }

    auto RenderSystem::getNode(NodeHandle handle) const -> const SceneNode*
    {
        return _nodes.get(handle);
    }

    auto RenderSystem::getNodeGlobalBBox(NodeHandle handle) const -> const math::AABBf*
    {
        ASSURE_VALID_RET(handle, nullptr);

        // Should be safe, because why instantiate a const RenderSystem?
        const_cast<RenderSystem*>(this)->_updateNodeTransform(handle);
        return &_nodes[handle]._globalBBox;
    }

    auto RenderSystem::getNodeGlobalMatrix(NodeHandle handle) const -> const sf::Transform*
    {
        ASSURE_VALID_RET(handle, nullptr);

        // Should be safe, because why instantiate a const RenderSystem?
        const_cast<RenderSystem*>(this)->_updateNodeTransform(handle);
        return &_nodes[handle]._globalTransform;
    }


    auto RenderSystem::setNodeOwner(NodeHandle handle, Component* owner) -> void
    {
        ASSURE_VALID(handle);
        _nodes[handle].owner = owner;
    }

    auto RenderSystem::setNodeOptions(NodeHandle handle, const RenderOptions& options) -> void
    {
        ASSURE_VALID(handle);
        _nodes[handle].options = options;
    }

    auto RenderSystem::setNodeOptions(
            NodeHandle handle,
            const unsigned int* flags,
            const float* parallax,
            const sf::BlendMode* blendMode,
            const sf::Texture* texture,
            const sf::Shader* shader)
        -> void
    {
        ASSURE_VALID(handle);
        RenderOptions options = _nodes[handle].options;
        if (flags)     options.flags = *flags;
        if (parallax)  options.parallax = *parallax;
        if (blendMode) options.blendMode = *blendMode;
        if (texture)   options.texture = texture;
        if (shader)    options.shader = shader;
        setNodeOptions(handle, options);
    }

    auto RenderSystem::setNodeDepth(NodeHandle handle, int depth) -> void
    {
        ASSURE_VALID(handle);
        _nodes[handle].depth = depth;
        _orderdirty = true;
    }

    auto RenderSystem::setNodeLayer(NodeHandle handle, LayerHandle layer) -> void
    {
        ASSURE_VALID(handle);

        if (!_layers.isValid(layer))
        {
            LOG_WARN("Assigning invalid layer");
            return;
        }

        _nodes[handle].layer = layer;
        _orderdirty = true;
        _markTransformDirty(handle);
    }

    auto RenderSystem::setNodeTransform(NodeHandle handle, const sf::Transform& transform) -> void
    {
        ASSURE_VALID(handle);
        _nodes[handle].transform = transform;
        _markTransformDirty(handle);
    }


    auto RenderSystem::createNodeMesh(NodeHandle handle, size_t size, sf::PrimitiveType type) -> void
    {
        ASSURE_VALID(handle);

        SceneNode& node = _nodes[handle];

        // Add to queue if there was no mesh yet
        if (!node.mesh.handle.isValid())
        {
            _renderqueue.push_back(handle);
            _orderdirty = true;
        }

        _freeMesh(handle);
        node.mesh.handle = _vertices.allocate(size);
        node.mesh.primitiveType = type;
        node.mesh.size = size;
        // don't update bbox, because there is no vertex data yet
    }

    auto RenderSystem::getNodeMesh(NodeHandle handle, size_t offset) const -> const sf::Vertex*
    {
        ASSURE_VALID_RET(handle, nullptr);
        CHECK_MESH_BOUNDS_RET(handle, offset, nullptr);
        return _vertices.get(_nodes[handle].mesh.handle.index + offset);
    }

    auto RenderSystem::updateNodeMesh(NodeHandle handle, const sf::Vertex* vertices, size_t size,
            size_t offset, bool updateSize, unsigned int copyflags)
        -> void
    {
        ASSURE_VALID(handle);
        CHECK_MESH_BOUNDS(handle, 0);
        size_t stop = offset + size;
        Mesh& mesh = _nodes[handle].mesh;

        if (mesh.handle.size < stop)
            LOG_WARN("Trying to assign more vertices than space allocated -> Clipping to maximum");

        stop = std::min(stop, mesh.handle.size);
        bool sizechanged = updateSize && stop != mesh.size;

        if (sizechanged)
            mesh.size = stop;

        for (size_t i = offset; i < stop; ++i)
        {
            sf::Vertex& v = *_vertices.get(mesh.handle.index + i);

            if (copyflags & vertex_position) v.position  = vertices[i - offset].position;
            if (copyflags & vertex_uv)       v.texCoords = vertices[i - offset].texCoords;
            if (copyflags & vertex_color)    v.color     = vertices[i - offset].color;
        }

        if (copyflags & vertex_position || sizechanged)
            _updateMeshBBox(handle);
    }

    auto RenderSystem::setNodeMeshType(NodeHandle handle, sf::PrimitiveType type) -> void
    {
        ASSURE_VALID(handle);
        _nodes[handle].mesh.primitiveType = type;
    }

    auto RenderSystem::setNodeMeshSize(NodeHandle handle, size_t size) -> void
    {
        ASSURE_VALID(handle);
        CHECK_MESH_BOUNDS(handle, size == 0 ? 0 : size - 1);
        _nodes[handle].mesh.size = size;
        _updateMeshBBox(handle);
    }


    auto RenderSystem::setRootOptions(const RenderOptions& options) -> void
    {
        _root = options;
    }

    auto RenderSystem::getRootOptions() const -> const RenderOptions&
    {
        return _root;
    }


	auto RenderSystem::getNumObjectsRendered() const -> size_t
	{
        return _numrendered;
	}

    auto RenderSystem::forceUpdate() -> void
    {
        _updateDirty();
        _updateQueue();
    }

    auto RenderSystem::render(sf::RenderTarget& target, const math::AABBf* rect) -> size_t
    {
        forceUpdate();

        _numrendered = 0;
        RenderOptions parent = _root;

        for (NodeHandle handle : _renderqueue)
        {
            const SceneNode& node = _nodes[handle];
            const Mesh& mesh = node.mesh;

            if (mesh.size == 0)
                continue;

            // TODO: layer

            if (!rect || math::intersect(*rect, node._globalBBox))
            {
                RenderOptions options = node.options.inherited(parent);
                // TODO: parallax, flags

                target.draw(
                        _vertices.get(mesh.handle.index), mesh.size, mesh.primitiveType,
                        sf::RenderStates(options.blendMode, node._globalTransform, options.texture, options.shader));

                ++_numrendered;
            }
        }

        return _numrendered;
    }


    auto RenderSystem::_updateDirty() -> void
    {
        if (_dirtylist.empty())
            return;

        LOG_DEBUG("Recalculating global node transforms and bboxs");

        for (NodeHandle handle : _dirtylist)
            if (_nodes.isValid(handle))
                _updateNodeTransform(handle);

        _dirtylist.clear();
    }

    auto RenderSystem::_updateNodeTransform(NodeHandle handle) -> void
    {
        SceneNode& node = _nodes[handle];

        if (!node._globaldirty)
            return;

        RenderLayer* layer = _layers.get(node.layer);
        node._globalTransform = layer ? layer->transform * node.transform : node.transform;
        _updateNodeGlobalBBox(handle);
        node._globaldirty = false;
    }

    auto RenderSystem::_updateNodeGlobalBBox(NodeHandle handle) -> void
    {
        SceneNode& node = _nodes[handle];
        if (node.mesh.size == 0)
            node._globalBBox = math::AABBf();
        else
            node._globalBBox = convert(
                    node._globalTransform.transformRect(convert(node.mesh.bbox)));
    }

    auto RenderSystem::_updateMeshBBox(NodeHandle handle) -> void
    {
        SceneNode& node = _nodes[handle];
        Mesh& mesh = node.mesh;
        mesh.bbox = calculateBBox(_vertices.get(mesh.handle.index), mesh.size);

        // If the node is dirty, we can skip the global bbox, because it is
        // recalculated anyway.
        if (!node._globaldirty)
            _updateNodeGlobalBBox(handle);
    }

    auto RenderSystem::_freeMesh(NodeHandle handle) -> void
    {
        ASSURE_VALID(handle);

        Mesh& mesh = _nodes[handle].mesh;

        if (mesh.handle.isValid())
        {
            LOG_DEBUG_WARN("Freeing mesh");
            _vertices.free(mesh.handle);
            mesh = Mesh();
            _nodes[handle]._globalBBox = math::AABBf();
        }
    }

    auto RenderSystem::_markTransformDirty(NodeHandle handle) -> void
    {
        SceneNode& node = _nodes[handle];
        if (!node._globaldirty)
        {
            node._globaldirty = true;
            _dirtylist.push_back(handle);
        }
    }

	auto RenderSystem::_updateQueue() -> void
	{
        if (!_orderdirty)
            return;

        LOG_DEBUG("RenderSystem content changed -> sorting");

        auto begin = _renderqueue.begin(),
             end = _renderqueue.end();

        std::stable_sort(begin, end, [this](NodeHandle a, NodeHandle b) {
            // auto la = a->getLayer(),
            //      lb = b->getLayer();
            // const int da = _layers.isValid(la) ? _layers[la]._depth : 0,
            //       db = _layers.isValid(lb) ? _layers[lb]._depth : 0;
            // return (da != db) ? da > db : a->_depth > b->_depth;
            const SceneNode *nodea = _nodes.get(a),
                            *nodeb = _nodes.get(b);
            if (!nodea)
                return false;
            if (!nodeb)
                return true;
            return nodea->depth > nodeb->depth;
        });

        auto it = std::find_if(begin, end, [this](NodeHandle handle) { return !_nodes.isValid(handle); });

        if (it != end)
        {
            LOG_DEBUG_WARN("Removing ", std::distance(it, end), " deleted nodes from queue");
            _renderqueue.erase(it, end);
        }

        _orderdirty = false;
	}
}
