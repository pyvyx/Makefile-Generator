#pragma once

#include "ImGui/imgui.h"

#define X_OFFSET_BTN    ws.x - 170.f
#define Y_OFFSET_BTN(x) ws.y - x

#define X_SIZE_BTN      150.f
#define Y_SIZE_BTN      40.f

#define X_POS_TXT_IP    10.f
#define X_SIZE_TXT_IP   ws.x - 200.f
#define Y_OFF_TXT_IP(x) ws.y - x

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

        bool used(const char* label, const char* hint);
        bool used();
        void resetPosSize(const ImVec2& ws, float y_offset);
    };

}