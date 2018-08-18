#ifndef EDITOR_SHARED_HPP
#define EDITOR_SHARED_HPP

#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class Editor;
    class SelectTool;
    class Entity;

    class EditorShared
    {
        friend class Editor;

        public:
            static auto getMouse()        -> const math::Point2f&;
            static auto getMouseSnapped() -> const math::Point2f&;

            static auto isSnapEnabled() -> bool;
            static auto snap(const math::Point2f& p) -> math::Point2f;

            static auto getSelectTool() -> SelectTool&;
            static auto getSelected()   -> Entity*;

        private:
            static Editor* _editor; // Set by Editor
    };
}

#endif
