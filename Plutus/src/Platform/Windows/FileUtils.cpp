#include "FileUtils.h"

#include <windows.h>

namespace Plutus
{
    bool windowDialog(int mode, std::string& path)
    {
        OPENFILENAME ofn;
        char* FilterSpec = "All Files(*.*)\0*.*\0";
        char* Title = "Open....";
        char szFileName[MAX_PATH];
        char szFileTitle[MAX_PATH];

        *szFileName = 0;
        *szFileTitle = 0;

        /* fill in non-variant fields of OPENFILENAME struct. */
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = GetActiveWindow();
        ofn.lpstrFilter = FilterSpec;
        ofn.lpstrCustomFilter = NULL;
        ofn.nMaxCustFilter = 0;
        ofn.nFilterIndex = 0;
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrInitialDir = "."; // Initial directory.
        ofn.lpstrFileTitle = szFileTitle;
        ofn.nMaxFileTitle = MAX_PATH;
        ofn.lpstrTitle = Title;
        ofn.lpstrDefExt = "json";

        ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        bool success = false;

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

        return success;
    }
} // namespace Plutus
