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

    struct ControlWindowInfo
    {
        int* selectedCompiler = nullptr;
        int selectedBinaryFormat = 0;
        bool* usePIL = nullptr;
        std::string* ccompilerFlags = nullptr;
        std::string* cppcompilerFlags = nullptr;
        std::string* linkLibraries = nullptr;
        std::string* makeFileOutput = nullptr;
        std::string* outputDir = nullptr;
        std::string* includeDirs = nullptr;
        std::string* libraryDirs = nullptr;
        std::string* outFileName = nullptr;
    };

    void startApplication();
    void createControlWindow(ControlWindowInfo* cwi);
    void createFileViewControl(ControlWindowInfo* cwi);
    void fillTestVector();
    void createFileView();

}