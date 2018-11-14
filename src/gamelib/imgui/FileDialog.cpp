#include "gamelib/imgui/FileDialog.hpp"

namespace gamelib
{
    FileDialog::FileDialog() :
        FileDialog(Save)
    {}

    FileDialog::FileDialog(Type type_) :
        type(type_),
        _isopen(false)
    {}

    void FileDialog::open()
    {
        _isopen = true;
    }

    bool FileDialog::process()
    {
        // TODO: default keys

        if (type == Save)
        {
            if (strlen(dialog.saveFileDialog(_isopen, dialog.getLastDirectory())) > 0)
                return true;
        }
        else
        {
            if (strlen(dialog.chooseFileDialog(_isopen, dialog.getLastDirectory())) > 0)
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
