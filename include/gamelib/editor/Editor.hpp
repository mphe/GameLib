#ifndef EDITOR_MAINSTATE_HPP
#define EDITOR_MAINSTATE_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>
#include "gamelib/core/GameState.hpp"
#include "gamelib/core/rendering/FreeCam.hpp"
#include "gamelib/core/rendering/Camera.hpp"
#include "gamelib/core/event/EventHandle.hpp"
#include "ui/Grid.hpp"
#include "ui/ResourceViewer.hpp"
#include "gamelib/editor/ui/Overlay.hpp"

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
                ToolSprite,
                ToolBrush,
                ToolVertex,
                ToolEntity,
                NumTools
            };

            typedef void(*ExportFunction)(const std::string&);

        public:
            Editor();

            auto init(Game* game) -> bool final override;
            auto quit()           -> void final override;

            auto update(float elapsed)            -> void final override;
            auto render(sf::RenderTarget& target) -> void final override;

            auto setTool(Tools tool) -> void;
            auto getSelectTool()     -> SelectTool&;

            auto pause()   -> void;
            auto unpause() -> void;

            auto load()                         -> bool;
            auto load(const std::string& fname) -> bool;
            auto save()                         -> bool;
            auto save(const std::string& fname) -> bool;

            auto registerExportCallback(ExportFunction callback) -> void;

        private:
            auto _drawGui() -> void;
            auto _handleInput() -> void;
            auto _updateRunFlags() -> void;

        private:
            ExportFunction _exportcallback;
            std::string _savefile;

            std::unique_ptr<Tool> _tools[NumTools];
            Tool* _currenttool;

            Camera _cam;
            FreeCamController _camctrl;

            ResourceViewer _resviewer;
            Overlay _overlay;
            bool _entsearch;

            math::Point2f _mouseSnapped;
            bool _drag;

            Grid _grid;
            bool _snap;
            bool _gridontop;

            bool _run;
            bool _hidegui;

            EventHandle _evSelected;
    };
}

#endif
