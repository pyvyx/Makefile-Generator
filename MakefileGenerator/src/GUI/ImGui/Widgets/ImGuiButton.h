#pragma once

#include "ImGui/imgui.h"


namespace IGWidget {

    class Button
    {
    public:
        ImVec2 pos;
        ImVec2 size;
        const char* m_Label;
    public:
        Button();
        Button(const char* label);
        Button(const char* label, const ImVec2& pos, const ImVec2& size);

        bool clicked(const char* label);
        bool clicked() const;
        bool clicked(float sameLineOffset);
        void resetPosSize(const ImVec2& ws, float y_offset);
    };

}