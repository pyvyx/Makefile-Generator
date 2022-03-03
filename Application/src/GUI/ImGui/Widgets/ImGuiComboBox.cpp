#include "ImGui/imgui.h"

#include "ImGuiComboBox.h"

namespace IGWidget {

    ComboBox::ComboBox() : label(""), items("") {}
    ComboBox::ComboBox(const char* label, const char* items) : label(label), items(items) {}

    bool ComboBox::used(const char* label_p, const char* items_p)
    {
        ImGui::SetCursorPos(pos);
        ImGui::PushItemWidth(length);
        return ImGui::Combo(label_p, &selected, items_p);
    }

    bool ComboBox::used()
    {
        ImGui::SetCursorPos(pos);
        ImGui::PushItemWidth(length);
        return ImGui::Combo(label, &selected, items);
    }

    void ComboBox::resetPosSize(const ImVec2& ws, float y_offset)
    {
        // set combo box pos
        pos.x = X_OFFSET_BTN;
        pos.y = Y_OFFSET_BTN(y_offset);

        // set combo box size
        length = X_SIZE_BTN;
    }

}