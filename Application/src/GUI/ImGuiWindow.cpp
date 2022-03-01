#include <iostream>

#include "GLFW/glfw3.h"

#include "ImGuiWindow.h"
#include "Application.h"

#include "Timer.h"
namespace IGW {

    Window* sg_Window = nullptr;
    static char sg_WindowEventHappened = 0;

    void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        sg_Window->resized(true);
        sg_WindowEventHappened = 2;
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        sg_WindowEventHappened = 2;
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        sg_WindowEventHappened = 2;
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        sg_WindowEventHappened = 2;
    }

    //public
    Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
        : m_Window(nullptr)
    {
        glfwSetErrorCallback(glfw_error_callback);
        
        if (!glfwInit())
        {
            std::cout << "[ERROR] Glfw Init failed\n";
            return;
        }

        // Create window with graphics context
        m_Window = glfwCreateWindow(width, height, title, monitor, share);
        if (m_Window == NULL)
        {
            std::cout << "[ERROR] glfwCreateWindow returned a NULL pointer\n";
            return;
        }

        // center the window
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(m_Window, (mode->width - width) / 2, (mode->height - height) / 2);

        sg_Window = this;
        glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
        glfwSetKeyCallback(m_Window, key_callback);
        glfwSetCursorPosCallback(m_Window, cursor_position_callback);
        glfwSetFramebufferSizeCallback(m_Window, glfw_framebuffer_size_callback);
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1); // Enable vsync
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
    }


    Window::~Window()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }


    void runWindow()
    {
        IGW::Window window;
        window.vsync(true);
        window.imGuiInit(NULL);

        Timer timer;
        timer.start();
        IGA::fillTestVector();

        // Main loop
        while (window.isOpen())
        {
            window.clear();
            window.imGuiStartFrame();

            //timer.measureFrameTime_FPS();
            IGA::createControlWindow();
            IGA::createFileView();


            //window.imGuiShowDemoWindow();
            window.imGuiRender();
            if (sg_WindowEventHappened) {
                window.pollEvents();
                --sg_WindowEventHappened;
            }
            else
                window.waitEvents();
            window.swap();
        }
    }
}