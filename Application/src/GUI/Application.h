#pragma once
#define USING_IMGUI

// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"

namespace App {

    void runApplication()
    {
        #ifdef USING_IMGUI
            IGW::runWindow();
        #endif
    }
}