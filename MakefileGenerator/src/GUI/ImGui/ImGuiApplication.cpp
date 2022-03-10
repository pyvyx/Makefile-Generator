#include <iostream>
#include <array>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"

// GUI includes
#include "GUI/ImGui/ImGuiApplication.h"
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/FileDialog/FileDialog.h"
#include "GUI/ImGui/ImGuiContext.h"
// Gui widgets
#include "GUI/ImGui/Widgets/ImGuiButton.h"
#include "GUI/ImGui/Widgets/ImGuiTextInputWithHint.h"
#include "GUI/ImGui/Widgets/ImGuiComboBox.h"
#include "GUI/ImGui/Widgets/ImGuiCheckBox.h"
#include "GUI/ImGui/Widgets/ImGuiSizeProperties.h"

#include "FileHandler.h"
#include "Generator.h"


#define IMGUI_WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar

// using ImGui --------------------------------------------------------
namespace IGA {

    template<size_t S>
    void PushStyleColors(const std::array<IGWidget::WidgetColor, S>& colorFlags)
    {
        for (size_t i = 0; i < colorFlags.size(); ++i)
            ImGui::PushStyleColor(colorFlags[i].colorFlag, *colorFlags[i].color);
    }


    template<size_t S>
    void PopStyleColors(const std::array<IGWidget::WidgetColor, S>& colorFlags)
    {
        for (size_t i = 0; i < colorFlags.size(); ++i)
            ImGui::PopStyleColor();
    }


    void StartApplication()
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

