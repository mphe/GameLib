#include "gamelib/editor/Editor.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/export.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/string.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/editor/tools/SpriteTool.hpp"
#include "gamelib/editor/tools/BrushTool.hpp"
#include "gamelib/editor/tools/VertexTool.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/editor/tools/EntityTool.hpp"
#include "gamelib/editor/ui/JsonView.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/imgui/FileDialog.hpp"
#include "gamelib/editor/ui/LayerUI.hpp"
#include "gamelib/editor/ui/CameraUI.hpp"
#include "gamelib/editor/ui/EntityList.hpp"
#include "gamelib/editor/ui/GlobalGameConfig.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "imgui.h"

namespace gamelib
{
    constexpr const char* buttonStrings[] = {
        "Select Tool",
        "Sprite Tool",
        "Brush Tool",
        "Vertex Tool",
        "Entity Tool"
    };

    // Strips PolygonBrushComponents from output
    void defaultExport(const std::string& fname);

    void hideRenderSystem();
    void showRenderSystem();

    void addRenderSystemFlags(unsigned int flags);
    void removeRenderSystemFlags(unsigned int flags);
    void toggleRenderSystemFlags(unsigned int flags);


    Editor::Editor() :
        GameState(gamestate_freeze),
        _exportcallback(defaultExport),
        _currenttool(nullptr),
        _cam("editorcam"),
        _camctrl(&_cam),
        _entsearch(false),
        _drag(false),
        _grid(32, 32),
        _snap(true),
        _gridontop(false),
        _run(false),
        _hidegui(true)
    { }

    bool Editor::init(Game* game)
    {
        LOG_DEBUG("Init Editor...");
        EditorShared::_editor = this;

        _cam.loadFromFile("assets/editorcam.json");

        _updateRunFlags();

        _tools[ToolSelect].reset(new SelectTool());
        _tools[ToolSprite].reset(new SpriteTool());
        _tools[ToolBrush].reset(new BrushTool());
        _tools[ToolVertex].reset(new VertexTool());
        _tools[ToolEntity].reset(new EntityTool());
        setTool(ToolBrush);

        return true;
    }

    void Editor::quit()
    {
        _resviewer.close();

        _sfmlEvent.unregister();

        _currenttool = nullptr;
        for (auto& i : _tools)
            i.reset();

        LOG_DEBUG("Editor unloaded");
    }

    void Editor::update(float elapsed)
    {
        auto input = getSubsystem<InputSystem>();
        _mouseSnapped = EditorShared::snap(input->getMouse().world);

        // Temporary set RenderSystem to visible, so that visible checks in SelectTool work
        if (!_run)
            showRenderSystem();

        if (input->isKeyPressed(sf::Keyboard::F5))
        {
            _run = !_run;
            _updateRunFlags();
        }

        _overlay.drawDebugOverlay();

        if (!_run || !_hidegui)
            _drawGui();

        if (!_run)
        {
            _handleInput();
            _camctrl.update(elapsed);
            hideRenderSystem();
        }
    }

    void Editor::render(sf::RenderTarget& target)
    {
        // if (_run && _hidegui)
        //     return;

        _cam.apply(target);

        if (!_run)
        {
            if (_snap && !_gridontop)
                _grid.render(target);

            auto rensys = getSubsystem<RenderSystem>();

            showRenderSystem();
            rensys->render(target, _cam.getCamRect());
            hideRenderSystem();

            _cam.apply(target);

            if (_snap && _gridontop)
                _grid.render(target);
        }
        else if (!_hidegui)
            _grid.render(target);

        // necessary, otherwise things will disappear randomly
        // getSubsystem<Game>()->getWindow().resetGLStates();

        if (!_run || !_hidegui)
        {
            if (_currenttool)
                _currenttool->render(target);

            _overlay.render(target);
        }
    }


    void Editor::setTool(Tools tool)
    {
        _currenttool = _tools[tool].get();
    }

    SelectTool& Editor::getSelectTool()
    {
        return *(SelectTool*)_tools[ToolSelect].get();
    }


    void Editor::pause()
    {
        _run = false;
        _updateRunFlags();
    }

    void Editor::unpause()
    {
        _run = true;
        _updateRunFlags();
    }


    bool Editor::load()
    {
        bool success = _savefile.empty() ? false : gamelib::loadSave(_savefile);
        setTool(ToolSelect);
        _updateRunFlags();
        return success;
    }

    bool Editor::load(const std::string& fname)
    {
        _savefile = fname;
        return load();
    }

