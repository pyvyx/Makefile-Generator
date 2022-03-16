#ifdef USING_IMGUI
#include <string>

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"

#include "ImGuiTextInputWithHint.h"
#include "GUI/ImGui/Widgets/ImGuiSizeProperties.h"


namespace IGWidget {

    TextInputWithHint::TextInputWithHint()
        : m_Label(""), m_Hint("") {};
    TextInputWithHint::TextInputWithHint(const char* label, const char* hint)
        : m_Label(label), m_Hint(hint) {}

    bool TextInputWithHint::added(const char* label, const char* hint)
    {
        ImGui::SetCursorPos(pos);
        ImGui::PushItemWidth(length);
        return ImGui::InputTextWithHint(label, hint, &input, ImGuiInputTextFlags_AllowTabInput);
    }

    bool TextInputWithHint::added()
    {
        ImGui::SetCursorPos(pos);
        ImGui::PushItemWidth(length);
        return ImGui::InputTextWithHint(m_Label, m_Hint, &input, ImGuiInputTextFlags_AllowTabInput);
    }

    bool TextInputWithHint::Added(float sameLineOffset, float width)
    {
        ImGui::SameLine(sameLineOffset);
        ImGui::PushItemWidth(width);
        return ImGui::InputTextWithHint(m_Label, m_Hint, &input, ImGuiInputTextFlags_AllowTabInput);
    }

    std::string& TextInputWithHint::Input() { return input; }
    void TextInputWithHint::SetInput(const std::string& str) { input = str; }

    void TextInputWithHint::resetPosSize(const ImVec2& ws, float y_offset)
    {
        // set text input position
        pos.x = sg_XPosTextInput;
        pos.y = IGWidget_Y_OFFSET_TEXT_INPUT(ws.y, y_offset);

        // set text input size
        length = IGWidget_X_SIZE_TEXT_INPUT(ws.x);
    }
}
#endif