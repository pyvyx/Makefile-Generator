#pragma once
#include <array>

#include "ImGui/imgui.h"

#include "GUI/ImGui/Widgets/ImGuiWidgetColor.h"

namespace IGA {

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

    void StartApplication();
    void createControlWindow(ControlWindowInfo* cwi);
    void createFileViewControl(ControlWindowInfo* cwi);
    void createFileView();

}