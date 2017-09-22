#include "gamelib/editor/editor/Editor.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/export.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/string.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/events/SFMLEvent.hpp"
#include "editor/editor/tools/BrushTool.hpp"
#include "editor/editor/tools/VertexTool.hpp"
#include "editor/editor/tools/SelectTool.hpp"
#include "editor/editor/tools/EntityTool.hpp"
#include "editor/editor/ui/JsonView.hpp"
#include "editor/editor/ui/props.hpp"
#include "editor/editor/EditorShared.hpp"
#include "editor/components/BrushComponent.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguifilesystem.h"

// TODO: block key pressed when they are consumed by imgui

namespace gamelib
{
    constexpr const char* buttonStrings[] = {
        "Select Tool",
        "Polygon Tool",
        "Vertex Tool",
        "Entity Tool"
    };

    // Strips BrushComponents from output
    void defaultExport(const std::string& fname);


    Editor::Editor() :
        GameState(gamestate_freeze),
        _exportcallback(defaultExport),
        _currenttool(nullptr),
        _camctrl(getSubsystem<Scene>()),
        _grid(32, 32),
        _snap(true),
        _drag(false),
        _gridOnTop(true),
        _run(false)
    { }

    bool Editor::init(Game* game)
    {
        LOG_DEBUG("Init Editor...");
        EditorShared::_editor = this;

        ImGui::SFML::Init(game->getWindow());

        auto evmgr = getSubsystem<EventManager>();
        evmgr->regCallback(SFMLEvent::id, _eventCallback, this);

        _updateRunFlags();

        _tools[ToolSelect].reset(new SelectTool());
        _tools[ToolBrush].reset(new BrushTool());
        _tools[ToolVertex].reset(new VertexTool());
        _tools[ToolEntity].reset(new EntityTool());
        setTool(ToolBrush);

        _layerui.refresh();

        return true;
    }

    void Editor::quit()
    {
        auto evmgr = getSubsystem<EventManager>();
        evmgr->unregCallback(SFMLEvent::id, _eventCallback, this);

        _currenttool = nullptr;
        for (auto& i : _tools)
            i.reset();

        ImGui::SFML::Shutdown();
        LOG_DEBUG("Editor unloaded");
    }

    void Editor::update(float elapsed)
    {
        auto game = getSubsystem<Game>();

        if (!_run)
            _camctrl.update(elapsed);

        ImGui::SFML::Update(game->getWindow(), sf::seconds(elapsed));
        _drawGui();
    }

    void Editor::render(sf::RenderTarget& target)
    {
        if (!_run)
        {
            // necessary, otherwise things will disappear randomly
            getSubsystem<Game>()->getWindow().resetGLStates();

            // if (!_gridOnTop && _snap)
            //     _grid.render(target);

            if (_gridOnTop && _snap)
                _grid.render(target);

            if (_currenttool)
                _currenttool->render(target);

            // if (_currenttool != &getSelectTool())
            //     getSelectTool().render(target);

            ImGui::Render();
        }
    }

    void Editor::setTool(Tools tool)
    {
        _currenttool = _tools[tool].get();
        LOG("Selected tool: ", buttonStrings[static_cast<size_t>(tool)]);
    }

    SelectTool& Editor::getSelectTool()
    {
        return *(SelectTool*)_tools[ToolSelect].get();
    }

    void Editor::registerExportCallback(ExportFunction callback)
    {
        if (callback == nullptr)
            _exportcallback = defaultExport;
        else
            _exportcallback = callback;
    }

    void Editor::_updateRunFlags()
    {
        auto scene = getSubsystem<Scene>();
        if (_run)
        {
            RMFLAG(scene->flags, render_noparallax | render_drawhidden);
            RMFLAG(flags, gamestate_freeze);
        }
        else
        {
            scene->flags |= render_noparallax | render_drawhidden;
            flags |= gamestate_freeze;
        }
    }

