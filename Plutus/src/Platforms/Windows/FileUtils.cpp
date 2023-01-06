#include "FileUtils.h"

#include <windows.h>
#include <filesystem>
#include <unordered_map>

namespace Plutus
{
    bool windowDialog(int mode, std::string& path, const std::string& title)
    {
        OPENFILENAME ofn;
        char szFileName[MAX_PATH];
        char szFileTitle[MAX_PATH];

        *szFileName = 0;
        *szFileTitle = 0;

        /* fill in non-variant fields of OPENFILENAME struct. */
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = GetActiveWindow();
        ofn.lpstrFilter = "Projects (*.json*)\0";
        ofn.lpstrCustomFilter = NULL;
        ofn.nMaxCustFilter = 0;
        ofn.nFilterIndex = 0;
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrInitialDir = "."; // Initial directory.
        ofn.lpstrFileTitle = szFileTitle;
        ofn.nMaxFileTitle = MAX_PATH;
        ofn.lpstrTitle = title.c_str();
        ofn.lpstrDefExt = "json";

        ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        bool success = false;
        TCHAR currentDir[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, currentDir);

        switch (mode) {
        case 0: {
            break;
        }
        case 1: {
            break;
        }
        case 2: {
            break;
        }
        }

        if (mode)
        {
            success = GetOpenFileName((LPOPENFILENAME)&ofn);
        }
        else
        {
            success = GetSaveFileName((LPOPENFILENAME)&ofn);
        }

        if (success)
        {
            path = szFileName;
        }
        SetCurrentDirectory(currentDir);
        return success;
    }

    bool copyFile(const std::string& from, const std::string& to)
    {
        if (std::filesystem::exists(from))
        {
            auto name = std::filesystem::path(from).filename().string();
            return std::filesystem::copy_file(from, "assets/textures/" + name);
        }
        return false;
    }
} // namespace Plutus
