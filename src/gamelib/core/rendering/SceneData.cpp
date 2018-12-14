#include "gamelib/core/rendering/SceneData.hpp"
#include "gamelib/core/rendering/Scene.hpp"

namespace gamelib
{
    SceneData::SceneData(int depth, float parallax, unsigned int flags) :
        flags(flags),
        _scene(nullptr),
        _parallax(parallax),
        _depth(depth)
    { };

    void SceneData::setDepth(int depth)
    {
        if (depth != _depth)
        {
            _depth = depth;
            if (_scene)
                _scene->_dirty = true;
        }
    }

    int SceneData::getDepth() const
    {
        return _depth;
    }

    void SceneData::setParallax(float parallax)
    {
        if (_parallax != parallax)
        {
            _parallax = parallax;
            if (_scene)
                _scene->_dirty = true;
        }
    }

    float SceneData::getParallax() const
    {
        return _parallax;
    }

    bool SceneData::loadFromJson(const Json::Value& node)
    {
        setParallax(node.get("parallax", _parallax).asFloat());
        setDepth(node.get("depth", 0).asInt());
        flags = node.get("flags", 0).asUInt();
        return true;
    }

    void SceneData::writeToJson(Json::Value& node) const
    {
        node["parallax"] = getParallax();
        node["depth"] = getDepth();
        node["flags"] = flags;
    }
}
