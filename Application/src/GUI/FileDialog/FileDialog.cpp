#include "nfd/nfd.h"

#include "FileDialog.h"
#include "FileHandler.h"

namespace FileDialog {

    void multiFileDialog()
    {
        nfdpathset_t pathSet;
        nfdresult_t result = NFD_OpenDialogMultiple("", NULL, &pathSet);
        if (result == NFD_OKAY)
        {
            for (size_t i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i)
            {
                nfdchar_t* path = NFD_PathSet_GetPath(&pathSet, i);
                FH::addEntry(path);
            }
            NFD_PathSet_Free(&pathSet);
        }
        else if (result == NFD_CANCEL)
        {
            //puts("User pressed cancel.");
        }
        else
        {
            printf("Error opening multi file dialog: %s\n", NFD_GetError());
        }
    }

    void folderSelectionDialog()
    {
        nfdchar_t* outPath = NULL;
        nfdresult_t result = NFD_PickFolder(NULL, &outPath);
        if (result == NFD_OKAY)
        {
            //puts("Success!");
            //puts(outPath);
            FH::addEntry(outPath);
            free(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            //puts("User pressed cancel.");
        }
        else
        {
            printf("Error opening folder selection dialog: %s\n", NFD_GetError());
        }
    }

    void handleFileDialog(char dialog)
    {
        if (dialog == MULTIPLE_FILE_DIALOG)
            multiFileDialog();
        else if (dialog == FOLDER_SELECT_DIALOG)
            folderSelectionDialog();
    }

}