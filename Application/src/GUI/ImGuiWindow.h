#pragma once
#include <iostream>

#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

extern const unsigned int Arial_compressed_size;
extern const unsigned int Arial_compressed_data[];

namespace IGW {

	class Window
	{
	private:
		GLFWwindow* m_Window;
		bool m_Resized = true;
	public:
		// 1280 720 / 700 650
		Window(int width = 930, int height = 660, const char* title = "Makefile-Generator", GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
		~Window();

		inline void vsync(bool state) const { glfwSwapInterval(state); }
		inline bool isOpen() const { return !glfwWindowShouldClose(m_Window); }
		inline void resized(bool state = false) { m_Resized = state; }
		inline bool hasResized() { return m_Resized; }
		inline std::pair<float, float> getSize() const
		{
			int width, height;
			glfwGetWindowSize(m_Window, &width, &height);
			return { static_cast<float>(width), static_cast<float>(height) };
		}

		// loop
		inline void swap() const { glfwSwapBuffers(m_Window); }
		inline void clear() const { glClear(GL_COLOR_BUFFER_BIT); }
		inline void pollEvents() const { glfwPollEvents(); }
		inline void waitEvents() const { glfwWaitEvents(); }

		// ImGui
		inline void imGuiInit(const char* iniFileName = NULL) const
		{
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontFromMemoryCompressedTTF(Arial_compressed_data, Arial_compressed_size, 19);
			//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			io.IniFilename = iniFileName;

			ImGuiStyle& style = ImGui::GetStyle();
			style.FrameRounding = 5.f;

			ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
			ImGui_ImplOpenGL3_Init("#version 130");
			ImGui::StyleColorsDark();
		}

		inline void imGuiStartFrame() const
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		inline void imGuiShowDemoWindow() const { ImGui::ShowDemoWindow(); }

		inline void imGuiRender() const
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			//ImGuiIO& io = ImGui::GetIO();
			//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			//{
			//	GLFWwindow* backup_current_context = glfwGetCurrentContext();
			//	ImGui::UpdatePlatformWindows();
			//	ImGui::RenderPlatformWindowsDefault();
			//	glfwMakeContextCurrent(backup_current_context);
			//}
		}
	};

	extern Window* sg_Window;
	void runWindow();

}
// Poll and handle events (inputs, window resize, etc.)
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
