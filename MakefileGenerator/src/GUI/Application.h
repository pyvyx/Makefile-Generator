#pragma once
#define USING_IMGUI

// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"

namespace App {

    void StartApplication()
    {
        #ifdef USING_IMGUI
            IGW::StartWindow();
        #endif
    }
}