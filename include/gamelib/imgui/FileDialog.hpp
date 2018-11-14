#ifndef GAMELIB_FILE_DIALOG_HPP
#define GAMELIB_FILE_DIALOG_HPP

#include "imguifilesystem.h"

namespace gamelib
{
    class FileDialog
    {
        public:
            enum Type
            {
                Save,
                Load
            };

        public:
            FileDialog();
            FileDialog(Type type_);

            auto open()          -> void;
            auto process()       -> bool;
            auto getPath() const -> const char*;

        public:
            ImGuiFs::Dialog dialog;
            Type type;

        private:
            bool _isopen;
    };
}

#endif
