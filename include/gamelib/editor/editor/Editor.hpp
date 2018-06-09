#ifndef EDITOR_MAINSTATE_HPP
#define EDITOR_MAINSTATE_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>
#include "gamelib/core/GameState.hpp"
#include "gamelib/core/FreeCam.hpp"
#include "gamelib/core/event/Event.hpp"
#include "ui/Grid.hpp"
#include "ui/LayerUI.hpp"
#include "EntityDesigner.hpp"
#include "ui/ResourceViewer.hpp"

namespace gamelib
{
    class SelectTool;
    class Tool;

    class Editor : public GameState
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

            typedef void(*ExportFunction)(const std::string&);

        public:
            Editor();

            auto init(Game* game) -> bool;
            auto quit()                    -> void;

            auto update(float elapsed)            -> void;
            auto render(sf::RenderTarget& target) -> void;

            auto setTool(Tools tool) -> void;
            auto getSelectTool()     -> SelectTool&;

            auto registerExportCallback(ExportFunction callback) -> void;

        private:
            static auto _eventCallback(Editor* me, EventPtr ev) -> void;
            auto _drawGui() -> void;
            auto _mapCoords(float x, float y) -> math::Point2f;
            auto _updateMouse(float mx, float my) -> void;
            auto _updateRunFlags() -> void;

        private:
            ExportFunction _exportcallback;
            std::unique_ptr<Tool> _tools[NumTools];
            Tool* _currenttool;
            FreeCam _camctrl;
            math::Point2f _mouse;
            math::Point2f _mouseSnapped;
            Grid _grid;
            LayerUI _layerui;
            EntityDesigner _entdesigner;
            ResourceViewer _resviewer;
            bool _snap;
            bool _drag;
            bool _gridOnTop;
            bool _run;
    };
}

#endif
