#ifndef GAMELIB_SCENE_HPP
#define GAMELIB_SCENE_HPP

#include <memory>
#include <vector>
#include "gamelib/Updatable.hpp"
#include "gamelib/Renderable.hpp"
#include "gamelib/GameObject.hpp"
#include "gamelib/Camera.hpp"

/**
 * Scenes stores and updates Camera, Updatable and Renderable objects.
 *
 * It is itself a GameObject (aka Updatable and Renderable) and can therefore
 * be used in places where Updatables or Renderables are needed.
 * One could create a parallax effect by adding a Scene to a Scene and
 * adapting the camera position of the sub-Scene to the main Scene's camera.
 *
 * Cameras are stored in a vector whereas Updatables/Renderables are stored
 * as raw pointers (in a vector). That means Scene is not responsible for
 * freeing Updatable and Renderable objects.
 * Cameras, Updatables, and Renderables can be added and queried but not
 * removed (except when calling destroy()).
 * Also there is at least 1 camera by default.
 *
 * When calling update() it iterates over all Updatables and Cameras and
 * update()s them (from front to back (LIFO)).
 * When calling render() it iterates over all Renderables for each Camera
 * and render()s them (from back to front (FIFO)). The current camera inside
 * the render loop can be accessed using the getCurrentCamera() function.
 */

namespace gamelib
{
    class Scene : public GameObject
    {
        public:
            Scene();

            // Removes all Updatables, Renderables and cameras
            void destroy();

            void update(float elapsed);
            void render(sf::RenderTarget& surface);

            Renderable* addObject(Renderable* obj);
            Updatable* addObject(Updatable* obj);

            // Takes a GameObject and splits it up automatically
            GameObject* addObject(GameObject* obj);

            // Creates a new camera and returns a reference to it.
            Camera& addCamera();

            // Creates a new camera using its copy constructor.
            Camera& addCamera(const Camera& cam);

            // Returns the camera at index
            Camera& getCamera(size_t index = 0);
            const Camera& getCamera(size_t index = 0) const;

            size_t getCameraCount() const;

            // Returns the camera currently being processed when inside
            // the render loop, otherwise the first camera.
            Camera& getCurrentCamera();
            const Camera& getCurrentCamera() const;

        private:
            size_t _currentcam;
            std::vector<Updatable*> _update;
            std::vector<Renderable*> _render;
            std::vector<Camera> _cams;
    };
}

#endif
