#ifdef USING_IMGUI
#include "GUI/ImGui/ImGuiApplication.h"
#include "GUI/ImGui/Widgets/ImGuiMessageBox.h"

// using ImGui --------------------------------------------------------
namespace IGA {

    Application::Application(const std::string& filePath) : SelectionWindow(&m_GenInfo), ControlWindow()  {
        if (filePath != "") {
            MG::GeneratorInfo info = MG::LoadConfigFile(filePath);
            SetGeneratorInfo(info);
        }
    }


    void Application::ShowMenuBar() {
        ImGui::BeginMainMenuBar();
        if (ImGui::MenuItem("Save configuration"))
        {
            FillGeneratorInfo();
            std::string filePath = FileDialog::SaveFileDialog();
            if (filePath != "")
                MG::SaveConfigFile(m_GenInfo, filePath);
        }
        if (ImGui::MenuItem("Load configuration"))
        {
            std::string filePath = FileDialog::FileSelectionDialog();
            if (filePath != "") {
                MG::GeneratorInfo info = MG::LoadConfigFile(filePath);
                SetGeneratorInfo(info);
            }
        }
        ImGui::EndMainMenuBar();
    }


    void Application::Run()
    {
        IGWidget::PushStyleColors(m_StyleColors);
        IGWidget::MessageBox::RenderBoxes();
        ShowMenuBar();
        ControlWindow::Show();
        Show();
        FileViewWindow::Show();
        IGWidget::PopStyleColors(m_StyleColors);
    }


    void Application::Show() {
        // Selection window
        ImGui::SetNextWindowBgAlpha(1.f);
        IGW::Window* window = IGW::GetWindowPtr();
        if (window->hasResized())
        {
            std::pair<float, float> window_size = window->getSize();
            ImVec2 ws(window_size.first, SELECTION_WINDOW_HEIGHT);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, (window_size.second / 2.f) });
        }

        ImGui::Begin("FileViewControl", (bool*)0, IMGUI_WINDOW_FLAGS);
        if (ImGui::Button("Generate"))
        {
            FillGeneratorInfo();
            MG::GenerateMakeFile(m_GenInfo);
        }

        m_SelectedBinFormat.Selected(100.f, 150.f);

        if (m_SelectAll.Changed(430.f))
            FH::SetSelectAllEntries(m_SelectAll.Checked());

        if (m_DeleteSelectedFiles.clicked(540.f))
            FH::DeleteSelectedEntries();

        if (m_UndoButton.clicked(680.f))
            FH::UndoLastDelete(m_SelectAll.Checked());


        m_UsePIL.Changed(740.f);
        if (m_UsePIL.IsHovered())
            m_UsePIL.SetTooltip("Position independent linking");


        if (m_SelectedBinFormat.SelectedItem() == MG::BuildMode::DynamicLibrary)
            m_DllFileNameInput.Added(265.f, 150.f);

        ImGui::End();
    }


    void Application::FillGeneratorInfo() {
        // Control window
        m_GenInfo.selectedCompiler = m_CompilerComboBox.selected;
        m_GenInfo.ccompilerFlags = m_CcompilerFlagsInput.input;
        m_GenInfo.cppcompilerFlags = m_CppcompilerFlagsInput.input;
        m_GenInfo.linkLibraries = m_LibrariesInput.input;
        m_GenInfo.makeFileOutput = m_MakeFileOutputPathInput.input;
        m_GenInfo.outputDir = m_OutputDirInput.input;
        m_GenInfo.includeDirs = m_IncludeDirsInput.input;
        m_GenInfo.libraryDirs = m_LibraryDirsInput.input;
        m_GenInfo.outFileName = m_OutputFileNameInput.input;

        // Selection window
        m_GenInfo.usePIL = m_UsePIL.Checked();
        m_GenInfo.selectedBinaryFormat = m_SelectedBinFormat.SelectedItem();
        m_GenInfo.dllFileName = m_DllFileNameInput.Input();

        // File view
        FH::FileEntryVec& fileEntries = FH::GetFileEntriesRef();
        m_GenInfo.files = fileEntries;
    }


    void Application::SetGeneratorInfo(const MG::GeneratorInfo& info) {
        // Control window
        m_CompilerComboBox.SetSelectedItem(info.selectedCompiler);
        m_CcompilerFlagsInput.SetInput(info.ccompilerFlags);
        m_CppcompilerFlagsInput.SetInput(info.cppcompilerFlags);
        m_LibrariesInput.SetInput(info.linkLibraries);
        m_MakeFileOutputPathInput.SetInput(info.makeFileOutput);
        m_OutputDirInput.SetInput(info.outputDir);
        m_IncludeDirsInput.SetInput(info.includeDirs);
        m_LibraryDirsInput.SetInput(info.libraryDirs);
        m_OutputFileNameInput.SetInput(info.outFileName);
        m_SetMakeFileOutputPath = true;

        // Selection window
        m_UsePIL.SetState(info.usePIL);
        m_SelectedBinFormat.SetSelectedItem(info.selectedBinaryFormat);
        m_DllFileNameInput.SetInput(info.dllFileName);

        // File view
        FH::FileEntryVec& fileEntries = FH::GetFileEntriesRef();
        fileEntries = info.files;
    }
}
#endif