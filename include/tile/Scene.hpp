#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <string>
#include <vector>
#include "Tile.hpp"
#include "math/geometry/Vector2.hpp"

namespace sf
{
    class RenderTarget;
}

/*
Map update order: back to front
Map render order: front to back
*/

namespace engine
{
    class Camera;
    class Layer;

    typedef std::unique_ptr<Camera> CameraPtr;
    typedef std::unique_ptr<Layer> LayerPtr;

    class Scene
    {
        public:
            //bool LoadFromFile(const std::string& fname);
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

            // const Tile* getTile(int x, int y) const;
            // const Tile* getTile(int x, int y, TileID id) const;

        private:
            std::vector<LayerPtr> _layers;
            std::vector<CameraPtr> _cams;
    };
}

/*
Config file structure:

#cams
cams: cam1, <camname>
cam1: <path to cam config>
<camname>: <path to cam config>

#maps
back: bgmap1, <mapname>
middle: mmap1, <mapname>
front: fgmap1, <mapname>

#bgmap1
file: <path to map config>
depth: <depth>
paradepth: <parallax depth>
offsetx: <parallax x offset>
offsety: <parallax y offset>

#<mapname>
...

*/

#endif
