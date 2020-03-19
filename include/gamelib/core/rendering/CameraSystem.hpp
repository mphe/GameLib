#ifndef GAMELIB_CAMERASYSTEM_HPP
#define GAMELIB_CAMERASYSTEM_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include <string>
#include <memory>
#include <vector>
#include "gamelib/core/Subsystem.hpp"
#include "Camera.hpp"

namespace gamelib
{
    class RenderSystem;

    class CameraSystem : public Subsystem<CameraSystem>
    {
        public:
            ASSIGN_NAMETAG("CameraSystem");

        public:
            CameraSystem();

            auto add(Camera* cam)                   -> void;
            auto remove(const std::string& name)    -> void;
            auto remove(const Camera* cam)          -> void;
            auto remove(size_t i)                   -> void;
            auto get(size_t i)                      -> Camera*;
            auto get(size_t i) const                -> const Camera*;
            auto get(const std::string& name)       -> Camera*;
            auto get(const std::string& name) const -> const Camera*;
            auto size() const                       -> size_t;
            auto clear()                            -> void;

            // Returns the camera currently being processed when inside the render loop, otherwise nullptr.
            auto getCurrent() const -> const Camera*;

            auto update(float elapsed) -> void;

            auto render(sf::RenderTarget& target) const                                -> size_t;
            auto render(const RenderSystem* rendersys, sf::RenderTarget& target) const -> size_t;
            auto getNumRendered() const                                                -> size_t;

        private:
            mutable size_t _numrendered;
            mutable const Camera* _currentcam;
            std::vector<Camera*> _cams;
    };
}

#endif
