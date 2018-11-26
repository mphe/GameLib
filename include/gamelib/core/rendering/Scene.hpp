#ifndef GAMELIB_SCENE_HPP
#define GAMELIB_SCENE_HPP

#include <memory>>
#include <vector>
#include "SceneObject.hpp"
#include "Layer.hpp"
#include "gamelib/core/Camera.hpp"
#include "gamelib/utils/SlotMap.hpp"
#include "gamelib/core/Subsystem.hpp"

#warning "UPDATE SCENE DOCS"

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
 * render() afterwards.
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
    typedef std::unique_ptr<Camera> CameraPtr;

    class Scene : public SceneData, public Subsystem<Scene>
    {
        friend class SceneObject;
        friend class SceneData;

        public:
            constexpr static const char* name = "Scene";

        public:
            Scene();

            auto update(float elapsed) -> void;
            auto render(sf::RenderTarget& target) -> unsigned int;
            auto getNumObjectsRendered() const -> size_t;

            // Resets everything and removes all SceneObjects and Cameras
            auto destroy() -> void;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

            auto add(SceneObject* obj)    -> SceneObject*;
            auto remove(SceneObject* obj) -> void;

            auto addCamera(const std::string& name) -> Camera&;
            auto addCamera(CameraPtr cam)           -> Camera&;
            auto removeCamera(const Camera& cam)    -> void;
            auto getNumCameras() const              -> size_t;
            auto getCamera(size_t i)                -> Camera*;
            auto getCamera(size_t i) const          -> const Camera*;
            auto findCamera(const std::string& name)       -> Camera*;
            auto findCamera(const std::string& name) const -> const Camera*;

            // After the render loop ends, reset to view to this camera.
            // nullptr to use the RenderTarget's default view.
            auto setDefaultCamera(const Camera* cam) -> void;
            auto getDefaultCamera() const            -> const Camera*;

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
            auto _render(sf::RenderTarget& target) -> unsigned int;
            auto _updateQueue() -> void;

        private:
            size_t _numrendered;
            const Camera* _currentcam;
            const Camera* _default;
            std::vector<CameraPtr> _cams;
            SlotMapShort<Layer> _layers;
            std::vector<SceneObject*> _renderQueue;
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
