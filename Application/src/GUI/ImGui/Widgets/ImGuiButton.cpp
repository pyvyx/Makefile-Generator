#include "ImGui/imgui.h"

#include "ImGuiButton.h"

namespace IGWidget {

    Button::Button() : m_Label("") {}
    Button::Button(const char* label) : m_Label(label) {}

    bool Button::used(const char* label)
    {
        ImGui::SetCursorPos(pos);
        return ImGui::Button(label, size);
    }

    bool Button::used()
    {
        //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.33f, 0.33f, 0.33f, 1.0f });
        ImGui::SetCursorPos(pos);
        return ImGui::Button(m_Label, size);
    }

    void Button::resetPosSize(const ImVec2& ws, float y_offset)
    {
        // set button pos
        pos.x = X_OFFSET_BTN;
        pos.y = Y_OFFSET_BTN(y_offset);

        // set button size
        size.x = X_SIZE_BTN;
        size.y = Y_SIZE_BTN;
    }

}