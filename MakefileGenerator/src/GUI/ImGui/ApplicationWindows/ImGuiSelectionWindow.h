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

#define SELECTION_WINDOW_HEIGHT 45.f

namespace IGA {

    class SelectionWindow
    {
    public:
        bool m_WillGenerate = false;
        MG::GeneratorInfo* m_GenInfo = nullptr;
        IGWidget::Button m_DeleteSelectedFiles = IGWidget::Button("Delete selected");
        IGWidget::Button m_UndoButton = IGWidget::Button("Undo");

        IGWidget::ComboBox m_SelectedBinFormat = IGWidget::ComboBox("##SelectBinaryFormat", "Application\0Static library\0Dynamic library\0");
        IGWidget::CheckBox m_SelectAll = IGWidget::CheckBox("Select all");
        IGWidget::CheckBox m_UsePIL = IGWidget::CheckBox("Use PIL", true);
        IGWidget::TextInputWithHint m_DllFileNameInput = IGWidget::TextInputWithHint("##DllFileName", "lib file name");
    public:
        SelectionWindow(MG::GeneratorInfo* const info) : m_GenInfo(info) {}
    };
}