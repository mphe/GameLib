#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/Camera.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/conversions.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/res/JsonResource.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>

namespace gamelib
{
    constexpr const char* Scene::name;

    Scene::Scene() :
        _currentcam(nullptr),
        _default(nullptr),
        _dirty(false)
    {}

    void Scene::destroy()
    {
        _renderQueue.clear();
        _cams.clear();
        _currentcam = nullptr;
        _default = nullptr;
        _dirty = false;
        _layers.clear();
        LOG_DEBUG_WARN("Scene destroyed");
    }

    void Scene::update(float elapsed)
    {
        for (auto& i : _cams)
            i->update(elapsed);
    }

    unsigned int Scene::render(sf::RenderTarget& target)
    {
        _numrendered = 0;

        // This call must be in render(), otherwise new objects won't appear
        // when the update loop is paused.
        _updateQueue();

        if (_cams.empty())
        {
            _numrendered = _render(target);
        }
        else
        {
            // Choose view that will be restored after rendering
            sf::View reset = _default ? _default->getView(target) : target.getView();

            for (const auto& i : _cams)
            {
                _currentcam = i;
                i->apply(target);
                _numrendered += _render(target);
            }

            target.setView(reset); // reset view
            _currentcam = nullptr;
        }

        return _numrendered;
    }

    unsigned int Scene::_render(sf::RenderTarget& target)
    {
        math::AABBf vbox;
        unsigned int numrendered = 0;
        const sf::View& view = target.getView();

        { // Calculate view bounding box
            sf::Transform vtrans;
            vtrans.rotate(view.getRotation());
            auto half = view.getSize() / 2.f;
            vbox = convert(vtrans.transformRect(sf::FloatRect(-half, view.getSize())));
            vbox.pos = convert(view.getCenter() - half);
        }

        for (auto& o : _renderQueue)
        {
            sf::Transform trans;
            math::Vec2f translate;
            unsigned int flags = o->flags | this->flags;
            float parallax = o->getParallax() * getParallax();
            auto bbox = o->getBBox();

            if (_layers.isValid(o->getLayer()))
            {
                parallax *= _layers[o->getLayer()].getParallax();
                flags |= _layers[o->getLayer()].flags;
            }

            if (flags & render_invisible)
                continue;

            if (flags & render_hidden && !(flags & render_drawhidden))
                continue;

            if (!(flags & render_noparallax) && !math::almostEquals(parallax, 1.0f))
            {
                auto vcenter = view.getCenter();
                translate = (bbox.getCenter() - convert(vcenter)) * (parallax - 1);

                if (flags & render_scaleparallax)
                {
                    trans.scale(parallax, parallax, vcenter.x, vcenter.y);
                    bbox.extend(bbox.size * parallax - bbox.size);
                    bbox.pos += translate;
                }
                else
                {
                    trans.translate(translate.x, translate.y);
                    bbox.pos += translate;
                }

                // sf::RectangleShape rect(convert(bbox.size));
                // rect.setFillColor(sf::Color::Transparent);
                // rect.setOutlineColor(sf::Color::White);
                // rect.setOutlineThickness(1);
                // rect.setPosition(convert(bbox.pos));
                // target.draw(rect);
            }

            // Skip if outside of view
            if (!bbox.intersect(vbox))
            {
                if (bbox.w == 0 || bbox.h == 0)
                    LOG_WARN("SceneObject bounding box has 0 width or height: ", bbox.w, "x", bbox.h);
                continue;
            }

            o->render(target, trans);
            ++numrendered;
        }

        return numrendered;
    }

    SceneObject* Scene::add(SceneObject* obj)
    {
        assert("SceneObject pointer is a null pointer" && obj);
        _renderQueue.push_back(obj);
        obj->_scene = this;
        _dirty = true;
        LOG_DEBUG("Added SceneObject to scene");
        return obj;
    }

    void Scene::remove(SceneObject* obj)
    {
        assert("SceneObject pointer is a null pointer" && obj);
        auto it = std::find(_renderQueue.begin(), _renderQueue.end(), obj);
        if (it != _renderQueue.end())
        {
            (*it)->_scene = nullptr;
            _renderQueue.erase(it);
            LOG_DEBUG("Removed SceneObject from scene");
        }
    }

