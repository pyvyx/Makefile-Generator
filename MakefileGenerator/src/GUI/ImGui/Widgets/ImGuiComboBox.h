#pragma once

#include "ImGui/imgui.h"


namespace IGWidget {

    class ComboBox
    {
    public:
        ImVec2 pos;
        float length = 100.f;
        int selected = 0;
        const char* label;
        const char* items;
    public:
        ComboBox();
        ComboBox(const char* label, const char* items);

        bool Selected(const char* label_p, const char* items_p);
        bool Selected();
        bool Selected(float sameLineOffset, float width);
        int SelectedItem();
        void resetPosSize(const ImVec2& ws, float y_offset);
    };

}