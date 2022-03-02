#pragma once
#include <vector>

#include "ImGuiWindow.h"
#include "FileHandler.h"

namespace App {

	void runApplication();
}

namespace IGA {

    struct WidgetColor
    {
        constexpr WidgetColor(int flag, const ImVec4* const color)
            : colorFlag(flag), color(color) {}
        const int colorFlag = 0;
        const ImVec4* const color;
    };

    template<size_t S>
    void pushStyleColor(const std::array<WidgetColor, S>& colorFlags)
    {
        for (size_t i = 0; i < colorFlags.size(); ++i)
            ImGui::PushStyleColor(colorFlags[i].colorFlag, *colorFlags[i].color);
    }

    template<size_t S>
    void popStyleColor(const std::array<WidgetColor, S>& colorFlags)
    {
        for (size_t i = 0; i < colorFlags.size(); ++i)
            ImGui::PopStyleColor();
    }

	void createControlWindow();
	void createFileViewControl();
	void fillTestVector();
	void createFileView();

}

