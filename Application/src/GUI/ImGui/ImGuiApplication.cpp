#include <iostream>
#include <array>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "nfd/nfd.h"

// GUI includes
#include "GUI/ImGui/ImGuiApplication.h"
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/FileDialog/FileDialog.h"
// Gui widgets
#include "GUI/ImGui/Widgets/ImGuiButton.h"
#include "GUI/ImGui/Widgets/ImGuiTextInputWithHint.h"
#include "GUI/ImGui/Widgets/ImGuiComboBox.h"

#include "FileHandler.h"


// using ImGui --------------------------------------------------------
namespace IGA {

    //#define Y_BTN_OFFSET 60.f
    //#define Y_BTN_DIST   60.f
#define Y_TXT_IP_OFFSET 7.f
    template <size_t S, size_t S2>
    void resizeControlWindow(std::array<IGWidget::Button*, S>& buttons, std::array<IGWidget::TextInputWithHint*, S2>& textInputs, IGWidget::ComboBox& compilerCombo)
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
        static IGWidget::ComboBox compilerCombo("##CompilerCombo", "gcc\0g++\0clang\0clang++\0Other (Cf)");
        static IGWidget::TextInputWithHint compilerFlagsInput("##IncludeFlags", "Compiler flags");

        static IGWidget::Button selectIncludeDirs("Select directories##SelectIncludeDirs");
        static IGWidget::TextInputWithHint includeDirsInput("##IncludeDirs", "Add additional include directories");

        static IGWidget::Button selectLibraries("Select libraries");
        static IGWidget::TextInputWithHint libraryInput("##Libraries", "Add additional libraries (already provide it for the linker)");

        static IGWidget::Button selectLibraryDirs("Select directories##SelectLibraryDirs");
        static IGWidget::TextInputWithHint libraryDirsInput("##LibraryDirs", "Add additional library directories");

        static IGWidget::Button selectFiles("Select files");
        static IGWidget::TextInputWithHint searchInput("##SearchFiles", "Search files");

        static std::array<IGWidget::Button*, 4> buttons = { &selectFiles, &selectLibraryDirs, &selectLibraries, &selectIncludeDirs };
        static std::array<IGWidget::TextInputWithHint*, 5> textInputs = { &searchInput, &libraryDirsInput, &libraryInput, &includeDirsInput, &compilerFlagsInput };

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

        if (libraryDirsInput.used())
            std::cout << "Library Dirs: " << libraryDirsInput.input << std::endl;

        if (selectFiles.used())
        {
            std::cout << "file Pressed\n";
            FileDialog::handleFileDialog(MULTIPLE_FILE_DIALOG);
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
            ImVec2 ws(window_size.first, 45.f);
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
            FH::undoLastDelete(selectAllChecked);

        ImGui::End();
    }


    static const int MAX = 100;
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