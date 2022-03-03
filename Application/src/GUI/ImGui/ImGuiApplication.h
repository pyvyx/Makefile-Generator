#pragma once
#include <array>

#include "ImGui/imgui.h"

#include "GUI/ImGui/Widgets/ImGuiWidgetColor.h"

namespace IGA {

    template<size_t S>
    void pushStyleColor(const std::array<IGWidget::WidgetColor, S>& colorFlags)
    {
        for (size_t i = 0; i < colorFlags.size(); ++i)
            ImGui::PushStyleColor(colorFlags[i].colorFlag, *colorFlags[i].color);
    }

    template<size_t S>
    void popStyleColor(const std::array<IGWidget::WidgetColor, S>& colorFlags)
    {
        for (size_t i = 0; i < colorFlags.size(); ++i)
            ImGui::PopStyleColor();
    }

    void startApplication();
    void createControlWindow();
    void createFileViewControl();
    void fillTestVector();
    void createFileView();

}