    bool Editor::save()
    {
        return _savefile.empty() ? false : gamelib::save(_savefile);
    }

    bool Editor::save(const std::string& fname)
    {
        _savefile = fname;
        return this->save();
    }


    void Editor::registerExportCallback(ExportFunction callback)
    {
        _exportcallback = callback ? callback : defaultExport;
    }

    void Editor::_updateRunFlags()
    {
        if (_run)
        {
            RMFLAG(flags, gamestate_freeze);
            removeRenderSystemFlags(render_noparallax | render_drawhidden);
            showRenderSystem(); // show RenderSystem for ingame rendering
        }
        else
        {
            ADDFLAG(flags, gamestate_freeze);
            addRenderSystemFlags(render_noparallax | render_drawhidden);

            // Hack to prevent RenderSystem from rendering so the editor can render it manually with its own camera
            hideRenderSystem();
        }
    }


    void Editor::_drawGui()
    {
        static bool testwindow = false;
        static bool jsonwindow = false;
        static bool toolbox = true;
        static bool layerbox = false;
        static bool cambox = false;
        static bool entprops = true;
        static bool entlist = true;
        static bool entfac = false;
        static bool gamecfg = false;
        static FileDialog loaddlg(FileDialog::Load);
        static FileDialog savedlg;
        static FileDialog exportdlg;

        auto selected = getSelectTool().getSelected();
        auto input = getSubsystem<InputSystem>();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", nullptr, nullptr, !_savefile.empty()))
                    save();

                if (ImGui::MenuItem("Save as"))
                    savedlg.open();

                if (ImGui::MenuItem("Load"))
                    loaddlg.open();

                if (ImGui::MenuItem("Reload"))
                    load();

                if (ImGui::MenuItem("Export"))
                    exportdlg.open();

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
                if (ImGui::BeginMenu("Selected entity"))
                {
                    ImGui::MenuItem("Show solids", nullptr, &_overlay.renderSolid);
                    ImGui::MenuItem("Show solid normals", nullptr, &_overlay.renderNormals);
                    ImGui::MenuItem("Show velocity", nullptr, &_overlay.renderVel);
                    ImGui::EndMenu();
                }

                ImGui::MenuItem("Show mouse coordinates", nullptr, &_overlay.showCoords);
                ImGui::MenuItem("Show cameras", nullptr, &_overlay.renderCams);

                auto rensys = getSubsystem<RenderSystem>();
                const auto& renflags = rensys->getRootOptions().flags;

                if (ImGui::MenuItem("Show hidden", nullptr, renflags & render_drawhidden))
                    toggleRenderSystemFlags(render_drawhidden);

                if (ImGui::MenuItem("Enable parallax", nullptr, !(renflags & render_noparallax)))
                    toggleRenderSystemFlags(render_noparallax);

                ImGui::MenuItem("Hide Editor when running", nullptr, &_hidegui);

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::MenuItem("Toolbox", nullptr, &toolbox);
                ImGui::MenuItem("Entity properties", nullptr, &entprops);
                ImGui::MenuItem("Entity list", nullptr, &entlist);
                ImGui::MenuItem("Layer settings", nullptr, &layerbox);
                ImGui::MenuItem("Camera settings", nullptr, &cambox);
                ImGui::MenuItem("Global game config", nullptr, &gamecfg);
                ImGui::Separator();
                ImGui::MenuItem("Json viewer", nullptr, &jsonwindow);
                ImGui::MenuItem("Entity Factory", nullptr, &entfac);
                if (ImGui::MenuItem("Resource Manager"))
                    _resviewer.open();
                ImGui::Separator();
                ImGui::MenuItem("Test window", nullptr, &testwindow);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Grid"))
            {
                ImGui::MenuItem("Snap to grid", "G", &_snap);
                ImGui::MenuItem("Grid on top", nullptr, &_gridontop);
                if (ImGui::MenuItem("Increase", "Q"))
                    _grid.increase();
                if (ImGui::MenuItem("Decrease", "E"))
                    _grid.decrease();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        { // Loading / Saving
            if (loaddlg.process())
                load(loaddlg.getPath());

            if (savedlg.process())
                this->save(savedlg.getPath());

            if (exportdlg.process())
                _exportcallback(exportdlg.getPath());
        }

        if (ImGui::BeginPopupContextVoid("Context Menu", sf::Mouse::Right))
        {
            if (ImGui::Selectable("Move entity here") && selected)
                selected->getTransform().setPosition(input->getMouse().world);
            ImGui::EndPopup();
        }

        { // Dialogues
            if (testwindow)
                ImGui::ShowDemoWindow(&testwindow);

            if (jsonwindow)
                drawJsonView(&jsonwindow);

            if (layerbox)
                drawLayerUI(&layerbox);

            if (cambox)
                drawCameraUI(&cambox);

            if (entlist)
                drawEntityList(&entlist);

            if (entfac)
                drawEntityFactory(&entfac);

            if (_entsearch)
                drawSearchPopup(&_entsearch);

            if (gamecfg)
                drawGameConfig(&gamecfg);

            if (entprops)
                drawEntityProperties(&entprops);

            _resviewer.draw();
            _overlay.drawGui();

            if (toolbox)
            {
                if (ImGui::Begin("Toolbox", &toolbox, ImVec2(250, 125)))
                {
                    for (size_t i = 0; i < NumTools; ++i)
                        if (ImGui::Button(buttonStrings[i]))
                            setTool(static_cast<Tools>(i));
                    ImGui::Separator();

                    _currenttool->drawGui();
                }
                ImGui::End();
            }
        }
    }

    void Editor::_handleInput()
    {
        auto input = getSubsystem<InputSystem>();
        Tool* realtool = _currenttool;  // backup

        { // Keyboard
            if (input->isKeyDown(sf::Keyboard::LControl))
            {
                _currenttool = &getSelectTool();

                if (input->isKeyPressed(sf::Keyboard::S))
                    save();
                else if (input->isKeyPressed(sf::Keyboard::R))
                {
                    getSelectTool().select(nullptr);
                    load();
                }
            }

            if (input->isKeyPressed(sf::Keyboard::F))
                _entsearch = true;

            if (input->isKeyPressed(sf::Keyboard::Delete) && getSelectTool().getSelected())
            {
                getSelectTool().getSelected()->destroy();
                getSelectTool().select(nullptr);
            }

            if (input->isKeyPressed(sf::Keyboard::Q))
                _grid.increase();

            if (input->isKeyPressed(sf::Keyboard::E))
                _grid.decrease();

            if (input->isKeyPressed(sf::Keyboard::G))
                _snap = !_snap;

            if (input->isKeyPressed(sf::Keyboard::Z))
            {
                auto selected = getSelectTool().getSelected();
                if (selected)
                {
                    _cam.center(selected->getTransform().getPosition());
                    _cam.zoom = 1;
                }
            }
        }

        { // Mouse
            auto& mouse = input->getMouse();

            if (!input->isMouseConsumed())
            {
                if (mouse.wheel)
                    _cam.zoomTowards(mouse.world.x, mouse.world.y, mouse.wheel / -10.0);
            }

            if (input->getMouse().moved)
            {
                if (_drag)
                    _currenttool->onDrag();
                else
                    _currenttool->onMouseMove();
            }

            if (input->isMousePressed(sf::Mouse::Left))
            {
                _currenttool->onMousePressed();
                _drag = true;
            }
            else if (_drag && !input->isMouseDown(sf::Mouse::Left))
                // Use !mouseDown because if mouse is consumed, mouseReleased will never be true
            {
                _currenttool->onMouseRelease();
                _drag = false;
            }
        }

        _currenttool = realtool;
    }

    void defaultExport(const std::string& fname)
    {
        // TODO: strip PolygonBrushComponent
        // TODO: export as entity
        save(fname);
        LOG("Map exported to ", fname);
    }

    void hideRenderSystem()
    {
        addRenderSystemFlags(render_invisible);
    }

    void showRenderSystem()
    {
        removeRenderSystemFlags(render_invisible);
    }

    void addRenderSystemFlags(unsigned int flags)
    {
        auto rensys = getSubsystem<RenderSystem>();
        auto renflags = rensys->getRootOptions().flags;
        ADDFLAG(renflags, flags);
        rensys->setRootOptions(&renflags);
    }

    void removeRenderSystemFlags(unsigned int flags)
    {
        auto rensys = getSubsystem<RenderSystem>();
        auto renflags = rensys->getRootOptions().flags;
        RMFLAG(renflags, flags);
        rensys->setRootOptions(&renflags);
    }

    void toggleRenderSystemFlags(unsigned int flags)
    {
        auto rensys = getSubsystem<RenderSystem>();
        auto renflags = rensys->getRootOptions().flags;
        TOGGLEFLAG(renflags, flags);
        rensys->setRootOptions(&renflags);
    }
}
