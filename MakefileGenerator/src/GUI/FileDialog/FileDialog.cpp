#include "nfd/nfd.h"

#include "FileDialog.h"
#include "FileHandler.h"

namespace FileDialog {

    bool multiFileDialog(std::string* base_str)
    {
        nfdpathset_t pathSet;
        nfdresult_t result = NFD_OpenDialogMultiple("", NULL, &pathSet);
        if (result == NFD_OKAY)
        {
            for (size_t i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i)
            {
                nfdchar_t* path = NFD_PathSet_GetPath(&pathSet, i);
                FH::addEntry(FH::getRelativePath(*base_str, path));
            }
            NFD_PathSet_Free(&pathSet);
            return true;
        }
        else if (result == NFD_CANCEL)
        {
            return false;
            //puts("User pressed cancel.");
        }
        else
        {
            printf("Error opening multi file dialog: %s\n", NFD_GetError());
            return false;
        }
    }


    bool folderSelectionDialog(std::string* base_str, std::string* str, bool single_str)
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
                {
                    if (*base_str == outPath) {}
                        //*str = *base_str;{}
                    else
                        *str = FH::getRelativePath(*base_str, outPath);
                }
            }
            else
            {
                if (*base_str == outPath)
                {
                    //str->append(*base_str);
                    //str->append(";");
                }
                else
                {
                    str->append(FH::getRelativePath(*base_str, outPath));
                    str->append(";");
                }
            }

            free(outPath);
            return true;
        }
        else if (result == NFD_CANCEL)
        {
            return false;
        }
        else
        {
            printf("Error opening folder selection dialog: %s\n", NFD_GetError());
            return false;
        }
    }


    bool handleFileDialog(char dialog, std::string* base_str, std::string* str, bool single_str)
    {
        switch (dialog)
        {
        case MULTIPLE_FILE_DIALOG:
            return multiFileDialog(base_str);
        case FOLDER_SELECT_DIALOG:
            return folderSelectionDialog(base_str, str, single_str);
        default:
            return false;
        }
    }


    std::string SaveFileDialog()
    {
        nfdchar_t* savePath = NULL;
        nfdresult_t result = NFD_SaveDialog("mg", NULL, &savePath);
        if (result == NFD_OKAY)
        {
            std::string resultStr = savePath;
            free(savePath);
            return resultStr;
        }
        else if (result == NFD_CANCEL)
        {
            puts("User pressed cancel.");
        }
        else
        {
            printf("Error: %s\n", NFD_GetError());
        }
        return "";
    }


    std::string FileSelectionDialog()
    {
        nfdchar_t* outPath = NULL;
        nfdresult_t result = NFD_OpenDialog("", NULL, &outPath);
        if (result == NFD_OKAY)
        {
            std::string path = outPath;
            free(outPath);
            return path;
        }
        else if (result == NFD_CANCEL)
        {
            puts("User pressed cancel.");
        }
        else
        {
            printf("Error: %s\n", NFD_GetError());
        }
        return "";
    }
}