#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

namespace sf
{
    class RenderTarget;
}

namespace engine
{
    class Camera;
    class Layer;

    typedef std::unique_ptr<Camera> CameraPtr;
    typedef std::unique_ptr<Layer> LayerPtr;

    class Scene
    {
        public:
            void destroy();

            void update(float fps);
            void render(sf::RenderTarget& surface) const;

            Camera& addCamera(CameraPtr cam);
            Camera& getCamera(size_t index) const;
            size_t getCameraCount() const;

            // Add a new layer. If layer is null, a new empty layer will be added.
            Layer& addLayer(LayerPtr layer = nullptr);

            // Returns last added map
            Layer& getLayer() const;
            Layer& getLayer(size_t index) const;
            size_t getLayerCount() const;

        private:
            std::vector<LayerPtr> _layers;
            std::vector<CameraPtr> _cams;
    };
}

#endif
