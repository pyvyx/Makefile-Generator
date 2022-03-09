#include <iostream>
#include <array>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"

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
#include "Generator.h"


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
        static ControlWindowInfo cwi;

        pushStyleColor(styleColors);
        createControlWindow(&cwi);
        createFileViewControl(&cwi);
        createFileView();
        popStyleColor(styleColors);
    }


    template <size_t S, size_t S2>
    void resizeControlWindow(std::array<IGWidget::Button*, S>& buttons, std::array<IGWidget::TextInputWithHint*, S2>& textInputs, IGWidget::ComboBox& compilerCombo, int selectedBinaryFormat)
    {
        static const ImVec2 window_pos(0.f, 25.f);
        std::pair<float, float> window_size = IGW::g_Window->getSize();
        ImVec2 ws(window_size.first, window_size.second / 2.f - 25.f);
        ImGui::SetNextWindowSize(ws);
        ImGui::SetNextWindowPos(window_pos);

        float btn_offset = ((ws.y - ((textInputs.size()) * IGWidget::sg_ButtonSize.y)) / ((textInputs.size() + 1))) + IGWidget::sg_ButtonSize.y;
        float offset = btn_offset;
        for (size_t i = 0; i < buttons.size(); ++i)
        {
            buttons[i]->resetPosSize(ws, offset);
            offset += btn_offset;
        }
        compilerCombo.resetPosSize(ws, offset + btn_offset - IGWidget::sg_TextInputYOffset);

        offset = btn_offset - IGWidget::sg_TextInputYOffset;
        for (size_t i = 0; i < textInputs.size(); ++i)
        {
            textInputs[i]->resetPosSize(ws, offset);
            offset += btn_offset;
        }
    }


    void createControlWindow(ControlWindowInfo* cwi)
    {
        static IGWidget::TextInputWithHint outputFileName("##NameOutputFile", "Set a name for the binary (libMyName.so/.dll for shared libs, libMyName.a/.lib for static libs)");
        
        static IGWidget::ComboBox compilerCombo("##CompilerCombo", "gcc | g++\0clang | clang++\0gcc\0g++\0clang\0clang++\0Other (Cf)\0");
        static IGWidget::TextInputWithHint ccompilerFlagsInput("##CFLAGS", "C compiler flags e.G. myccompiler -O2 -o MyOut");
        static IGWidget::TextInputWithHint cppcompilerFlagsInput("##CXXFLAGS", "C++ compiler flags e.G. myccompiler -O2 -o MyOut");

        static IGWidget::Button selectMakeFileOutputPath("Select file output##SelectFileOutput");
        static IGWidget::TextInputWithHint makeFileOutputPath("##FileOutput", "Set an output directory for the generated makefile");
       
        static IGWidget::Button selectOutputDir("Select output dir##SelectOutputDir");
        static IGWidget::TextInputWithHint outputDir("##OutputDir", "Set an output directory for the binary files");

        static IGWidget::Button selectIncludeDirs("Select directories##SelectIncludeDirs");
        static IGWidget::TextInputWithHint includeDirsInput("##IncludeDirs", "Add additional include directories");

        static IGWidget::Button selectLibraries("Select libraries");
        static IGWidget::TextInputWithHint libraryInput("##Libraries", "Link libraries e.G. -lpthread");

        static IGWidget::Button selectLibraryDirs("Select directories##SelectLibraryDirs");
        static IGWidget::TextInputWithHint libraryDirsInput("##LibraryDirs", "Add additional library directories");

        static IGWidget::Button selectFiles("Select files");
        static IGWidget::TextInputWithHint searchInput("##SearchFiles", "Search files");

        static std::array<IGWidget::Button*, 6> buttons = { &selectFiles, &selectLibraryDirs, &selectIncludeDirs, &selectOutputDir, &selectMakeFileOutputPath, &selectLibraries };
        static std::array<IGWidget::TextInputWithHint*, 9> textInputs = { &searchInput, &libraryDirsInput, &includeDirsInput, &outputDir, &makeFileOutputPath, &libraryInput, &cppcompilerFlagsInput, &ccompilerFlagsInput, &outputFileName };

        ImGui::SetNextWindowBgAlpha(1.f);
        if (IGW::g_Window->hasResized())
            resizeControlWindow(buttons, textInputs, compilerCombo, cwi->selectedBinaryFormat);

        ImGui::Begin("ControlWindow", (bool*)0, IMGUI_WINDOW_FLAGS);


        static bool selectedMakeFileOutputPath = false;
        if (selectMakeFileOutputPath.clicked())
        {
            if (FileDialog::handleFileDialog(FOLDER_SELECT_DIALOG, nullptr, &makeFileOutputPath.input, true))
                selectedMakeFileOutputPath = true;
        }

        if (selectedMakeFileOutputPath && makeFileOutputPath.input != "")
        {
            if (outputFileName.added())
            {
            }

            if (makeFileOutputPath.added())
            {
            }

            // binary output directory
            if (selectOutputDir.clicked())
            {
                FileDialog::handleFileDialog(FOLDER_SELECT_DIALOG, &makeFileOutputPath.input, &outputDir.input, true);
            }

            if (outputDir.added())
            {
            }

            // compiler combo box
            if (compilerCombo.Selected())
            {
            }

            if (ccompilerFlagsInput.added())
            { }

            if (cppcompilerFlagsInput.added())
            {
            }

            // include directories
            if (selectIncludeDirs.clicked())
            {
                FileDialog::handleFileDialog(FOLDER_SELECT_DIALOG, &makeFileOutputPath.input, &includeDirsInput.input);
            }

            if (includeDirsInput.added())
            { }

            // link libraries
            if (libraryInput.added())
            { }

            // library directories
            if (selectLibraryDirs.clicked())
            {
                FileDialog::handleFileDialog(FOLDER_SELECT_DIALOG, &makeFileOutputPath.input, &libraryDirsInput.input);
            }

            if (libraryDirsInput.added())
            {
            }

            // files
            if (selectFiles.clicked())
            {
                FileDialog::handleFileDialog(MULTIPLE_FILE_DIALOG, &makeFileOutputPath.input, nullptr);
            }

            if (searchInput.added())
                FH::filterFileEntries(searchInput.input);
        }

        cwi->selectedCompiler = &compilerCombo.selected;
        cwi->ccompilerFlags   = &ccompilerFlagsInput.input;
        cwi->cppcompilerFlags = &cppcompilerFlagsInput.input;
        cwi->linkLibraries    = &libraryInput.input;
        cwi->makeFileOutput   = &makeFileOutputPath.input;
        cwi->outputDir        = &outputDir.input;
        cwi->includeDirs      = &includeDirsInput.input;
        cwi->libraryDirs      = &libraryDirsInput.input;
        cwi->outFileName      = &outputFileName.input;

        ImGui::End();
    }


    static constexpr float sg_FileViewControlHight = 45.f;
    void createFileViewControl(ControlWindowInfo* cwi)
    {
        ImGui::SetNextWindowBgAlpha(1.f);
        if (IGW::g_Window->hasResized())
        {
            std::pair<float, float> window_size = IGW::g_Window->getSize();
            ImVec2 ws(window_size.first, sg_FileViewControlHight);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, (window_size.second / 2.f) });
        }
        static int selectedBinaryFormat = 0;
        static bool selectAllChecked = false;
        static std::string dllFileName;

        ImGui::Begin("FileViewControl", (bool*)0, IMGUI_WINDOW_FLAGS);
        if (ImGui::Button("Generate"))
        {
            FH::FileEntryVec& fileEntries = FH::getFileEntriesRef();
            MG::GenerateMakeFile({ *cwi->selectedCompiler, *cwi->usePIL, *cwi->ccompilerFlags,
                * cwi->cppcompilerFlags,
                *cwi->linkLibraries, *cwi->makeFileOutput,
                *cwi->outputDir, *cwi->includeDirs,
                *cwi->libraryDirs, fileEntries, selectedBinaryFormat, dllFileName, *cwi->outFileName });
        }



        ImGui::BeginMainMenuBar();
        if (ImGui::MenuItem("Save configuration"))
        {
            std::string filePath = FileDialog::SaveFileDialog();
            if (filePath != "")
            {
                FH::FileEntryVec& fileEntries = FH::getFileEntriesRef();
                MG::SaveConfigFile({ *cwi->selectedCompiler, *cwi->usePIL, *cwi->ccompilerFlags,
                    *cwi->cppcompilerFlags,
                    *cwi->linkLibraries, *cwi->makeFileOutput,
                    *cwi->outputDir, *cwi->includeDirs,
                    *cwi->libraryDirs, fileEntries, selectedBinaryFormat, dllFileName, *cwi->outFileName }, filePath, selectAllChecked);
            }
        }
        if (ImGui::MenuItem("Load configuration"))
        {
            std::string filePath = FileDialog::FileSelectionDialog();
            MG::LoadConfigFile(filePath);
        }
        ImGui::EndMainMenuBar();



        ImGui::SameLine(100.f);
        ImGui::PushItemWidth(150.f);
        ImGui::Combo("##SelectBinaryFormat", &selectedBinaryFormat, "Application\0Static library\0Dynamic library\0");

        ImGui::SameLine(430.f);
        if (ImGui::Checkbox("Select all", &selectAllChecked))
            FH::setSelectAllEntries(selectAllChecked);

        ImGui::SameLine(540.f);
        if (ImGui::Button("Delete selected"))
            FH::deleteSelectedEntries();

        ImGui::SameLine(680.f);
        if (ImGui::Button("undo"))
            FH::undoLastDelete(selectAllChecked);

        static bool usePIL = true;
        ImGui::SameLine(740.f);
        // Position independent linking
        ImGui::Checkbox("Use PIL", &usePIL);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Position independent linking");
        cwi->usePIL = &usePIL;


        if (selectedBinaryFormat == MG::BuildModes::DynamicLibrary)
        {
            ImGui::SameLine(265.f);
            ImGui::PushItemWidth(150.f);
            ImGui::InputTextWithHint("##DllFileName", "lib file name", &dllFileName, ImGuiInputTextFlags_AllowTabInput);
        }

        ImGui::End();
    }


    static const int MAX = 100;
    void fillTestVector()
    {
        //static FH::FileEntryVec& entries = FH::getFileEntriesRef();
        //for (int i = 0; i < MAX; ++i)
        //    entries.push_back("File" + std::to_string(i));
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