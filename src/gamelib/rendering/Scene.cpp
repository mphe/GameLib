#include "gamelib/rendering/Scene.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Scene::Scene() :
        _currentcam(-1),
        _default(-1),
        _dirty(false)
    {}

    void Scene::destroy()
    {
        _renderQueue.clear();
        _cams.clear();
        _currentcam = -1;
        _default = -1;
        _dirty = false;
        LOG_DEBUG_WARN("Scene destroyed");
    }

    void Scene::update(float elapsed)
    {
        for (auto& i : _cams)
            i.update(elapsed);

        if (_dirty)
        {
            LOG_DEBUG("Scene content changed -> sorting");
            std::sort(_renderQueue.begin(), _renderQueue.end(),
                    [](SceneObject* a, SceneObject* b) { return a->_depth > b->_depth; });
            _dirty = false;
        }
    }

    void Scene::render(sf::RenderTarget& target)
    {
        if (_cams.empty())
        {
            _render(target);
        }
        else
        {
            sf::View backup;  // backup view

            if (_default < _cams.size())
                backup = target.getView();

            for (size_t i = 0; i < _cams.size(); ++i)
            {
                if (i == _default)
                    backup = _cams[i].getView();

                _currentcam = i;
                target.setView(backup);
                _render(target);
            }

            target.setView(backup); // reset view
            _currentcam = -1;
        }
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

    Camera& Scene::addCamera()
    {
        _cams.emplace_back();
        LOG_DEBUG("Added camera to scene");
        return _cams.back();
    }

    Camera& Scene::addCamera(const Camera& cam)
    {
        _cams.push_back(cam);
        LOG_DEBUG("Added camera to scene");
        return _cams.back();
    }

    void Scene::setDefaultCamera(size_t index)
    {
        _default = index;
    }

    Camera* Scene::getCamera(size_t index)
    {
        return (index < _cams.size()) ? &_cams[index] : nullptr;
    }

    const Camera* Scene::getCamera(size_t index) const
    {
        return (index < _cams.size()) ? &_cams[index] : nullptr;
    }

    Camera* Scene::getCurrentCamera()
    {
        return getCamera(_currentcam);
    }

    const Camera* Scene::getCurrentCamera() const
    {
        return getCamera(_currentcam);
    }

    size_t Scene::getCameraCount() const
    {
        return _cams.size();
    }

    void Scene::_render(sf::RenderTarget& target)
    {
        for (auto& o : _renderQueue)
            o->render(target);
    }
}
