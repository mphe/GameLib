#include "gamelib/Scene.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Scene::Scene() : _currentcam(NULL)
    {}

    void Scene::destroy()
    {
        _objs.clear();
        _cams.clear();
        LOG_DEBUG_WARN("Scene destroyed");
    }

    void Scene::update(float fps)
    {
        for (auto& i : _cams)
            i->update(fps);
        for (size_t i = _objs.size(); i-- > 0;) // objects in the front get the "focus" first
            _objs[i]->update(fps);
    }

    void Scene::render(sf::RenderTarget& target) const
    {
        if (!_cams.empty())
        {
            sf::View tmp(target.getView()); // backup view
            for (auto& c : _cams)
            {
                _currentcam = c.get();
                target.setView(c->getView());
                for (auto& o : _objs)
                    o->render(target);
            }
            target.setView(tmp); // reset view
        }
        else
        {
            for (auto& o : _objs)
                o->render(target);
        }
    }

    GameObject& Scene::addObject(GameObjectPtr obj)
    {
        assert("GameObject pointer is a null pointer" && obj);
        _objs.push_back(std::move(obj));
        LOG_DEBUG("Added object to scene");
        return *_objs.back();
    }

    Camera& Scene::addCamera(CameraPtr cam)
    {
        assert("Camera pointer is a null pointer" && cam);
        _cams.push_back(std::move(cam));
        _currentcam = _cams.front().get();
        LOG_DEBUG("Added camera to scene");
        return *_cams.back();
    }

    Camera& Scene::getCamera(size_t index) const
    {
        return *_cams[index];
    }

    Camera* Scene::getCurrentCamera() const
    {
        return _currentcam;
    }

    size_t Scene::getCameraCount() const
    {
        return _cams.size();
    }
}
