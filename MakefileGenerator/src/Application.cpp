#include "Application.h"

// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"

#include "Generator.h"

namespace App {

    bool StartApplication(bool usingGui, const std::string& filePath)
    {
        if (!usingGui) {
            MG::GeneratorInfo info = MG::LoadConfigFile(filePath);
            MG::GenerateMakeFile(info);
            return true;
        }

        #ifdef USING_IMGUI
            return IGW::StartWindow(filePath);
        #endif
    }

}