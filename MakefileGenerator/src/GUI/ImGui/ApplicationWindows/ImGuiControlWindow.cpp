#include "ImGuiControlWindow.h"

#include "FileHandler.h"

namespace IGA {

    //public
    ControlWindow::ControlWindow() 
        : m_Buttons({ &m_SelectFiles, &m_SelectLibraryDirs, &m_SelectIncludeDirs, &m_SelectOutputDir, &m_SelectMakeFileOutputPath }),
        m_TextInputs({ &m_SearchInput, &m_LibraryDirsInput, &m_IncludeDirsInput, &m_OutputDirInput, &m_MakeFileOutputPathInput, &m_LibrariesInput, &m_CppcompilerFlagsInput, &m_CcompilerFlagsInput, &m_OutputFileNameInput })
        {}


    void ControlWindow::Show() {
        ImGui::SetNextWindowBgAlpha(1.f);
        IGW::Window* window = IGW::GetWindowPtr();
        if (window->hasResized())
            Resize();

        ImGui::Begin("ControlWindow", (bool*)0, IMGUI_WINDOW_FLAGS);

        if (m_SelectMakeFileOutputPath.clicked())
            FileDialog::FolderSelectionDialog(nullptr, &m_MakeFileOutputPathInput.input, true);


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
        
        ImGui::End();
    }


    // private
	void ControlWindow::Resize()
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
        offset += btn_offset;
        m_CompilerComboBox.resetPosSize(ws, offset + btn_offset - IGWidget::sg_TextInputYOffset);

        offset = btn_offset - IGWidget::sg_TextInputYOffset;
        for (size_t i = 0; i < m_TextInputs.size(); ++i)
        {
            m_TextInputs[i]->resetPosSize(ws, offset);
            offset += btn_offset;
        }
    }

}