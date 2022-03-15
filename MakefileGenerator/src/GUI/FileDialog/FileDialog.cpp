#ifdef USING_IMGUI
#include <string>

#include "nfd/nfd.h"

#include "FileDialog.h"
#include "FileHandler.h"

#include "Debug.h"

namespace FileDialog {

    bool MultiFileDialog(std::string* base_str)
    {
        nfdpathset_t pathSet;
        nfdresult_t result = NFD_OpenDialogMultiple("", base_str->c_str(), &pathSet);
        if (result == NFD_OKAY)
        {
            for (size_t i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i)
            {
                nfdchar_t* path = NFD_PathSet_GetPath(&pathSet, i);
                FH::AddEntry(FH::GetRelativePath(base_str, path));
            }
            NFD_PathSet_Free(&pathSet);
            return true;
        }
        // pressed cancel
        else if (result == NFD_CANCEL)
        {
            DEBUG_PRINT_NL("[NFD][INFO] User pressed cancel (MultiFileDialog)")
            return false;
        }
        // error opening the file
        else
        {
            DEBUG_PRINT_NL("[NFD][Error] Error opening mulitple file dialog " << NFD_GetError())
            return false;
        }
    }


    bool FolderSelectionDialog(std::string* base_str, std::string* str, bool single_str)
    {
        nfdchar_t* outPath = NULL;
        nfdresult_t result = NFD_PickFolder(NULL, &outPath);
        if (result == NFD_OKAY)
        {
            if (single_str)
            {
                if (base_str == nullptr)
                    *str = outPath;
                else
                    if (*base_str != outPath)
                        *str = FH::GetRelativePath(base_str, outPath);
            }
            else
            {
                if (*base_str != outPath)
                {
                    str->append(FH::GetRelativePath(base_str, outPath));
                    str->append(";");
                }
            }

            free(outPath);
            return true;
        }
        // pressed cancel
        else if (result == NFD_CANCEL)
        {
            DEBUG_PRINT_NL("[NFD][INFO] User pressed cancel (FolderSelectionDialog)")
            return false;
        }
        // error opening the file
        else
        {
            DEBUG_PRINT_NL("[NFD][Error] Error opening pick folder dialog " << NFD_GetError())
            return false;
        }
    }


    std::string SaveFileDialog()
    {
        nfdchar_t* savePath = NULL;
        nfdresult_t result = NFD_SaveDialog("mg", NULL, &savePath);
        if (result == NFD_OKAY)
        {
            std::string filePath = savePath;
            free(savePath);
            return filePath;
        }
        // pressed cancel
        else if (result == NFD_CANCEL)
        {
            DEBUG_PRINT_NL("[NFD][INFO] User pressed cancel (SaveFileDialog)")
        }
        // error opening the file
        else
        {
            DEBUG_PRINT_NL("[NFD][Error] Error opening save file dialog" << NFD_GetError())
        }
        return "";
    }


    std::string FileSelectionDialog()
    {
        nfdchar_t* outPath = NULL;
        nfdresult_t result = NFD_OpenDialog("", NULL, &outPath);
        if (result == NFD_OKAY)
        {
            std::string filePath = outPath;
            free(outPath);
            return filePath;
        }
        // pressed cancel
        else if (result == NFD_CANCEL)
        {
            DEBUG_PRINT_NL("[NFD][INFO] User pressed cancel (FileSelectionDialog)")
        }
        // error opening the file
        else
        {
            DEBUG_PRINT_NL("[NFD][Error] Error opening file selection dialog" << NFD_GetError())
        }
        return "";
    }
}
#endif