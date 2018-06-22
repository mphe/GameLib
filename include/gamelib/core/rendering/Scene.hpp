#ifndef GAMELIB_SCENE_HPP
#define GAMELIB_SCENE_HPP

#include <vector>
#include "SceneObject.hpp"
#include "Layer.hpp"
#include "gamelib/core/Camera.hpp"
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/Subsystem.hpp"

/*
 * Scenes are responsible for rendering SceneObjects and keeping track of
 * Cameras.
 *
 * The scene will be rendered for each camera completely.
 * There's no optimization currently.
 * TODO: Do optimization like quadtrees.
 *
 * Cameras are stored as copies in a vector whereas SceneObjects are stored
 * as raw pointers (in a vector). That means Scene does not take ownership of
 * SceneObjects.
 * If no camera was added, the scene will be rendered with the RenderTarget's
 * current view.
 * Adding a SceneObject requires a render() call for the object being placed
 * at its correct position in the render queue (according to depth).
 * That means, it's fast to add multiple elements at once and then call
 * update() afterwards.
 *
 * When calling update() it iterates over all Cameras and update()s them
 * (from front to back (LIFO)). Also, in case of new elements being added
 * or depths changed, the SceneObject list will be sorted.
 *
 * When calling render() it iterates over all SceneObjects for each Camera
 * and render()s them in order of depth. That means, objects with a high depth
 * appear in the back, while objects with small depths appear in the front.
 * The currently processed camera inside the render loop can be accessed
 * using the getCurrentCamera() function.
 *
 * TODO: document layers
 */

namespace gamelib
{
    class Scene : public SceneData, public Subsystem<Scene>
    {
        friend class SceneObject;
        friend class SceneData;

        public:
            constexpr static const char* name = "Scene";

        public:
            Scene();

            // Removes all Renderables and cameras
            auto destroy() -> void;

            auto update(float elapsed) -> void;
            auto render(sf::RenderTarget& target) -> unsigned int;

            auto add(SceneObject* obj)    -> SceneObject*;
            auto remove(SceneObject* obj) -> void;

            // Creates a new camera and returns a reference to it.
            auto addCamera()                  -> Camera&;
            auto addCamera(const Camera& cam) -> Camera&;

            // After the render loop ends, reset to view to this camera.
            // -1 to use the RenderTarget's default view.
            auto setDefaultCamera(size_t index) -> void;

            // Returns the camera at index.
            // If no camera exists, nullptr is returned
            auto getCamera(size_t index)       -> Camera*;
            auto getCamera(size_t index) const -> const Camera*;

            // Returns the camera currently being processed when inside
            // the render loop, otherwise nullptr.
            auto getCurrentCamera()       -> Camera*;
            auto getCurrentCamera() const -> const Camera*;

            auto getCameraCount() const -> size_t;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

            // Creates a new layer or returns an existing one
            auto createLayer(const std::string& name)    -> Layer::Handle;
            auto deleteLayer(Layer::Handle handle)       -> void;
            auto getLayer(Layer::Handle handle) const    -> const Layer*;
            auto getLayer(Layer::Handle handle)          -> Layer*;
            auto getLayer(const std::string& name) const -> Layer::Handle;

            // Calls a function for each layer.
            // Signature: void (Handle, Layer&)
            template <typename F>
            void foreachLayer(F f)
            {
                for (auto it = _layers.begin(), end = _layers.end(); it != end; ++it)
                    f(it.handle(), *it);
            }

        private:
            auto _render(sf::RenderTarget& surface, const sf::View& view) -> unsigned int;
            auto _updateQueue() -> void;

        private:
            size_t _currentcam;
            size_t _default;
            SlotMapShort<Layer> _layers;
            std::vector<SceneObject*> _renderQueue;
            std::vector<Camera> _cams;
            bool _dirty;
    };
}

/*
 * Config file format
 * {
 *     "layers": {
 *         "<layername>": {
 *             <scenedata>
 *         },
 *         ...
 *     },
 *     "cameras": [
 *         "path/to/camera.json",
 *         {
 *             <cameraconfig>
 *         },
 *         ...
 *     ]
 * }
 */

#endif
