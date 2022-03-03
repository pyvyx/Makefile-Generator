#include <string>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"

#include "ImGuiTextInputWithHint.h"


namespace IGWidget {

    TextInputWithHint::TextInputWithHint()
        : m_Label(""), m_Hint("") {};
    TextInputWithHint::TextInputWithHint(const char* label, const char* hint)
        : m_Label(label), m_Hint(hint) {}

    bool TextInputWithHint::used(const char* label, const char* hint)
    {
        ImGui::SetCursorPos(pos);
        ImGui::PushItemWidth(length);
        return ImGui::InputTextWithHint(label, hint, &input, ImGuiInputTextFlags_AllowTabInput);
    }

    bool TextInputWithHint::used()
    {
        ImGui::SetCursorPos(pos);
        ImGui::PushItemWidth(length);
        return ImGui::InputTextWithHint(m_Label, m_Hint, &input, ImGuiInputTextFlags_AllowTabInput);
    }

    void TextInputWithHint::resetPosSize(const ImVec2& ws, float y_offset)
    {
        // set text input position
        pos.x = X_POS_TXT_IP;
        pos.y = Y_OFF_TXT_IP(y_offset);

        // set text input size
        length = X_SIZE_TXT_IP;
    }

}