    void Editor::_eventCallback(Editor* me, EventPtr ev)
    {
        sf::Event& sfev = ev->get<SFMLEvent>()->ev;
        ImGui::SFML::ProcessEvent(sfev);
        bool consumed = ImGui::GetIO().WantCaptureMouse;
        auto game = getSubsystem<Game>();

        Tool* realtool = me->_currenttool;  // backup
        if (game->isKeyPressed(sf::Keyboard::LControl))
            me->_currenttool = &me->getSelectTool();

        if (sfev.type == sf::Event::KeyPressed && sfev.key.code == sf::Keyboard::F5)
        {
            me->_run = !me->_run;
            me->_updateRunFlags();
        }

        if (me->_run)
            return;

        switch (sfev.type)
        {
            case sf::Event::KeyPressed:
                if (ImGui::GetIO().WantCaptureKeyboard)
                    break;

                switch (sfev.key.code)
                {
                    case sf::Keyboard::R:
                        LOG_WARN("---------------- Reloading...");
                        game->reload();
                        me->quit();
                        me->init(game);
                        LOG("---------------- Reloading complete");
                        return; // early out, so that _currenttool doesn't get reset at the end of the function

                    // case sf::Keyboard::Q:
                    //     game->close();
                    //     break;

                    case sf::Keyboard::Delete:
                        if (me->getSelectTool().getSelected())
                        {
                            me->getSelectTool().getSelected()->destroy();
                            me->getSelectTool().select(nullptr);
                        }
                        break;

                    case sf::Keyboard::Q:
                        me->_grid.increase();
                        break;

                    case sf::Keyboard::E:
                        me->_grid.decrease();
                        break;

                    case sf::Keyboard::G:
                        me->_snap = !me->_snap;
                        break;
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (!consumed && sfev.mouseButton.button == sf::Mouse::Left)
                {
                    me->_updateMouse(sfev.mouseButton.x, sfev.mouseButton.y);
                    me->_currenttool->onMousePressed();
                    me->_drag = true;
                }
                break;

            case sf::Event::MouseMoved:
                {
                    me->_updateMouse(sfev.mouseMove.x, sfev.mouseMove.y);
                    if (me->_drag)
                        me->_currenttool->onDrag();
                    else
                        me->_currenttool->onMouseMove();
                }
                break;

            case sf::Event::MouseButtonReleased:
                {
                    me->_updateMouse(sfev.mouseButton.x, sfev.mouseButton.y);
                    me->_drag = false;
                    me->_currenttool->onMouseRelease();
                }
                break;

            case sf::Event::MouseWheelMoved:
                if (!consumed)
                {
                    auto p = me->_mapCoords(sfev.mouseWheel.x, sfev.mouseWheel.y);
                    getSubsystem<Scene>()->getCamera(0)->zoomTowards(p.x, p.y, sfev.mouseWheel.delta / -10.0);
                }
                break;
        }

        me->_currenttool = realtool;
    }

    void Editor::_drawGui()
    {
        static bool testwindow = false;
        static bool jsonwindow = false;
        static ImGuiFs::Dialog loaddlg;
        static ImGuiFs::Dialog savedlg;
        static ImGuiFs::Dialog exportdlg;
        static std::string currentFilePath;

        bool chooseload = false;
        bool choosesave = false;
        bool chooseexport = false;
        const char* chosenPath;

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    if (currentFilePath.empty())
                        choosesave = true;
                    else
                        saveState(currentFilePath);
                }

                if (ImGui::MenuItem("Save as"))
                    choosesave = true;

                if (ImGui::MenuItem("Load"))
                    chooseload = true;

                if (ImGui::MenuItem("Export"))
                    chooseexport = true;

                if (ImGui::MenuItem("Quit"))
                    getSubsystem<Game>()->close();

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Run", "F5", &_run))
                    _updateRunFlags();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Show test window", nullptr, &testwindow);
                ImGui::MenuItem("Show json window", nullptr, &jsonwindow);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Grid"))
            {
                ImGui::MenuItem("Snap to grid", nullptr, &_snap);
                // ImGui::MenuItem("Grid on top", nullptr, &_gridOnTop);
                if (ImGui::MenuItem("Increase"))
                    _grid.increase();
                if (ImGui::MenuItem("Decrease"))
                    _grid.decrease();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (testwindow)
            ImGui::ShowTestWindow(&testwindow);

        if (jsonwindow)
            drawJsonView(&jsonwindow);

        chosenPath = loaddlg.chooseFileDialog(chooseload);
        if (strlen(chosenPath) > 0)
        {
            currentFilePath = chosenPath;
            loadState(currentFilePath);
        }

        chosenPath = savedlg.saveFileDialog(choosesave);
        if (strlen(chosenPath) > 0)
        {
            currentFilePath = chosenPath;
            saveState(currentFilePath);
        }

        chosenPath = exportdlg.saveFileDialog(chooseexport);
        if (strlen(chosenPath) > 0)
            _exportcallback(chosenPath);

        ImGui::Begin("Toolbox", nullptr, ImVec2(250, 125));
        for (size_t i = 0; i < NumTools; ++i)
            if (ImGui::Button(buttonStrings[i]))
                setTool(static_cast<Tools>(i));
        ImGui::End();

        ImGui::Begin("Tool properties", nullptr, ImVec2(250, 275));
        _currenttool->drawGui();
        ImGui::End();

        ImGui::Begin("Layer Properties", nullptr, ImVec2(250, 285));
        _layerui.drawLayerUI();
        ImGui::End();

        if (getSelectTool().getSelected())
        {
            ImGui::Begin("Entity properties", nullptr, ImVec2(256, 690));
            drawEntityProps(*getSelectTool().getSelected());
            ImGui::End();
        }
    }

    math::Point2f Editor::_mapCoords(float x, float y)
    {
        auto p = getSubsystem<Game>()->getWindow().mapPixelToCoords(sf::Vector2i(x, y));
        return math::Point2f(p.x, p.y);
    }

    void Editor::_updateMouse(float mx, float my)
    {
        _mouse = _mapCoords(mx, my);
        _mouseSnapped = EditorShared::snap(_mouse);
    }



    void defaultExport(const std::string& fname)
    {
        saveState(fname, [](Json::Value& node, Entity& ent) {
                ent.writeToJson(node, [](Json::Value& compnode, Component& comp) {
                        if (comp.getID() != BrushComponent::id)
                            comp.writeToJson(compnode);
                    });
            });
        LOG("Map saved to ", fname);
    }
}
