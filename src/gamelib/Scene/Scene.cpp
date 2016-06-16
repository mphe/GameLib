#include "gamelib/Scene/Scene.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Scene::Scene() : _currentcam(0), _cams(1)
    {}

    void Scene::destroy()
    {
        _update.clear();
        _render.clear();
        _cams.clear();
        _cams.emplace_back();
        _currentcam = 0;
        LOG_DEBUG_WARN("Scene destroyed");
    }

    void Scene::update(float fps)
    {
        for (auto& i : _cams)
            i.update(fps);
        for (size_t i = _update.size(); i-- > 0;)
            _update[i]->update(fps);
    }

    void Scene::render(sf::RenderTarget& target)
    {
        sf::View tmp(target.getView()); // backup view
        for (size_t i = 0; i < _cams.size(); ++i)
        {
            _currentcam = i;
            target.setView(_cams[i].getView());
            for (auto& o : _render)
                o->render(target);
        }
        target.setView(tmp); // reset view
        _currentcam = 0;
    }

    Updatable* Scene::addObject(Updatable* obj)
    {
        assert("Updatable pointer is a null pointer" && obj);
        _update.push_back(obj);
        LOG_DEBUG("Added Updatable to scene");
        return obj;
    }

    Renderable* Scene::addObject(Renderable* obj)
    {
        assert("Renderable pointer is a null pointer" && obj);
        _render.push_back(obj);
        LOG_DEBUG("Added Renderable to scene");
        return obj;
    }

    GameObject* Scene::addObject(GameObject* obj)
    {
        assert("GameObject pointer is a null pointer" && obj);
        addObject(static_cast<Updatable*>(obj));
        addObject(static_cast<Renderable*>(obj));
        return obj;
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

    Camera& Scene::getCamera(size_t index)
    {
        assert("Trying to access non existing camera" && index < _cams.size());
        return _cams[index];
    }

    const Camera& Scene::getCamera(size_t index) const
    {
        assert("Trying to access non existing camera" && index < _cams.size());
        return _cams[index];
    }

    Camera& Scene::getCurrentCamera()
    {
        return _cams[_currentcam];
    }

    const Camera& Scene::getCurrentCamera() const
    {
        return _cams[_currentcam];
    }

    size_t Scene::getCameraCount() const
    {
        return _cams.size();
    }
}
