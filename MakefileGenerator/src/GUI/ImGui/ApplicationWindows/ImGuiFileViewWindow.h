#pragma once

#include "ImGui/imgui.h"

// GUI includes
#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/ImGui/ApplicationWindows/ImGuiSelectionWindow.h"

#include "FileHandler.h"

namespace IGA {

	class FileViewWindow
	{
	public:
		virtual ~FileViewWindow() = default;
		virtual void Show();
	};

}