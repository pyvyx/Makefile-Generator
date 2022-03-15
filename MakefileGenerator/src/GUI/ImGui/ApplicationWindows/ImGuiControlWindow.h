#pragma once
#include <array>

// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/FileDialog/FileDialog.h"
// GUI widgets
#include "GUI/ImGui/Widgets/ImGuiButton.h"
#include "GUI/ImGui/Widgets/ImGuiTextInputWithHint.h"
#include "GUI/ImGui/Widgets/ImGuiComboBox.h"
#include "GUI/ImGui/Widgets/ImGuiSizeProperties.h"

namespace IGA {

    class ControlWindow
    {
    public:
        bool m_SetMakeFileOutputPath = false;
        std::array<IGWidget::Button*, 5> m_Buttons;
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

        IGWidget::TextInputWithHint m_LibrariesInput = IGWidget::TextInputWithHint("##Libraries", "Link libraries e.G. -lpthread");
        IGWidget::Button m_SelectLibraryDirs = IGWidget::Button("Select directories##SelectLibraryDirs");
        IGWidget::TextInputWithHint m_LibraryDirsInput = IGWidget::TextInputWithHint("##LibraryDirs", "Add additional library directories");

        IGWidget::Button m_SelectFiles = IGWidget::Button("Select files");
        IGWidget::TextInputWithHint m_SearchInput = IGWidget::TextInputWithHint("##SearchFiles", "Search files");
    private:
        void Resize();
    public:
        ControlWindow();
        virtual void Show();
    };

}