#include "gamelib/components/rendering/MeshRenderer.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/utils/conversions.hpp"
#include "gamelib/utils/utils.hpp"
#include "gamelib/properties/PropDummy.hpp"
#include "gamelib/properties/PropResource.hpp"
#include <algorithm>

namespace gamelib
{
    MeshRenderer::MeshRenderer() :
        reserveAhead(4),
        _texscale(1, 1),
        _mapping(MapInstance)
    {
        auto primcb = +[](const sf::PrimitiveType* val, MeshRenderer* self) -> const sf::PrimitiveType*
        {
            if (val)
                self->_system->setNodeMeshType(self->_handle, *val);
            return &self->_getNode()->mesh.primitiveType;
        };

        _props.registerProperty("primitiveType", primcb, this, 0, ARRAY_SIZE(str_primitives), str_primitives);
        registerResourceProperty(_props, "texture", _tex, PROP_METHOD(_tex, setTexture), this);
        _props.registerProperty("texoffset", _texoffset, PROP_METHOD(_texoffset, setTexOffset), this);
        _props.registerProperty("texscale", _texscale, PROP_METHOD(_texscale, setTexScale), this);
        _props.registerProperty("mapping", _mapping, PROP_METHOD(_mapping, setMappingMethod), this, 0, NumMappingMethods, str_mappings);
        registerDummyProperty(_props, "vertices");
    }

    auto MeshRenderer::_init() -> bool
    {
        if (!RenderComponent::_init())
            return false;

        // Allocate at least one vertex to prevent out-of-bounds warnings from RenderSystem
        _resize(std::max(reserveAhead, (decltype(reserveAhead))1));
        return true;
    }

    void MeshRenderer::fetch(const math::AABBf& rect, sf::PrimitiveType type)
    {
        sf::Vector2f vertices[] = {
            sf::Vector2f(0, 0),
            sf::Vector2f(0, rect.h),
            sf::Vector2f(rect.w, 0),
            sf::Vector2f(rect.w, rect.h),
        };

        _resize(4);
        _system->updateNodeMesh(_handle, 4, 0, vertices);
        _system->setNodeMeshType(_handle, type);
        _mapTexture();
    }

    void MeshRenderer::fetch(const math::AbstractPointSet<float>& pol, sf::PrimitiveType type)
    {
        sf::Vector2f vertices[pol.size()];

        for (size_t i = 0; i < pol.size(); ++i)
            vertices[i] = convert(pol.get(i));

        fetch(vertices, pol.size(), type);
    }

    auto MeshRenderer::fetch(const sf::Vector2f* vertices, size_t size, sf::PrimitiveType type) -> void
    {
        _resize(size);
        _system->updateNodeMesh(_handle, size, 0, vertices);
        _system->setNodeMeshType(_handle, type);
        _mapTexture();
    }




    void MeshRenderer::setMappingMethod(MappingMethod m)
    {
        _mapping = m;
        _mapTexture();
    }

    MappingMethod MeshRenderer::getMappingMethod() const
    {
        return _mapping;
    }

    void MeshRenderer::setPrimitiveType(sf::PrimitiveType type)
    {
        _system->setNodeMeshType(_handle, type);
    }

    sf::PrimitiveType MeshRenderer::getPrimitiveType() const
    {
        return _system->getNode(_handle)->mesh.primitiveType;
    }

    void MeshRenderer::setTexture(TextureResource::Handle tex)
    {
        _tex = tex;
        _system->setNodeOptions(_handle, nullptr, nullptr, nullptr, &_tex);
        _mapTexture();
    }

    TextureResource::Handle MeshRenderer::getTexture() const
    {
        return _tex;
    }

    void MeshRenderer::setTexOffset(const math::Vec2f& vec)
    {
        const sf::Vertex* mesh = _system->getNodeMesh(_handle, 0);
        sf::Vector2f diff = convert(vec - _texoffset);

        if (convert(diff).isZero())
            return;

        sf::Vector2f uv[size()];

        // if you change something here, remember to change it in _mapTexture(), too
        for (size_t i = 0; i < size(); ++i)
            uv[i] = mesh[i].texCoords + diff;

        _system->updateNodeMesh(_handle, size(), 0, nullptr, uv);
        _texoffset = vec;
    }

