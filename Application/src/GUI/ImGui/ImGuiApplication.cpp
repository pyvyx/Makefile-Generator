#include <iostream>
#include <array>

#include "ImGui/imgui.h"

// GUI includes
#include "GUI/ImGui/ImGuiApplication.h"
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/FileDialog/FileDialog.h"
// Gui widgets
#include "GUI/ImGui/Widgets/ImGuiButton.h"
#include "GUI/ImGui/Widgets/ImGuiTextInputWithHint.h"
#include "GUI/ImGui/Widgets/ImGuiComboBox.h"
#include "GUI/ImGui/Widgets/ImGuiSizeProperties.h"

#include "FileHandler.h"


#define IMGUI_WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar

// using ImGui --------------------------------------------------------
namespace IGA {

    void startApplication()
    {
        // for styling
        static const ImVec4 s_DarkerColor  (0.27f, 0.27f, 0.27f, 1.0f);
        static const ImVec4 s_BrighterColor(0.57f, 0.57f, 0.57f, 1.0f);
        static const ImVec4 s_WhiteColor   (  1.f,   1.f,   1.f, 1.0f);
        static constexpr std::array<IGWidget::WidgetColor, 9> styleColors = {
            IGWidget::WidgetColor(ImGuiCol_FrameBg,        &s_DarkerColor),
            IGWidget::WidgetColor(ImGuiCol_FrameBgHovered, &s_BrighterColor),
            IGWidget::WidgetColor(ImGuiCol_CheckMark,      &s_WhiteColor),
            IGWidget::WidgetColor(ImGuiCol_Header,         &s_DarkerColor),
            IGWidget::WidgetColor(ImGuiCol_HeaderActive,   &s_DarkerColor),
            IGWidget::WidgetColor(ImGuiCol_HeaderHovered,  &s_BrighterColor),
            IGWidget::WidgetColor(ImGuiCol_Button,         &s_DarkerColor),
            IGWidget::WidgetColor(ImGuiCol_ButtonHovered,  &s_BrighterColor),
            IGWidget::WidgetColor(ImGuiCol_ButtonActive,   &s_BrighterColor)
        };

        pushStyleColor(styleColors);
        createControlWindow();
        createFileViewControl();
        createFileView();
        popStyleColor(styleColors);
    }


    template <size_t S, size_t S2>
    void resizeControlWindow(std::array<IGWidget::Button*, S>& buttons, std::array<IGWidget::TextInputWithHint*, S2>& textInputs, IGWidget::ComboBox& compilerCombo)
    {
        static const ImVec2 window_pos(0.f, 0.f);
        static constexpr float textInputYOffset = 7.f;

        std::pair<float, float> window_size = IGW::g_Window->getSize();
        ImVec2 ws(window_size.first, window_size.second / 2.f);
        ImGui::SetNextWindowSize(ws);
        ImGui::SetNextWindowPos(window_pos);

        float btn_offset = ((ws.y - (textInputs.size() * IGWidget::sg_ButtonSize.y)) / (textInputs.size() + 1)) + IGWidget::sg_ButtonSize.y;
        float offset = btn_offset;
        for (size_t i = 0; i < buttons.size(); ++i)
        {
            buttons[i]->resetPosSize(ws, offset);
            offset += btn_offset;
        }
        compilerCombo.resetPosSize(ws, offset - textInputYOffset);

        offset = btn_offset - textInputYOffset;
        for (size_t i = 0; i < textInputs.size(); ++i)
        {
            textInputs[i]->resetPosSize(ws, offset);
            offset += btn_offset;
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
        if (IGW::g_Window->hasResized())
            resizeControlWindow(buttons, textInputs, compilerCombo);

        ImGui::Begin("ControlWindow", (bool*)0, IMGUI_WINDOW_FLAGS);

        if (compilerCombo.Selected())
            std::cout << "Compiler combo selected: " << compilerCombo.selected << std::endl;

        if (compilerFlagsInput.added())
            std::cout << "Compiler flags: " << compilerFlagsInput.input << std::endl;

        if (selectIncludeDirs.clicked())
            std::cout << "Include dirs Pressed\n";

        if (includeDirsInput.added())
            std::cout << "Include Dirs: " << includeDirsInput.input << std::endl;

        if (selectLibraries.clicked())
            std::cout << "Select libs pressed\n";

        if (libraryInput.added())
            std::cout << "Library input: " << libraryInput.input << std::endl;

        if (selectLibraryDirs.clicked())
            std::cout << "select lib dirs Pressed\n";

        if (libraryDirsInput.added())
            std::cout << "Library Dirs: " << libraryDirsInput.input << std::endl;

        if (selectFiles.clicked())
        {
            std::cout << "file Pressed\n";
            FileDialog::handleFileDialog(MULTIPLE_FILE_DIALOG);
        }

        if (searchInput.added())
            FH::filterFileEntries(searchInput.input);


        ImGui::End();
    }


    static constexpr float sg_FileViewControlHight = 45.f;
    void createFileViewControl()
    {
        ImGui::SetNextWindowBgAlpha(1.f);
        if (IGW::g_Window->hasResized())
        {
            std::pair<float, float> window_size = IGW::g_Window->getSize();
            ImVec2 ws(window_size.first, sg_FileViewControlHight);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, (window_size.second / 2.f) });
        }

        ImGui::Begin("FileViewControl", (bool*)0, IMGUI_WINDOW_FLAGS);
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
        static FH::FileEntryVec& entries = FH::getFileEntriesRef();
        for (int i = 0; i < MAX; ++i)
            entries.push_back("File" + std::to_string(i));
    }

    void createFileView()
    {
        ImGui::SetNextWindowBgAlpha(1.f);
        if (IGW::g_Window->hasResized())
        {
            std::pair<float, float> window_size = IGW::g_Window->getSize();
            IGW::g_Window->resized(false);
            ImVec2 ws(window_size.first, (window_size.second / 2.f) - sg_FileViewControlHight);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, ws.y + 90.f });
        }
        static FH::FileEntryVec& fileEntries = FH::getFileEntriesRef();
        size_t fileEntriesSize = fileEntries.size();

        ImGui::Begin("FileView", (bool*)0, IMGUI_WINDOW_FLAGS);
        ImGui::Columns(1);

        if(fileEntriesSize != 0)
            ImGui::Separator();


        for (size_t i = 0; i < fileEntriesSize; ++i)
        {
            if (!fileEntries[i].isDeleted() && fileEntries[i].isShown())
            {
                ImGui::Selectable(fileEntries[i].fileName().c_str(), (bool*)&fileEntries[i].getSelectedRef());
                ImGui::Separator();
            }
        }

        ImGui::End();
    }
}