    Camera& Scene::addCamera(Camera* cam)
    {
        assert(cam && "Camera is null");
        _cams.emplace_back(cam);
        LOG_DEBUG("Added camera to scene");
        return *cam;
    }

    void Scene::removeCamera(const Camera* cam)
    {
        auto it = std::find(_cams.begin(), _cams.end(), cam);
        if (it != _cams.end())
        {
            _cams.erase(it);
            if (cam == _default)
                _default = nullptr;
        }
    }

    void Scene::setDefaultCamera(const Camera* cam)
    {
        _default = cam;
    }

    Camera* Scene::findCamera(const std::string& name)
    {
        for (auto& i : _cams)
            if (i->getName() == name)
                return i;
        return nullptr;
    }

    const Camera* Scene::findCamera(const std::string& name) const
    {
        return const_cast<Scene*>(this)->findCamera(name);
    }

    Camera* Scene::getCamera(size_t index)
    {
        return (index < _cams.size()) ? _cams[index] : nullptr;
    }

    const Camera* Scene::getCamera(size_t index) const
    {
        return const_cast<Scene*>(this)->getCamera(index);
    }

    const Camera* Scene::getCurrentCamera() const
    {
        return _currentcam;
    }

    size_t Scene::getNumCameras() const
    {
        return _cams.size();
    }

    Layer::Handle Scene::createLayer(const std::string& name)
    {
        auto hnd = getLayer(name);
        if (hnd.isNull())
        {
            hnd = _layers.acquire();
            _layers[hnd] = Layer(name);
            _layers[hnd]._scene = this;
        }
        else
            LOG_WARN("Layer already exists: ", name, " -> using existing one");

        return hnd;
    }

    void Scene::deleteLayer(Layer::Handle handle)
    {
        _layers.destroy(handle);
    }

    const Layer* Scene::getLayer(Layer::Handle handle) const
    {
        return (_layers.isValid(handle)) ? &_layers[handle] : nullptr;
    }

    Layer* Scene::getLayer(Layer::Handle handle)
    {
        return (_layers.isValid(handle)) ? &_layers[handle] : nullptr;
    }

    Layer::Handle Scene::getLayer(const std::string& name) const
    {
        for (auto it = _layers.begin(), end = _layers.end(); it != end; ++it)
            if ((*it).getName() == name)
                return it.handle();
        return Layer::Handle();
    }

    bool Scene::loadFromJson(const Json::Value& node)
    {
        destroy();

        SceneData::loadFromJson(node);

        if (node.isMember("layers"))
        {
            const auto& layers = node["layers"];
            for (auto it = layers.begin(), end = layers.end(); it != end; ++it)
            {
                auto h = createLayer(it.key().asString());

                if (!getLayer(h)->loadFromJson(*it))
                    deleteLayer(h);
            }
        }

        _default = findCamera(node["defaultcam"].asString());

        _updateQueue();
        return true;
    }

    void Scene::writeToJson(Json::Value& node)
    {
        SceneData::writeToJson(node);

        if (_layers.begin() != _layers.end())
        {
            auto& layers = node["layers"];
            for (auto& i : _layers)
                i.writeToJson(layers[i.getName()]);
        }

        if (_default)
            node["defaultcam"] = _default->getName();
    }

    void Scene::_updateQueue()
    {
        if (_dirty)
        {
            LOG_DEBUG("Scene content changed -> sorting");
            std::stable_sort(_renderQueue.begin(), _renderQueue.end(), [this](SceneObject* a, SceneObject* b) {
                    auto la = a->getLayer(),
                         lb = b->getLayer();
                    const int da = _layers.isValid(la) ? _layers[la]._depth : 0,
                              db = _layers.isValid(lb) ? _layers[lb]._depth : 0;
                    return (da != db) ? da > db : a->_depth > b->_depth;
                });
            _dirty = false;
        }
    }

    size_t Scene::getNumObjectsRendered() const
    {
        return _numrendered;
    }
}
