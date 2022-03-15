#pragma once

// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/FileDialog/FileDialog.h"
// GUI widgets
#include "GUI/ImGui/Widgets/ImGuiButton.h"
#include "GUI/ImGui/Widgets/ImGuiTextInputWithHint.h"
#include "GUI/ImGui/Widgets/ImGuiComboBox.h"
#include "GUI/ImGui/Widgets/ImGuiSizeProperties.h"

#include "FileHandler.h"
#include "Generator.h"


#define IMGUI_WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar

namespace IGA {

    class ControlWindow
    {
    private:
        bool m_SetMakeFileOutputPath = false;
        std::array<IGWidget::Button*, 6> m_Buttons;
        std::array<IGWidget::TextInputWithHint*, 9> m_TextInputs;

        IGWidget::TextInputWithHint m_OutputFileNameInput = IGWidget::TextInputWithHint("##NameOutputFile", "Set a name for the binary (libMyName.so/.dll for shared libs, libMyName.a/.lib for static libs)");

        IGWidget::ComboBox m_CompilerComboBox = IGWidget::ComboBox("##CompilerCombo", "gcc | g++\0clang | clang++\0gcc\0g++\0clang\0clang++\0Other (Cf)\0");
        IGWidget::TextInputWithHint m_CcompilerFlagsInput = IGWidget::TextInputWithHint("##CFLAGS", "C compiler flags e.G. myccompiler -O2 -o MyOut");
        IGWidget::TextInputWithHint m_CppcompilerFlagsInput = IGWidget::TextInputWithHint("##CXXFLAGS", "C++ compiler flags e.G. myccompiler -O2 -o MyOut");

        IGWidget::Button m_SelectMakeFileOutputPath = IGWidget::Button("Select file output##SelectFileOutput");
        IGWidget::TextInputWithHint m_MakeFileOutputPathInput = IGWidget::TextInputWithHint("##FileOutput", "Set an output directory for the generated makefile");

        IGWidget::Button m_SelectOutputDir = IGWidget::Button("Select output dir##SelectOutputDir");
        IGWidget::TextInputWithHint m_OutputDirInput = IGWidget::TextInputWithHint("##OutputDir", "Set an output directory for the binary files");

        IGWidget::Button m_SelectIncludeDirs = IGWidget::Button("Select directories##SelectIncludeDirs");
        IGWidget::TextInputWithHint m_IncludeDirsInput = IGWidget::TextInputWithHint("##IncludeDirs", "Add additional include directories");

        IGWidget::Button m_SelectLibraries = IGWidget::Button("Select libraries");
        IGWidget::TextInputWithHint m_LibrariesInput = IGWidget::TextInputWithHint("##Libraries", "Link libraries e.G. -lpthread");

        IGWidget::Button m_SelectLibraryDirs = IGWidget::Button("Select directories##SelectLibraryDirs");
        IGWidget::TextInputWithHint m_LibraryDirsInput = IGWidget::TextInputWithHint("##LibraryDirs", "Add additional library directories");

        IGWidget::Button m_SelectFiles = IGWidget::Button("Select files");
        IGWidget::TextInputWithHint m_SearchInput = IGWidget::TextInputWithHint("##SearchFiles", "Search files");
    private:
        void Resize()
        {
            static const ImVec2 window_pos(0.f, 25.f);
            IGW::Window* window = IGW::GetWindowPtr();
            std::pair<float, float> window_size = window->getSize();
            ImVec2 ws(window_size.first, window_size.second / 2.f - 25.f);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos(window_pos);

            float btn_offset = ((ws.y - ((m_TextInputs.size()) * IGWidget::sg_ButtonSize.y)) / ((m_TextInputs.size() + 1))) + IGWidget::sg_ButtonSize.y;
            float offset = btn_offset;
            for (size_t i = 0; i < m_Buttons.size(); ++i)
            {
                m_Buttons[i]->resetPosSize(ws, offset);
                offset += btn_offset;
            }
            m_CompilerComboBox.resetPosSize(ws, offset + btn_offset - IGWidget::sg_TextInputYOffset);

            offset = btn_offset - IGWidget::sg_TextInputYOffset;
            for (size_t i = 0; i < m_TextInputs.size(); ++i)
            {
                m_TextInputs[i]->resetPosSize(ws, offset);
                offset += btn_offset;
            }
        }
    public:
        ControlWindow() {
            m_Buttons = { &m_SelectFiles, &m_SelectLibraryDirs, &m_SelectIncludeDirs, &m_SelectOutputDir, &m_SelectMakeFileOutputPath, &m_SelectLibraries };
            m_TextInputs = { &m_SearchInput, &m_LibraryDirsInput, &m_IncludeDirsInput, &m_OutputDirInput, &m_MakeFileOutputPathInput, &m_LibrariesInput, &m_CppcompilerFlagsInput, &m_CcompilerFlagsInput, &m_OutputFileNameInput };
        }

