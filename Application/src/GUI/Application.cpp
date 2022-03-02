#define USING_IMGUI
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <array>

#include "nfd/nfd.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"

// GUI includes
#include "Application.h"
#include "ImGuiWindow.h"

#include "FileHandler.h"


namespace App {

    void runApplication()
    {
        #ifdef USING_IMGUI
            IGW::runWindow();
        #endif
    }
}

// using ImGui --------------------------------------------------------
namespace IGA {

    void handleFileDialog()
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
            printf("Error opening file dialog: %s\n", NFD_GetError());
        }
    }

    #define X_OFFSET_BTN    ws.x - 170.f
    #define Y_OFFSET_BTN(x) ws.y - x

    #define X_SIZE_BTN      150.f
    #define Y_SIZE_BTN      40.f

    #define X_POS_TXT_IP    10.f
    #define X_SIZE_TXT_IP   ws.x - 200.f
    #define Y_OFF_TXT_IP(x) ws.y - x

    class Button
    {
    public:
        ImVec2 pos;
        ImVec2 size;
        const char* m_Label;
    public:
        Button() : m_Label("") {}
        Button(const char* label) : m_Label(label) {}

        bool used(const char* label)
        {
            ImGui::SetCursorPos(pos);
            return ImGui::Button(label, size);
        }

        bool used()
        {
            //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.33f, 0.33f, 0.33f, 1.0f });
            ImGui::SetCursorPos(pos);
            return ImGui::Button(m_Label, size);
        }
        
        void resetPosSize(const ImVec2& ws, float y_offset)
        {
            // set button pos
            pos.x = X_OFFSET_BTN;
            pos.y = Y_OFFSET_BTN(y_offset);

            // set button size
            size.x = X_SIZE_BTN;
            size.y = Y_SIZE_BTN;
        }
    };

    class TextInput
    {
    public:
        ImVec2 pos;
        float length = 100.f;
        std::string input;
        const char* m_Label;
        const char* m_Hint;
    public:
        TextInput() : m_Label(""), m_Hint("") {};
        TextInput(const char* label, const char* hint) : m_Label(label), m_Hint(hint) {}

        bool used(const char* label, const char* hint)
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushItemWidth(length);
            return ImGui::InputTextWithHint(label, hint, &input, ImGuiInputTextFlags_AllowTabInput);
        }

        bool used()
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushItemWidth(length);
            return ImGui::InputTextWithHint(m_Label, m_Hint, &input, ImGuiInputTextFlags_AllowTabInput);
        }

        void resetPosSize(const ImVec2& ws, float y_offset)
        {
            // set text input position
            pos.x = X_POS_TXT_IP;
            pos.y = Y_OFF_TXT_IP(y_offset);

            // set text input size
            length = X_SIZE_TXT_IP;
        }
    };

    class ComboBox
    {
    public:
        ImVec2 pos;
        float length = 100.f;
        int selected = 0;
        const char* label;
        const char* items;
    public:
        ComboBox() : label(""), items("") {}
        ComboBox(const char* label, const char* items) : label(label), items(items) {}
        
        bool used(const char* label_p, const char* items_p)
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushItemWidth(length);
            return ImGui::Combo(label_p, &selected, items_p);
        }

        bool used()
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushItemWidth(length);
            return ImGui::Combo(label, &selected, items);
        }

        void resetPosSize(const ImVec2& ws, float y_offset)
        {
            // set combo box pos
            pos.x = X_OFFSET_BTN;
            pos.y = Y_OFFSET_BTN(y_offset);

            // set combo box size
            length = X_SIZE_BTN;
        }
    };

    //#define Y_BTN_OFFSET 60.f
    //#define Y_BTN_DIST   60.f
    #define Y_TXT_IP_OFFSET 7.f
    template <size_t S, size_t S2>
    void resizeControlWindow(std::array<Button*, S>& buttons, std::array<TextInput*, S2>& textInputs, ComboBox& compilerCombo)
    {
        static const ImVec2 window_pos(0.f, 0.f);
        std::pair<float, float> window_size = IGW::sg_Window->getSize();
        ImVec2 ws(window_size.first, window_size.second / 2.f);
        ImGui::SetNextWindowSize(ws);
        ImGui::SetNextWindowPos(window_pos);

        float distance = ((ws.y - (textInputs.size() * Y_SIZE_BTN)) / (textInputs.size() + 1)) + Y_SIZE_BTN;

        float offset = distance;
        for (size_t i = 0; i < buttons.size(); ++i)
        {
            buttons[i]->resetPosSize(ws, offset);
            offset += distance;
        }
        compilerCombo.resetPosSize(ws, offset - Y_TXT_IP_OFFSET);

        offset = distance - Y_TXT_IP_OFFSET;
        for (size_t i = 0; i < textInputs.size(); ++i)
        {
            textInputs[i]->resetPosSize(ws, offset);
            offset += distance;
        }

    }

    void createControlWindow()
    {
        static ComboBox compilerCombo("##CompilerCombo", "gcc\0g++\0clang\0clang++\0Other (Cf)");
        static TextInput compilerFlagsInput("##IncludeFlags", "Compiler flags");

        static Button selectIncludeDirs("Select directories##SelectIncludeDirs");
        static TextInput includeDirsInput("##IncludeDirs", "Add additional include directories");

        static Button selectLibraries("Select libraries");
        static TextInput libraryInput("##Libraries", "Add additional libraries (already provide it for the linker)");

        static Button selectLibraryDirs("Select directories##SelectLibraryDirs");
        static TextInput libraryDirsInput("##LibraryDirs", "Add additional library directories");

        static Button selectFiles("Select files");
        static TextInput searchInput("##SearchFiles", "Search files");
        
        static std::array<Button*, 4> buttons = { &selectFiles, &selectLibraryDirs, &selectLibraries, &selectIncludeDirs };
        static std::array<TextInput*, 5> textInputs = { &searchInput, &libraryDirsInput, &libraryInput, &includeDirsInput, &compilerFlagsInput };

        ImGui::SetNextWindowBgAlpha(1.f);
        if (IGW::sg_Window->hasResized())
            resizeControlWindow(buttons, textInputs, compilerCombo);

        ImGui::Begin("ControlWindow", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

        if (compilerCombo.used())
            std::cout << "Compiler combo selected: " << compilerCombo.selected << std::endl;

        if (compilerFlagsInput.used())
            std::cout << "Compiler flags: " << compilerFlagsInput.input << std::endl;

        if (selectIncludeDirs.used())
            std::cout << "Include dirs Pressed\n";
        
        if (includeDirsInput.used())
            std::cout << "Include Dirs: " << includeDirsInput.input << std::endl;
        
        if (selectLibraries.used())
            std::cout << "Select libs pressed\n";
        
        if (libraryInput.used())
            std::cout << "Library input: " << libraryInput.input << std::endl;
        
        if (selectLibraryDirs.used())
            std::cout << "select lib dirs Pressed\n";

        if(libraryDirsInput.used())
            std::cout << "Library Dirs: " << libraryDirsInput.input << std::endl;
        
        if (selectFiles.used())
        {
            std::cout << "file Pressed\n";
            handleFileDialog();
        }
        
        if (searchInput.used())
            FH::filterFileEntries(searchInput.input);


        ImGui::End();
    }


    void createFileViewControl()
    {
        ImGui::SetNextWindowBgAlpha(1.f);
        if (IGW::sg_Window->hasResized())
        {
            std::pair<float, float> window_size = IGW::sg_Window->getSize();
            ImVec2 ws(window_size.first,  45.f);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, (window_size.second / 2.f) });
        }

        ImGui::Begin("FileViewControl", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        if (ImGui::Button("Generate"))
            std::cout << "Generate pressed\n";
        
        static bool selectAllChecked = false;
        ImGui::SameLine(100.f);
        if (ImGui::Checkbox("Select all", &selectAllChecked))
        {
            FH::setSelectAllEntries(selectAllChecked);
        }
        
        ImGui::SameLine(210.f);
        if (ImGui::Button("Delete selected"))
            FH::deleteSelectedEntries();
        
        ImGui::SameLine(350.f);
        if (ImGui::Button("undo"))
            FH::undoLastDelete();

        ImGui::End();
    }


    #define MAX 1000
    void fillTestVector()
    {
        static FH::FileEntryVec* entries = FH::getFileEntriesPtr();
        for (int i = 0; i < MAX; ++i)
            entries->push_back("File" + std::to_string(i));
    }

    void createFileView()
    {
        ImGui::SetNextWindowBgAlpha(1.f);
        if (IGW::sg_Window->hasResized())
        {
            std::pair<float, float> window_size = IGW::sg_Window->getSize();
            IGW::sg_Window->resized(false);
            ImVec2 ws(window_size.first, window_size.second / 2.f - 45);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, ws.y + 90.f });
        }
        static FH::FileEntryVec* fileEntries = FH::getFileEntriesPtr();

        ImGui::Begin("FileView", (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        ImGui::Columns(1);
        ImGui::Separator();
        

        for (size_t i = 0; i < fileEntries->size(); ++i)
        {
            if (!fileEntries->at(i).isDeleted() && fileEntries->at(i).isShown())
            {
                ImGui::Selectable(fileEntries->at(i).fileName().c_str(), (bool*)&fileEntries->at(i).getSelectedRef());
                ImGui::Separator();
            }
        }

        ImGui::Separator();
        ImGui::End();
    }
}