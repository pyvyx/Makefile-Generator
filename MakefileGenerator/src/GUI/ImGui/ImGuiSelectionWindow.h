#pragma once
// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/FileDialog/FileDialog.h"
// GUI widgets
#include "GUI/ImGui/Widgets/ImGuiButton.h"
#include "GUI/ImGui/Widgets/ImGuiTextInputWithHint.h"
#include "GUI/ImGui/Widgets/ImGuiComboBox.h"
#include "GUI/ImGui/Widgets/ImGuiCheckBox.h"

#include "FileHandler.h"
#include "Generator.h"

#define IMGUI_WINDOW_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
#define SELECTION_WINDOW_HEIGHT 45.f

namespace IGA {

    class SelectionWindow
    {
    private:
        bool m_WillGenerate = false;
        MG::GeneratorInfo* const m_GenInfo;
        IGWidget::Button m_DeleteSelectedFiles = IGWidget::Button("Delete selected");
        IGWidget::Button m_UndoButton = IGWidget::Button("Undo");

        IGWidget::ComboBox m_SelectedBinFormat = IGWidget::ComboBox("##SelectBinaryFormat", "Application\0Static library\0Dynamic library\0");
        IGWidget::CheckBox m_SelectAll = IGWidget::CheckBox("Select all");
        IGWidget::CheckBox m_UsePIL = IGWidget::CheckBox("Use PIL", true);
        IGWidget::TextInputWithHint m_DllFileNameInput = IGWidget::TextInputWithHint("##DllFileName", "lib file name");
    public:
        SelectionWindow(MG::GeneratorInfo* const info) : m_GenInfo(info) {}

        inline bool WillGenerate() {
            return m_WillGenerate;
        }

        void SetGeneratorInfo(const MG::GeneratorInfo& info) {
            FH::FileEntryVec& fileEntries = FH::GetFileEntriesRef();
            fileEntries = info.files;
            m_UsePIL.SetState(info.usePIL);
            m_SelectedBinFormat.SetSelectedItem(info.selectedBinaryFormat);
            m_DllFileNameInput.SetInput(info.dllFileName);
            m_SelectAll.SetState(info.selectedAll);
        }

        void FillGeneratorInfo(MG::GeneratorInfo* const info) {
            FH::FileEntryVec& fileEntries = FH::GetFileEntriesRef();
            info->usePIL = m_UsePIL.Checked();
            info->selectedBinaryFormat = m_SelectedBinFormat.SelectedItem();
            info->dllFileName = m_DllFileNameInput.Input();
            info->files = fileEntries;
            info->selectedAll = m_SelectAll.Checked();
        }

        void Show() {
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
            if (ImGui::Button("Generate") || WillGenerate())
            {
                if (!WillGenerate()) {
                    m_WillGenerate = true;
                    ImGui::End();
                    return;
                }
                MG::GenerateMakeFile(*m_GenInfo);
                m_WillGenerate = false;
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
    };
}