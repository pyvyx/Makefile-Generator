#include "ImGui/imgui.h"

#include "ImGuiComboBox.h"
#include "GUI/ImGui/Widgets/ImGuiSizeProperties.h"

namespace IGWidget {

    ComboBox::ComboBox() : label(""), items("") {}
    ComboBox::ComboBox(const char* label, const char* items) : label(label), items(items) {}

    bool ComboBox::Selected(const char* label_p, const char* items_p)
    {
        ImGui::SetCursorPos(pos);
        ImGui::PushItemWidth(length);
        return ImGui::Combo(label_p, &selected, items_p);
    }

    bool ComboBox::Selected()
    {
        ImGui::SetCursorPos(pos);
        ImGui::PushItemWidth(length);
        return ImGui::Combo(label, &selected, items);
    }

    bool ComboBox::Selected(float sameLineOffset, float width)
    {
        ImGui::SameLine(sameLineOffset);
        ImGui::PushItemWidth(width);
        return ImGui::Combo(label, &selected, items);
    }

    int ComboBox::SelectedItem() { return selected; }

    void ComboBox::resetPosSize(const ImVec2& ws, float y_offset)
    {
        // set combo box pos
        pos.x = IGWidget_BTN_OFFSET_X(ws.x);
        pos.y = IGWidget_BTN_OFFSET_Y(ws.y, y_offset);

        // set combo box size
        length = sg_ButtonSize.x;
    }

}