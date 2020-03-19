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

    void CameraSystem::add(Camera* cam)
    {
        _cams.emplace_back(cam);
        LOG_DEBUG("Camera added: ", cam->name);
    }

    auto CameraSystem::remove(const std::string& name) -> void
    {
        for (size_t i = 0; i < _cams.size(); ++i)
            if (_cams[i]->name == name)
                return remove(i);
    }

    void CameraSystem::remove(const Camera* cam)
    {
        for (size_t i = 0; i < _cams.size(); ++i)
            if (_cams[i] == cam)
                return remove(i);
    }

    auto CameraSystem::remove(size_t i) -> void
    {
        LOG_DEBUG("Camera removed: ", _cams[i]->name);
        _cams.erase(_cams.begin() + i);
    }

    Camera* CameraSystem::get(const std::string& name)
    {
        return CALL_CONST_OVERLOAD(Camera*, get, name);
    }

    const Camera* CameraSystem::get(const std::string& name) const
    {
        for (size_t i = 0; i < _cams.size(); ++i)
            if (_cams[i]->name == name)
                return _cams[i];
        return nullptr;
    }

    Camera* CameraSystem::get(size_t index)
    {
        return CALL_CONST_OVERLOAD(Camera*, get, index);
    }

    const Camera* CameraSystem::get(size_t index) const
    {
        return (index < _cams.size()) ? _cams[index] : nullptr;
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

                _currentcam = i;
                i->apply(target);
                _numrendered += sys->render(target, i->getBBox());
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
}
