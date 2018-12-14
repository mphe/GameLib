#ifndef GAMELIB_SCENE_HPP
#define GAMELIB_SCENE_HPP

#include <vector>
#include <memory>
#include "SceneObject.hpp"
#include "Layer.hpp"
#include "Camera.hpp"
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/Subsystem.hpp"

/*
 * Scenes are responsible for rendering SceneObjects and keeping track of
 * Cameras.
 *
 * The scene will be rendered for each camera, while ignoring objects outside
 * the camera rect.
 * TODO: Do optimization like quadtrees.
 *
 * Cameras and SceneObjects are both stored as raw pointers (in a vector).
 * That means Scene does not take ownership of them, but only the references.
 *
 * If no camera was added, the scene will be rendered with the RenderTarget's
 * current view.
 * Adding a SceneObject or changing depth requires a render() call for the
 * object to be placed at its correct position in the render queue (sorted by depth).
 * That means, it's fast to add multiple elements at once and then call
 * render() afterwards.
 *
 * When calling update() it iterates over all Cameras and update()s them
 * (from front to back (LIFO)).
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

        typedef std::unique_ptr<Camera> CameraPtr;

        public:
            constexpr static const char* name = "Scene";

        public:
            Scene();

            auto update(float elapsed) -> void;
            auto render(sf::RenderTarget& target) -> unsigned int;
            auto getNumObjectsRendered() const -> size_t;

            // Renders the scene only for the current view, not for all cameras
            auto renderDirect(sf::RenderTarget& target) -> unsigned int;

            // Resets everything and removes all SceneObjects and Cameras
            auto destroy() -> void;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node) const  -> void;

            auto add(SceneObject* obj)    -> SceneObject*;
            auto remove(SceneObject* obj) -> void;

            auto createCamera(const std::string& name) -> Camera&;
            auto removeCamera(const Camera* cam)       -> void;
            auto getCamera(size_t i)                   -> Camera*;
            auto getCamera(size_t i) const             -> const Camera*;
            auto getNumCameras() const                 -> size_t;
            auto findCamera(const std::string& name)       -> Camera*;
            auto findCamera(const std::string& name) const -> const Camera*;

            // After the render loop ends, reset to view to this camera.
            // nullptr to use the RenderTarget's default view.
            auto setDefaultCamera(const Camera* cam) -> void;

            // Returns the camera currently being processed when inside the render loop, otherwise nullptr.
            auto getCurrentCamera() const -> const Camera*;

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
            auto _updateQueue() -> void;

        private:
            size_t _numrendered;
            const Camera* _currentcam;
            const Camera* _default;
            SlotMapShort<Layer> _layers;
            std::vector<SceneObject*> _renderQueue;
            std::vector<CameraPtr> _cams;
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
 *     "cameras": {
 *         "<cameraname>": "path/to/camera.json",
 *         "<cameraname>": {
 *             <cameraconfig>
 *         },
 *         ...
 *     }
 * }
 */

#endif
