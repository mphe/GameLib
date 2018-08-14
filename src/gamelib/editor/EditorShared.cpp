#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/Editor.hpp"
#include "gamelib/core/input/InputSystem.hpp"

namespace gamelib
{
    const math::Point2f& EditorShared::getMouse()
    {
        return InputSystem::getActive()->getMouse().world;
    }

    const math::Point2f& EditorShared::getMouseSnapped()
    {
        return _editor->_mouseSnapped;
    }

    math::Point2f EditorShared::snap(const math::Point2f& p)
    {
        if (_editor->_snap)
            return _editor->_grid.snap(p);
        return p;
    }

    bool EditorShared::isSnapEnabled()
    {
        return _editor->_snap;
    }

    SelectTool& EditorShared::getSelectTool()
    {
        return _editor->getSelectTool();
    }


    Editor* EditorShared::_editor = nullptr;
}
