#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>
#include "GameObject.hpp"
#include "Camera.hpp"

namespace sf
{
    class RenderTarget;
}

namespace gamelib
{
    class Camera;

    typedef std::unique_ptr<Camera> CameraPtr;
    typedef std::unique_ptr<GameObject> GameObjectPtr;

    class Scene : public GameObject
    {
        public:
            Scene();
            void destroy();

            void update(float fps);
            void render(sf::RenderTarget& surface) const;

            GameObject& addObject(GameObjectPtr obj);

            Camera& addCamera(CameraPtr cam);
            Camera& getCamera(size_t index = 0) const;
            size_t getCameraCount() const;

            // Returns the camera currently being processed when inside the render loop, otherwise the first camera.
            // If no cameras have been added it will return NULL.
            Camera* getCurrentCamera() const;

        private:
            mutable Camera* _currentcam;
            std::vector<GameObjectPtr> _objs;
            std::vector<CameraPtr> _cams;
    };
}

#endif
