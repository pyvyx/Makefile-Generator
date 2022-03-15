#include "ImGui/imgui.h"

#include "ImGuiCheckBox.h"

namespace IGWidget {

	CheckBox::CheckBox(const char* label, bool checked)
		: m_Label(label), m_Checked(checked) {}

	bool CheckBox::Changed(float sameLineOffset)
	{
		ImGui::SameLine(sameLineOffset);
		return ImGui::Checkbox(m_Label, &m_Checked);
	}

	bool& CheckBox::Checked() { return m_Checked; }
	void CheckBox::SetState(bool checked) { m_Checked = checked; }
}