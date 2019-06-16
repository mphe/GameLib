#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/utils/utils.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    CameraSystem::CameraSystem() :
        _numrendered(0),
        _currentcam(nullptr)
    { }

    Camera& CameraSystem::create(const std::string& name)
    {
        auto cam = find(name);
        if (cam)
        {
            LOG_WARN("Camera already exists: ", name, " -> using existing one");
            return *cam;
        }

        _cams.emplace_back(new Camera(name));
        LOG_DEBUG("Camera added: ", name);
        return *_cams.back();
    }

    auto CameraSystem::remove(const std::string& name) -> void
    {
        auto it = _find(name);
        if (it != _cams.end())
        {
            _cams.erase(it);
            LOG_DEBUG("Camera removed: ", name);
        }
    }

    void CameraSystem::remove(const Camera* cam)
    {
        if (cam)
            remove(cam->getName());
    }

    Camera* CameraSystem::find(const std::string& name)
    {
        return CALL_CONST_OVERLOAD(Camera*, find, name);
    }

    const Camera* CameraSystem::find(const std::string& name) const
    {
        auto it = _find(name);
        return it != _cams.end() ? it->get() : nullptr;
    }

    Camera* CameraSystem::get(size_t index)
    {
        return CALL_CONST_OVERLOAD(Camera*, get, index);
    }

    const Camera* CameraSystem::get(size_t index) const
    {
        return (index < _cams.size()) ? _cams[index].get() : nullptr;
    }

    const Camera* CameraSystem::getCurrent() const
    {
        return _currentcam;
    }

    size_t CameraSystem::size() const
    {
        return _cams.size();
    }

    void CameraSystem::clear()
    {
        _cams.clear();
    }

    auto CameraSystem::_find(const std::string& name) const -> std::vector<CameraPtr>::const_iterator
    {
        for (auto it = _cams.begin(), end = _cams.end(); it != end; ++it)
            if ((*it)->getName() == name)
                return it;
        return _cams.end();
    }

    auto CameraSystem::update(float elapsed) -> void
    {
        for (auto& i : _cams)
            i->update(elapsed);
    }

    auto CameraSystem::render(sf::RenderTarget& target) const -> size_t
    {
        auto sys = getSubsystem<RenderSystem>();
        if (sys)
            return render(sys, target);
        return 0;
    }

    auto CameraSystem::render(const RenderSystem* sys, sf::RenderTarget& target) const -> size_t
    {
        _numrendered = 0;

        if (_cams.empty())
        {
            _numrendered = sys->render(target);
        }
        else
        {
            sf::View reset = target.getView();  // backup current view

            for (const auto& i : _cams)
            {
                if (!i->active)
                    continue;

                _currentcam = i.get();
                i->apply(target);
                _numrendered += sys->render(target, i->getCamRect());
            }

            target.setView(reset); // reset view
            _currentcam = nullptr;
        }

        return _numrendered;
    }

    auto CameraSystem::getNumRendered() const -> size_t
    {
        return _numrendered;
    }

    bool CameraSystem::loadFromJson(const Json::Value& node)
    {
        clear();

        if (!node.isArray())
        {
            LOG_ERROR("Json node is not an array");
            return false;
        }

        // Use an array to save cams to keep their order
        for (Json::ArrayIndex i = 0; i < node.size(); ++i)
        {
            CameraPtr cam(new Camera(""));
            if (!cam->loadFromJson(node[i]))
                // Don't remove it, because code might rely on the camera count
                LOG_WARN("Invalid config for camera ", cam->getName());
            _cams.emplace_back(std::move(cam));
        }

        return true;
    }

    void CameraSystem::writeToJson(Json::Value& node) const
    {
        for (size_t i = 0; i < _cams.size(); ++i)
            _cams[i]->writeToJson(node[static_cast<Json::ArrayIndex>(i)]);
    }
}
