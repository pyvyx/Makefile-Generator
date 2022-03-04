#pragma once
#define USING_IMGUI

// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"

namespace App {

    void startApplication()
    {
        #ifdef USING_IMGUI
            IGW::startWindow();
        #endif
    }
}