        PushStyleColors(styleColors);
        createControlWindow(&cwi);
        createFileViewControl(&cwi);
        createFileView();
        PopStyleColors(styleColors);
    }

    template <size_t S, size_t S2>
    struct ControlWindowWidgets
    {
        std::array<IGWidget::Button*, S> buttons;
        std::array<IGWidget::TextInputWithHint*, S2> textInputs;
        IGWidget::ComboBox compilerCombo;
    };

    //template <size_t S, size_t S2>
    //void resizeControlWindow(std::array<IGWidget::Button*, S>& buttons, std::array<IGWidget::TextInputWithHint*, S2>& textInputs, IGWidget::ComboBox& compilerCombo)
    //{
    //    static const ImVec2 window_pos(0.f, 25.f);
    //    IGW::Window* window = IGW::GetWindowPtr();
    //    std::pair<float, float> window_size = window->getSize();
    //    ImVec2 ws(window_size.first, window_size.second / 2.f - 25.f);
    //    ImGui::SetNextWindowSize(ws);
    //    ImGui::SetNextWindowPos(window_pos);
    //
    //    float btn_offset = ((ws.y - ((textInputs.size()) * IGWidget::sg_ButtonSize.y)) / ((textInputs.size() + 1))) + IGWidget::sg_ButtonSize.y;
    //    float offset = btn_offset;
    //    for (size_t i = 0; i < buttons.size(); ++i)
    //    {
    //        buttons[i]->resetPosSize(ws, offset);
    //        offset += btn_offset;
    //    }
    //    compilerCombo.resetPosSize(ws, offset + btn_offset - IGWidget::sg_TextInputYOffset);
    //
    //    offset = btn_offset - IGWidget::sg_TextInputYOffset;
    //    for (size_t i = 0; i < textInputs.size(); ++i)
    //    {
    //        textInputs[i]->resetPosSize(ws, offset);
    //        offset += btn_offset;
    //    }
    //}

    void resizeControlWindow(ControlWindowWidgets<6,9> cw)
    {

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

        static ControlWindowWidgets<6, 9> abc;// (buttons, textInputs, compilerCombo);
        abc.buttons = buttons;
        abc.textInputs = textInputs;
        abc.compilerCombo = compilerCombo;

        static ImGuiContext<ControlWindowWidgets<6,9>> ct("test", [](ControlWindowWidgets<6, 9> cw) { std::cout << 1 << std::endl; }, abc);

        ImGui::SetNextWindowBgAlpha(1.f);
        IGW::Window* window = IGW::GetWindowPtr();
        if (window->hasResized())
            ct.OnResize();
            //resizeControlWindow(buttons, textInputs, compilerCombo);

        ImGui::Begin("ControlWindow", (bool*)0, IMGUI_WINDOW_FLAGS);


        static bool selectedMakeFileOutputPath = false;
        if (selectMakeFileOutputPath.clicked())
        {
            if(FileDialog::FolderSelectionDialog(nullptr, &makeFileOutputPath.input, true))
                selectedMakeFileOutputPath = true;
        }

        if (selectedMakeFileOutputPath && makeFileOutputPath.input != "")
        {
            outputFileName.added();
            makeFileOutputPath.added();

            // binary output directory
            if (selectOutputDir.clicked())
            {
                FileDialog::FolderSelectionDialog(&makeFileOutputPath.input, &outputDir.input, true);
            }
            outputDir.added();

            // compiler combo box
            compilerCombo.Selected();

            // compiler flags
            ccompilerFlagsInput.added();
            cppcompilerFlagsInput.added();

            // include directories
            if (selectIncludeDirs.clicked())
            {
                FileDialog::FolderSelectionDialog(&makeFileOutputPath.input, &includeDirsInput.input, false);
            }
            includeDirsInput.added();

            // link libraries
            libraryInput.added();

            // library directories
            if (selectLibraryDirs.clicked())
            {
                FileDialog::FolderSelectionDialog(&makeFileOutputPath.input, &libraryDirsInput.input, false);
            }
            libraryDirsInput.added();

            // files
            if (selectFiles.clicked())
            {
                FileDialog::MultiFileDialog(&makeFileOutputPath.input);
            }

            if (searchInput.added())
                FH::FilterFileEntries(searchInput.input);
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
        IGW::Window* window = IGW::GetWindowPtr();
        if (window->hasResized())
        {
            std::pair<float, float> window_size = window->getSize();
            ImVec2 ws(window_size.first, sg_FileViewControlHight);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, (window_size.second / 2.f) });
        }


        static IGWidget::Button deleteSelectedFiles("Delete selected");
        static IGWidget::Button undoButton("Undo");

        static IGWidget::ComboBox selectedBinFormat("##SelectBinaryFormat", "Application\0Static library\0Dynamic library\0");
        static IGWidget::CheckBox selectAll("Select all");
        static IGWidget::CheckBox usePIL("Use PIL", true);
        static IGWidget::TextInputWithHint dllFileNameInput("##DllFileName", "lib file name");


        ImGui::Begin("FileViewControl", (bool*)0, IMGUI_WINDOW_FLAGS);
        if (ImGui::Button("Generate"))
        {
            FH::FileEntryVec& fileEntries = FH::GetFileEntriesRef();
            MG::GenerateMakeFile({ *cwi->selectedCompiler, *cwi->usePIL, *cwi->ccompilerFlags,
                * cwi->cppcompilerFlags,
                *cwi->linkLibraries, *cwi->makeFileOutput,
                *cwi->outputDir, *cwi->includeDirs,
                *cwi->libraryDirs, fileEntries, selectedBinFormat.SelectedItem(), dllFileNameInput.Input(), *cwi->outFileName});
        }



        ImGui::BeginMainMenuBar();
        if (ImGui::MenuItem("Save configuration"))
        {
            std::string filePath = FileDialog::SaveFileDialog();
            if (filePath != "")
            {
                FH::FileEntryVec& fileEntries = FH::GetFileEntriesRef();
                MG::SaveConfigFile({ *cwi->selectedCompiler, *cwi->usePIL, *cwi->ccompilerFlags,
                    *cwi->cppcompilerFlags,
                    *cwi->linkLibraries, *cwi->makeFileOutput,
                    *cwi->outputDir, *cwi->includeDirs,
                    *cwi->libraryDirs, fileEntries, selectedBinFormat.SelectedItem(), dllFileNameInput.Input(), *cwi->outFileName }, filePath, selectAll.Checked());
            }
        }
        if (ImGui::MenuItem("Load configuration"))
        {
            //std::string filePath = FileDialog::FileSelectionDialog();
            //MG::LoadConfigFile(filePath);
        }
        ImGui::EndMainMenuBar();

        
        selectedBinFormat.Selected(100.f, 150.f);

        if (selectAll.Changed(430.f))
            FH::SetSelectAllEntries(selectAll.Checked());

        if (deleteSelectedFiles.clicked(540.f))
            FH::DeleteSelectedEntries();

        if(undoButton.clicked(680.f))
            FH::UndoLastDelete(selectAll.Checked());


        usePIL.Changed(740.f);
        if (usePIL.IsHovered())
            usePIL.SetTooltip("Position independent linking");
        cwi->usePIL = &usePIL.Checked();


        if (selectedBinFormat.SelectedItem() == MG::BuildMode::DynamicLibrary)
            dllFileNameInput.Added(265.f, 150.f);

        ImGui::End();
    }


    void createFileView()
    {
        // make the background non transparent
        ImGui::SetNextWindowBgAlpha(1.f);
        IGW::Window* window = IGW::GetWindowPtr();
        if (window->hasResized())
        {
            std::pair<float, float> window_size = window->getSize();
            window->resized(false);
            ImVec2 ws(window_size.first, (window_size.second / 2.f) - sg_FileViewControlHight);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, ws.y + 90.f });
        }
        static FH::FileEntryVec& fileEntries = FH::GetFileEntriesRef();
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