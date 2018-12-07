#include "gamelib/imgui/FileDialog.hpp"

namespace gamelib
{
    FileDialog::FileDialog() :
        FileDialog(Save)
    {}

    FileDialog::FileDialog(Type type_) :
        type(type_),
        _isopen(false),
        _workingdir(nullptr)
    {}

    void FileDialog::open(const char* dir)
    {
        _isopen = true;
        _workingdir = dir;
    }

    bool FileDialog::process()
    {
        // TODO: default keys

        auto dir = _workingdir ? _workingdir : dialog.getLastDirectory();

        if (type == Save)
        {
            if (strlen(dialog.saveFileDialog(_isopen, dir)) > 0)
                return true;
        }
        else
        {
            if (strlen(dialog.chooseFileDialog(_isopen, dir)) > 0)
                return true;
        }

        _isopen = false; // has to be reset, otherwise the dialog will follow the mouse
        return false;
    }

    const char* FileDialog::getPath() const
    {
        return dialog.getChosenPath();
    }
}
