#ifdef USING_IMGUI
#include "ImGui/imgui.h"

#include "ImGuiMessageBox.h"
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/ImGui/Widgets/ImGuiButton.h"

namespace IGWidget {

	void MessageBox::RenderBoxes() {
		for (size_t i = 0; i < sm_Boxes.size(); ++i) {
			sm_Boxes[i].Show();
			if (!sm_Boxes[i].IsVisible()) {
				sm_Boxes.erase(sm_Boxes.begin() + i);
			}
		}
	}


	void MessageBox::SetTitleBarColor(uint8_t color) {
		m_TitleColor = color;

		switch (m_TitleColor) 
		{
		case static_cast<uint8_t>(App::WidgetColor::GREEN):
			m_TitleColorVec = {	 0.21f, 0.63f, 0.21f, 1.f };
			break;
		case static_cast<uint8_t>(App::WidgetColor::YELLOW):
			m_TitleColorVec = { 0.83f, 0.83f, 0.18f, 1.f };
			break;
		case static_cast<uint8_t>(App::WidgetColor::RED):
			m_TitleColorVec = { 0.71f, 0.13f, 0.13f, 1.f };
			break;
		default:
			//m_TitleColorVec = { 0.27f, 0.27f, 0.27f, 1.f };
			m_TitleColorVec = { 0.44f, 0.44f, 0.44f, 1.f };
			break;
		}
	}


	MessageBox::MessageBox(const char* title, const std::string& message, const App::MessageBoxCallbacks& callbacks, uint8_t color)
		: m_Title(title), m_Message(message), m_Callbacks(callbacks), m_TitleColor(color) {
		SetTitleBarColor(color);
		sm_Boxes.push_back(*this);
	}


	MessageBox& MessageBox::operator=(const MessageBox& mb) {
		m_Title = mb.m_Title;
		m_Message = mb.m_Message;
		m_Show = mb.m_Show;
		m_TitleColor = mb.m_TitleColor;
		m_Size = mb.m_Size;
		m_OkBtn = mb.m_OkBtn;
		return *this;
	}


	void MessageBox::Show() {
		if (!m_Show)
			return;

		ImGui::SetNextWindowBgAlpha(1.f);
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowSize(m_Size);

		static bool firstTime = true;
		IGW::Window* window = IGW::GetWindowPtr();
		if (firstTime || window->hasResized()) {
			// resize and reposition the window
			std::pair<float, float> windowSize = window->getSize();
			float xpos = (windowSize.first - m_Size.x) / 2;
			float ypos = (windowSize.second - m_Size.y) / 2;
			ImGui::SetNextWindowPos(ImVec2(xpos, ypos));
			firstTime = false;
		}

		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, m_TitleColorVec);
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 10.f;

		ImGui::Begin(m_Title, (bool*)0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::TextWrapped(m_Message.c_str());

		if (m_OkBtn.clicked()) {
			if(m_Callbacks.m_OnOkClicked != nullptr)
				m_Callbacks.m_OnOkClicked(m_Callbacks.m_OnOkParam);
			m_Show = false;
		}

		if (m_CancelBtn.clicked())
		{
			if (m_Callbacks.m_OnCancelClicked != nullptr)
				m_Callbacks.m_OnCancelClicked(m_Callbacks.m_OnCancelParam);
			m_Show = false;
		}

		ImGui::End();

		style.WindowRounding = 0.f;
		ImGui::PopStyleColor();
	}


	void MessageBox::SetVisibility(bool show) {
		m_Show = show;
	}


	bool MessageBox::IsVisible() {
		return m_Show;
	}

}
#endif