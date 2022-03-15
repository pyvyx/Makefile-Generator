#pragma once

#include "ImGui/imgui.h"

namespace IGWidget {

    class TextInputWithHint
    {
    public:
        ImVec2 pos;
        float length = 100.f;
        std::string input;
        const char* m_Label;
        const char* m_Hint;
    public:
        TextInputWithHint();
        TextInputWithHint(const char* label, const char* hint);

        bool added(const char* label, const char* hint);
        bool added();
        bool Added(float sameLineOffset, float width);
        std::string& Input();
        void SetInput(const std::string& str);
        void resetPosSize(const ImVec2& ws, float y_offset);
    };

}