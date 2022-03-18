#include <iostream>

// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/ImGui/Widgets/ImGuiMessageBox.h"

#include "Application.h"
#include "Generator.h"

namespace App {

    static bool sg_UsingGui;

    bool StartApplication(bool usingGui, const std::string& filePath)
    {
        sg_UsingGui = usingGui;
        if (!usingGui) {
            MG::GeneratorInfo info = MG::LoadConfigFile(filePath);
            MG::GenerateMakeFile(info);
            return true;
        }

        #ifdef USING_IMGUI
            return IGW::StartWindow(filePath);
        #endif

        return false;
    }


    void NotifyUser(const char* type, const char* message, const MessageBoxCallbacks& callbacks, uint8_t color) {

        if (!sg_UsingGui) {
            // std::endl is intentional
            std::cout << type << ' ' << message << std::endl;
        }

        #ifdef USING_IMGUI
            IGWidget::MessageBox(type, message, callbacks, color);
        #endif
    }

}