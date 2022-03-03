#include "ImGui/imgui.h"

#include "ImGuiButton.h"
#include "GUI/ImGui/Widgets/ImGuiSizeProperties.h"

namespace IGWidget {

    Button::Button() : m_Label("") {}
    Button::Button(const char* label) : m_Label(label) {}

    bool Button::clicked(const char* label)
    {
        ImGui::SetCursorPos(pos);
        return ImGui::Button(label, size);
    }

    bool Button::clicked()
    {
        //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.33f, 0.33f, 0.33f, 1.0f });
        ImGui::SetCursorPos(pos);
        return ImGui::Button(m_Label, size);
    }

    void Button::resetPosSize(const ImVec2& ws, float y_offset)
    {
        // set button pos
        pos.x = IGWidget_BTN_OFFSET_X(ws.x);
        pos.y = IGWidget_BTN_OFFSET_Y(ws.y, y_offset);

        // set button size
        size.x = sg_ButtonSize.x;
        size.y = sg_ButtonSize.y;
    }

}