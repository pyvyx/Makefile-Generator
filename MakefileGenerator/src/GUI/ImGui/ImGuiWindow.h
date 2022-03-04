#pragma once
#include <utility>
#include "GLFW/glfw3.h"

namespace IGW {

	class Window
	{
	private:
		GLFWwindow* m_Window;
		bool m_Resized = true;
	public:
		// 1280 720 / 700 650
		Window(int width = 930, int height = 700, const char* title = "Makefile-Generator", GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
		~Window();

		inline void vsync(bool state) const { glfwSwapInterval(state); }
		inline bool hasResized()	  const { return m_Resized;		   }
		inline void resized(bool state)		{ m_Resized = state;       }

		// loop
		inline bool isOpen()     const	{ return !glfwWindowShouldClose(m_Window);	}
		inline void swap()       const	{ glfwSwapBuffers(m_Window);				}
		inline void clear()      const	{ glClear(GL_COLOR_BUFFER_BIT);				}
		inline void pollEvents() const	{ glfwPollEvents();							}
		inline void waitEvents() const	{ glfwWaitEvents();							}

		std::pair<float, float> getSize() const;

		// ImGui
		void imGuiInit(const char* iniFileName) const;
		void imGuiStartFrame() const;
		void imGuiRender() const;
	};

	extern Window* g_Window;
	void startWindow();
}
// Poll and handle events (inputs, window resize, etc.)
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
