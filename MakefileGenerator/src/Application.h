#pragma once
#include <string>
#include <cstdint>
#include <functional>

namespace App {

    enum class WidgetColor {
        RED, YELLOW, GREEN, NORMAL
    };

    struct MessageBoxCallbacks {
        std::function<void(void*)> m_OnOkClicked = nullptr;
        std::function<void(void*)> m_OnCancelClicked = nullptr;
        void* m_OnOkParam = nullptr, * m_OnCancelParam = nullptr;
    };

    bool StartApplication(bool usingGui, const std::string& filePath);
    void NotifyUser(const char* type, const std::string& message, const MessageBoxCallbacks& callbacks, uint8_t color = static_cast<uint8_t>(WidgetColor::NORMAL));
}