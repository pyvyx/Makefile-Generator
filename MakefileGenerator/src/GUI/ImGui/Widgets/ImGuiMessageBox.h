#pragma once
#include <vector>
#include <string>

#include "ImGui/imgui.h"

#include "ImGuiMessageBox.h"
#include "GUI/ImGui/Widgets/ImGuiButton.h"

#include "Application.h"

namespace IGWidget {

	class MessageBox
	{
	private:
		const char* m_Title;
		const char* m_Message;
		bool m_Show = true;
		uint8_t m_TitleColor;
		ImVec4 m_TitleColorVec;
		ImVec2 m_Size = { 594.f, 340.f };
		Button m_OkBtn = Button( "Ok", {385.f, 250.f}, {80.f, 40.f} );
		Button m_CancelBtn = Button("Cancel", { 480.f, 250.f }, { 80.f, 40.f });

		App::MessageBoxCallbacks m_Callbacks;

		inline static std::vector<MessageBox> sm_Boxes;
	private:
		void SetTitleBarColor(uint8_t color);
	public:
		static void RenderBoxes();
		MessageBox(const char* title, const char* message, const App::MessageBoxCallbacks& callbacks, uint8_t color = static_cast<uint8_t>(App::WidgetColor::NORMAL));
		MessageBox& operator=(const MessageBox& mb);

		void Show();
		void SetVisibility(bool show);
		bool IsVisible();
	};

}

