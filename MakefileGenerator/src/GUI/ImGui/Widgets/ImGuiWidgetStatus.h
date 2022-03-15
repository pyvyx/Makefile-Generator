#pragma once
#include "ImGui/imgui.h"

namespace IGWidget {

	class WidgetStatus
	{
	public:
		inline bool IsHovered() const { return ImGui::IsItemHovered(); }
		inline void SetTooltip(const char* tooltip) const { ImGui::SetTooltip(tooltip); }
	};

}