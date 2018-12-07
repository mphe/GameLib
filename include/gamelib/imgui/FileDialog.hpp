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

            auto open(const char* dir = nullptr) -> void;
            auto process() -> bool;
            auto getPath() const -> const char*;

        public:
            ImGuiFs::Dialog dialog;
            Type type;

        private:
            bool _isopen;
            const char* _workingdir;
    };
}

#endif