        void SetGeneratorInfo(const MG::GeneratorInfo& info) {
            m_CompilerComboBox.SetSelectedItem(info.selectedCompiler);
            m_CcompilerFlagsInput.SetInput(info.ccompilerFlags);
            m_CppcompilerFlagsInput.SetInput(info.cppcompilerFlags);
            m_LibrariesInput.SetInput(info.linkLibraries);
            m_MakeFileOutputPathInput.SetInput(info.makeFileOutput);
            m_OutputDirInput.SetInput(info.outputDir);
            m_IncludeDirsInput.SetInput(info.includeDirs);
            m_LibraryDirsInput.SetInput(info.libraryDirs);
            m_OutputFileNameInput.SetInput(info.outFileName);

            if(m_MakeFileOutputPathInput.input != "")
                m_SetMakeFileOutputPath = true;
        }

        void FillGeneratorInfo(MG::GeneratorInfo* const info) {
            info->selectedCompiler = m_CompilerComboBox.selected;   
            info->ccompilerFlags = m_CcompilerFlagsInput.input;
            info->cppcompilerFlags = m_CppcompilerFlagsInput.input;
            info->linkLibraries = m_LibrariesInput.input;
            info->makeFileOutput = m_MakeFileOutputPathInput.input;
            info->outputDir = m_OutputDirInput.input;
            info->includeDirs = m_IncludeDirsInput.input;
            info->libraryDirs = m_LibraryDirsInput.input;
            info->outFileName = m_OutputFileNameInput.input;
        }

        void Show() {
            ImGui::SetNextWindowBgAlpha(1.f);
            IGW::Window* window = IGW::GetWindowPtr();
            if (window->hasResized())
                Resize();

            ImGui::Begin("ControlWindow", (bool*)0, IMGUI_WINDOW_FLAGS);

            if (m_SelectMakeFileOutputPath.clicked())
            {
                if (FileDialog::FolderSelectionDialog(nullptr, &m_MakeFileOutputPathInput.input, true))
                    m_SetMakeFileOutputPath = true;
            }

            if (m_SetMakeFileOutputPath && m_MakeFileOutputPathInput.input != "")
            {
                m_OutputFileNameInput.added();
                m_MakeFileOutputPathInput.added();

                // binary output directory
                if (m_SelectOutputDir.clicked())
                {
                    FileDialog::FolderSelectionDialog(&m_MakeFileOutputPathInput.input, &m_OutputDirInput.input, true);
                }
                m_OutputDirInput.added();

                // compiler combo box
                m_CompilerComboBox.Selected();

                // compiler flags
                m_CcompilerFlagsInput.added();
                m_CppcompilerFlagsInput.added();

                // include directories
                if (m_SelectIncludeDirs.clicked())
                {
                    FileDialog::FolderSelectionDialog(&m_MakeFileOutputPathInput.input, &m_IncludeDirsInput.input, false);
                }
                m_IncludeDirsInput.added();

                // link libraries
                m_LibrariesInput.added();

                // library directories
                if (m_SelectLibraryDirs.clicked())
                {
                    FileDialog::FolderSelectionDialog(&m_MakeFileOutputPathInput.input, &m_LibraryDirsInput.input, false);
                }
                m_LibraryDirsInput.added();

                // files
                if (m_SelectFiles.clicked())
                {
                    FileDialog::MultiFileDialog(&m_MakeFileOutputPathInput.input);
                }

                if (m_SearchInput.added())
                    FH::FilterFileEntries(m_SearchInput.input);
            }

            ImGui::End();
        }
    };

}