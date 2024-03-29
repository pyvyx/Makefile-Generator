#ifdef USING_IMGUI
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "GUI/ImGui/ImGuiWindow.h"
#include "GUI/ImGui/ImGuiApplication.h"
#include "GUI/Icon.h"
#include "GUI/Arial.h"

namespace IGW {

    static char sg_WindowEventHappened = 0;
    static bool sg_ErrorOccured = false;

    //public
    Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
        : m_Window(nullptr)
    {
        glfwSetErrorCallback([](int error, const char* description){ fprintf(stderr, "Glfw Error %d: %s\n", error, description); });

        if (!glfwInit())
        {
            std::cout << "[ERROR] Glfw Init failed\n";
            return;
        }

        // Create window with graphics context
        m_Window = glfwCreateWindow(width, height, title, monitor, share);
        if (m_Window == NULL)
        {
            std::cout << "[ERROR] glfwCreateWindow failed\n";
            return;
        }
        
        // center the window
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(m_Window, (mode->width - width) / 2, (mode->height - height) / 2);

        // set window icon
        GLFWimage icon_s;
        icon_s.pixels = stbi_load_from_memory(sg_RawIconData, sg_IconSize, &icon_s.width, &icon_s.height, NULL, 4);
        glfwSetWindowIcon(m_Window, 1, &icon_s);
        stbi_image_free(icon_s.pixels);

        // set window pointer and callbacks
        Window* window = GetWindowPtr();
        if (sg_ErrorOccured)
            return;
        *window = *this;

        glfwSetCursorPosCallback      (m_Window, [](GLFWwindow*, double, double)        { sg_WindowEventHappened = 2; });
        glfwSetMouseButtonCallback    (m_Window, [](GLFWwindow*, int, int, int)         { sg_WindowEventHappened = 2; });
        glfwSetKeyCallback            (m_Window, [](GLFWwindow*, int, int, int, int)    { sg_WindowEventHappened = 2; });
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow*, int width, int height) { glViewport(0, 0, width, height); Window* window = GetWindowPtr(); window->resized(true); sg_WindowEventHappened = 2; });
        
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1); // Enable vsync
        glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
    }


    Window::~Window()
    {
        // Cleanup
        if (!sg_ErrorOccured) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }


    std::pair<float, float> Window::getSize() const
    {
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
        return { static_cast<float>(width), static_cast<float>(height) };
    }


    void Window::imGuiInit(const char* iniFileName) const
    {
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        //io.ConfigFlags |= ImGuiPopupFlags;
        io.Fonts->AddFontFromMemoryCompressedTTF(sg_ArialCompressedData, sg_ArialCompressedSize, 19);
        io.IniFilename = iniFileName;

        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 5.f;
        //style.WindowRounding = 5.f;

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();
    }


    void Window::imGuiStartFrame() const
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }


    void Window::imGuiRender() const
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


    Window* GetWindowPtr()
    {
        static Window* window = (Window*)::operator new(sizeof(Window), std::nothrow);
        if (window == nullptr) {
            sg_ErrorOccured = true;
        } // inform the user
        return window;
    }


    void FreeWindowPtr()
    {
        ::operator delete(GetWindowPtr());
    }


    bool StartWindow(const std::string& filePath)
    {
        IGW::Window window;
        if (sg_ErrorOccured)
            return false;

        window.imGuiInit(NULL);
        IGA::Application app(filePath);

        // Main loop
        while (window.isOpen())
        {
            window.clear();
            window.imGuiStartFrame();

            app.Run();

            window.imGuiRender();
            if (sg_WindowEventHappened) {
                window.pollEvents();
                --sg_WindowEventHappened;
            }
            else
                window.waitEvents();
            window.swap();
        }
        FreeWindowPtr();
        return true;
    }
}
#endif