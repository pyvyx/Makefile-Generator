#pragma once
#include <array>

#include "ImGui/imgui.h"

// GUI windows
#include "GUI/ImGui/ApplicationWindows/ImGuiControlWindow.h"
#include "GUI/ImGui/ApplicationWindows/ImGuiSelectionWindow.h"
#include "GUI/ImGui/ApplicationWindows/ImGuiFileViewWindow.h"
// Gui widgets
#include "GUI/ImGui/Widgets/ImGuiWidgetColor.h"

#include "Generator.h"


namespace IGA {

    class Application : private ControlWindow, private SelectionWindow, private FileViewWindow
    {
    private:
        MG::GeneratorInfo m_GenInfo;

        // for styling
        const ImVec4 m_ColorDark = { 0.27f, 0.27f, 0.27f, 1.0f };
        const ImVec4 m_ColorGrey = { 0.57f, 0.57f, 0.57f, 1.0f };
        const ImVec4 m_ColorWhite = {  1.f,   1.f,   1.f, 1.0f };
        const std::array<IGWidget::WidgetColor, 9> m_StyleColors = {
            IGWidget::WidgetColor(ImGuiCol_FrameBg,        &m_ColorDark),
            IGWidget::WidgetColor(ImGuiCol_FrameBgHovered, &m_ColorGrey),
            IGWidget::WidgetColor(ImGuiCol_CheckMark,      &m_ColorWhite),
            IGWidget::WidgetColor(ImGuiCol_Header,         &m_ColorDark),
            IGWidget::WidgetColor(ImGuiCol_HeaderActive,   &m_ColorDark),
            IGWidget::WidgetColor(ImGuiCol_HeaderHovered,  &m_ColorGrey),
            IGWidget::WidgetColor(ImGuiCol_Button,         &m_ColorDark),
            IGWidget::WidgetColor(ImGuiCol_ButtonHovered,  &m_ColorGrey),
            IGWidget::WidgetColor(ImGuiCol_ButtonActive,   &m_ColorGrey)
        };
    private:
        void Show() final override;
        void ShowMenuBar();
        void FillGeneratorInfo();
        void SetGeneratorInfo(const MG::GeneratorInfo& info);
    public:
        Application(const std::string& filePath);
        void Run();
    };

}