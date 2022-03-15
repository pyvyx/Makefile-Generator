#pragma once
#include <array>

#include "ImGui/imgui.h"

// GUI windows
#include "GUI/ImGui/ImGuiControlWindow.h"
#include "GUI/ImGui/ImGuiSelectionWindow.h"
#include "GUI/ImGui/ImGuiFileViewWindow.h"
// Gui widgets
#include "GUI/ImGui/Widgets/ImGuiWidgetColor.h"

#include "Generator.h"


namespace IGA {

    class Application
    {
    private:
        MG::GeneratorInfo m_GenInfo;
        ControlWindow m_ControlWindow;
        SelectionWindow m_SelectionWindow;
        FileViewWindow m_FileViewWindow;

        bool m_WillSaveConfig = false;

        // for styling
        const ImVec4 m_ColorDark = { 0.27f, 0.27f, 0.27f, 1.0f };
        const ImVec4 m_ColorGrey = { 0.57f, 0.57f, 0.57f, 1.0f };
        const ImVec4 m_ColorWhite = {  1.f,   1.f,   1.f, 1.0f };
        const std::array<IGWidget::WidgetColor, 9> m_StyleColors = {
            IGWidget::WidgetColor(ImGuiCol_FrameBg,        &m_ColorDark),
            IGWidget::WidgetColor(ImGuiCol_FrameBgHovered, &m_ColorGrey),
            IGWidget::WidgetColor(ImGuiCol_CheckMark,      &m_ColorWhite),
            IGWidget::WidgetColor(ImGuiCol_Header,         &m_ColorDark),
            IGWidget::WidgetColor(ImGuiCol_HeaderActive,   &m_ColorDark),
            IGWidget::WidgetColor(ImGuiCol_HeaderHovered,  &m_ColorGrey),
            IGWidget::WidgetColor(ImGuiCol_Button,         &m_ColorDark),
            IGWidget::WidgetColor(ImGuiCol_ButtonHovered,  &m_ColorGrey),
            IGWidget::WidgetColor(ImGuiCol_ButtonActive,   &m_ColorGrey)
        };
    private:
        void ShowMenuBar() {
            ImGui::BeginMainMenuBar();
            if (ImGui::MenuItem("Save configuration") || m_WillSaveConfig)
            {
                if (!m_WillSaveConfig) {
                    m_WillSaveConfig = true;
                    // just to be able to still see this button
                    ImGui::MenuItem("Load configuration");
                    ImGui::EndMainMenuBar();
                    return;
                }
                std::string filePath = FileDialog::SaveFileDialog();
                if (filePath != "")
                    MG::SaveConfigFile(m_GenInfo, filePath);
                m_WillSaveConfig = false;
            }
            if (ImGui::MenuItem("Load configuration"))
            {
                std::string filePath = FileDialog::FileSelectionDialog();
                MG::GeneratorInfo info = MG::LoadConfigFile(filePath);
                m_SelectionWindow.SetGeneratorInfo(info);
                m_ControlWindow.SetGeneratorInfo(info);
            }
            ImGui::EndMainMenuBar();
        }
    public:
        Application() : m_SelectionWindow(&m_GenInfo) {}

        void Run()
        {
            if (m_SelectionWindow.WillGenerate() || m_WillSaveConfig) {
                m_ControlWindow.FillGeneratorInfo(&m_GenInfo);
                m_SelectionWindow.FillGeneratorInfo(&m_GenInfo);
            }

            IGWidget::PushStyleColors(m_StyleColors);
            ShowMenuBar();
            m_ControlWindow.Show();
            m_SelectionWindow.Show();
            m_FileViewWindow.Show();
            IGWidget::PopStyleColors(m_StyleColors);
        }
    };



    //struct ControlWindowInfo
    //{
    //    int* selectedCompiler = nullptr;
    //    int selectedBinaryFormat = 0;
    //    bool* usePIL = nullptr;
    //    std::string* ccompilerFlags = nullptr;
    //    std::string* cppcompilerFlags = nullptr;
    //    std::string* linkLibraries = nullptr;
    //    std::string* makeFileOutput = nullptr;
    //    std::string* outputDir = nullptr;
    //    std::string* includeDirs = nullptr;
    //    std::string* libraryDirs = nullptr;
    //    std::string* outFileName = nullptr;
    //};

    //void StartApplication();
    //void createControlWindow(ControlWindowInfo* cwi);
    //void createFileViewControl(ControlWindowInfo* cwi);
    //void createFileView();

}