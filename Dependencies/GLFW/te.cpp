#include "include/GLFW/glfw3.h"
                         //Important note: GLEW must NEVER be included after 
                         //gl.h is already included(which is included in glew.h 
                         //and glfw3.h) so if you want to include one of these
                         //headers again, wrap them
                         //into #ifndef __gl_h_ directives just to be sure

GLFWwindow* window;      //This is the GLFW handle for a window, it is used in several 
                         //GLFW functions, so make sure it is accessible

int main()
{
    if(!glfwInit())             //Inititalizes the library
        return 1;

    glfwDefaultWindowHints();   //See second example

    window = glfwCreateWindow(100, 100, "er", NULL, NULL);
    //Creates a window with the following parameters:
    // + int width: Width of the window to be created
    // + int height: Height of the window to be created
    // + const char* title: Title of the window to be created
    // + GLFWmonitor* monitor: If this parameter is non-NULL, the window will be 
    //   created in fullscreen mode, covering the specified monitor. Monitors can be  
    //   queried using either glfwGetPrimaryMonitor() or glfwGetMonitors()
    // + GLFWwindow* share: For more information about this parameter, please
    //   consult the documentation

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    while(true)
    {
        glfwPollEvents();
    }
    //Creates a OpenGL context for the specified window
    //Specifies how many monitor-refreshes GLFW should wait, before swapping the 
    return 0;
    //Initializes GLEW
}