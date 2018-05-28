#ifndef EDITOR_ENTITY_DESIGNER_HPP
#define EDITOR_ENTITY_DESIGNER_HPP

#include "gamelib/core/ecs/Entity.hpp"
#include "imguifilesystem.h"
#include <string>

namespace gamelib
{
    class EntityDesigner
    {
        public:
            EntityDesigner();
            ~EntityDesigner();

            auto open()  -> void;
            auto close() -> void;
            auto draw()  -> void;

        private:
            Entity::Handle _ent;
            bool _open;
            ImGuiFs::Dialog loaddlg;
            ImGuiFs::Dialog savedlg;
            std::string currentFilePath;
    };
}

#endif