    const math::Vec2f& MeshRenderer::getTexOffset() const
    {
        return _texoffset;
    }


    void MeshRenderer::setTexScale(const math::Vec2f& vec)
    {
        _texscale = vec;
        _mapTexture();
    }

    const math::Vec2f& MeshRenderer::getTexScale() const
    {
        return _texscale;
    }


    auto MeshRenderer::size() const -> size_t
    {
        return _system->getNode(_handle)->mesh.size;
    }



    bool MeshRenderer::loadFromJson(const Json::Value& node)
    {
        if (!RenderComponent::loadFromJson(node))
            return false;

        if (node.isMember("vertices"))
        {
            const auto& vertices = node["vertices"];
            size_t size = vertices.size();
            sf::Vector2f mesh[size];

            for (Json::ArrayIndex i = 0; i < size; ++i)
                gamelib::loadFromJson(vertices[i], mesh[i], true);

            _resize(size);
            _system->updateNodeMesh(_handle, size, 0, mesh);
            _mapTexture();
        }

        if (node.isMember("primitiveType"))
            _system->setNodeMeshType(_handle,
                    static_cast<sf::PrimitiveType>(node["primitiveType"].asInt()));

        return true;
    }

    void MeshRenderer::writeToJson(Json::Value& node) const
    {
        RenderComponent::writeToJson(node);

        const sf::Vertex* mesh = _system->getNodeMesh(_handle, 0);
        auto& vertices = node["vertices"];
        vertices.resize(size());

        for (Json::ArrayIndex i = 0; i < size(); ++i)
            gamelib::writeToJson(vertices[i], mesh[i].position);

        node["primitiveType"] = _system->getNode(_handle)->mesh.primitiveType;
    }


    void MeshRenderer::_mapTexture()
    {
        _mapTexture(_mapping);
    }

    void MeshRenderer::_mapTexture(MappingMethod mappingMethod)
    {
        const sf::Vertex* mesh = _system->getNodeMesh(_handle, 0);
        sf::Vector2f uvs[size()];

        switch (mappingMethod)
        {
            default:
            case MapWorld:
                for (size_t i = 0; i < size(); ++i)
                    uvs[i] = convert(getPosition()) + mesh[i].position;
                break;

            case MapInstance:
                for (size_t i = 0; i < size(); ++i)
                    uvs[i] = mesh[i].position;
                break;

            case MapLine:
                {
                    float offx = 0;
                    unsigned int texh = _tex ? _tex->getSize().y : 0;
                    math::Vec2f p[4];

                    for (size_t i = 0; i + 4 <= size(); i += 4)
                    {
                        for (int ii = 0; ii < 4; ++ii)
                            p[ii] = convert(mesh[i + ii].position);

                        double d1 = (p[2] - p[0]).abs(),
                               d2 = (p[3] - p[1]).abs();

                        uvs[i    ] = sf::Vector2f(offx,      0);
                        uvs[i + 1] = sf::Vector2f(offx,      texh);
                        uvs[i + 2] = sf::Vector2f(offx + d1, 0);
                        uvs[i + 3] = sf::Vector2f(offx + d2, texh);

                        offx += std::min(d1, d2);
                    }
                    break;
                }
        }

        // apply scale and offset
        for (size_t i = 0; i < size(); ++i)
        {
            convert(uvs[i]) /= _texscale;
            convert(uvs[i]) += _texoffset;
        }

        _system->updateNodeMesh(_handle, size(), 0, nullptr, uvs);
    }

    void MeshRenderer::_onChanged(const sf::Transform& old)
    {
        RenderComponent::_onChanged(old);
        _mapTexture();
    }

    auto MeshRenderer::_resize(size_t newsize) -> void
    {
        const Mesh& mesh = _system->getNode(_handle)->mesh;
        if (newsize == mesh.size)
            return;
        if (newsize <= mesh.handle.size)
            _system->setNodeMeshSize(_handle, newsize);
        else
        {
            LOG_DEBUG("Reallocating mesh");
            auto ptype = _system->getNode(_handle)->mesh.primitiveType;
            _system->createNodeMesh(_handle, newsize + reserveAhead, ptype);
        }
    }
}
