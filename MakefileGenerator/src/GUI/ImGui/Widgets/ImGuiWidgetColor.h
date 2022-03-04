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

}