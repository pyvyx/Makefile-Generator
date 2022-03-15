#pragma once
#include "GUI/ImGui/Widgets/ImGuiWidgetStatus.h"

namespace IGWidget {

	class CheckBox : public WidgetStatus
	{
	private:
		const char* m_Label = nullptr;
		bool m_Checked = false;
	public:
		CheckBox() = default;
		CheckBox(const char* label, bool checked = false);
		bool Changed(float sameLineOffset);
		bool& Checked();
		void SetState(bool checked);
	};

}