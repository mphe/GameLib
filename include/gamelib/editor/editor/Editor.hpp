#ifndef EDITOR_MAINSTATE_HPP
#define EDITOR_MAINSTATE_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>
#include "gamelib/core/GameState.hpp"
#include "gamelib/core/FreeCam.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/geometry/CollisionSystem.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/update/UpdateSystem.hpp"
#include "ui/Grid.hpp"
#include "ui/LayerUI.hpp"


namespace gamelib
{
    class Game;
}

namespace gamelib
{
    class SelectTool;
    class Tool;

    class Editor : public GameState, public JsonSerializer
    {
        friend class EditorShared;

        public:
            enum Tools{
                ToolSelect,
                ToolBrush,
                ToolVertex,
                ToolEntity,
                NumTools
            };

        public:
            Editor();

            auto init(Game* game) -> bool;
            auto quit()                    -> void;

            auto update(float elapsed)            -> void;
            auto render(sf::RenderTarget& target) -> void;

            auto setTool(Tools tool) -> void;
            auto getSelectTool()     -> SelectTool&;

            auto writeToJson(Json::Value& node)        -> void;
            auto loadFromJson(const Json::Value& node) -> bool;

        private:
            static auto _eventCallback(Editor* me, EventPtr ev) -> void;
            auto _drawGui() -> void;
            auto _mapCoords(float x, float y) -> math::Point2f;
            auto _updateShared(float mx, float my) -> void;

        private:
            Game* _game;
            Scene _scene;
            CollisionSystem _colsys;
            ResourceManager _resmgr;
            EntityManager _entmgr;
            EntityFactory _entfactory;
            UpdateSystem _updatesystem;

            // Editor
            std::unique_ptr<Tool> _tools[NumTools];
            Tool* _currenttool;
            FreeCam _camctrl;
            math::Point2f _mouse;
            math::Point2f _mouseSnapped;
            Grid _grid;
            LayerUI _layerui;
            bool _snap;
            bool _drag;
            bool _gridOnTop;
            bool _run;
    };
}

#endif
