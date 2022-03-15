#include "ImGuiFileViewWindow.h"

namespace IGA {

	void FileViewWindow::Show() {
        // make the background non transparent
        ImGui::SetNextWindowBgAlpha(1.f);
        IGW::Window* window = IGW::GetWindowPtr();
        if (window->hasResized())
        {
            std::pair<float, float> window_size = window->getSize();
            window->resized(false);
            ImVec2 ws(window_size.first, (window_size.second / 2.f) - SELECTION_WINDOW_HEIGHT);
            ImGui::SetNextWindowSize(ws);
            ImGui::SetNextWindowPos({ 0.f, ws.y + 90.f });
        }
        static FH::FileEntryVec& fileEntries = FH::GetFileEntriesRef();
        size_t fileEntriesSize = fileEntries.size();

        ImGui::Begin("FileView", (bool*)0, IMGUI_WINDOW_FLAGS);
        ImGui::Columns(1);

        if (fileEntriesSize != 0)
            ImGui::Separator();


        for (size_t i = 0; i < fileEntriesSize; ++i)
        {
            if (!fileEntries[i].isDeleted() && fileEntries[i].isShown())
            {
                ImGui::Selectable(fileEntries[i].fileName().c_str(), (bool*)&fileEntries[i].getSelectedRef());
                ImGui::Separator();
            }
        }

        ImGui::End();
	}
}