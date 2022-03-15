#pragma once

#include "ImGui/imgui.h"

namespace IGWidget {

    struct WidgetColor
    {
        constexpr WidgetColor(int flag, const ImVec4* const color)
            : colorFlag(flag), color(color) {}
        const int colorFlag = 0;
        const ImVec4* const color;
    };

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

}