SOURCE_FILES=Dependencies/GLFW/src/context.c Dependencies/GLFW/src/egl_context.c Dependencies/GLFW/src/egl_context.h Dependencies/GLFW/src/init.c Dependencies/GLFW/src/input.c Dependencies/GLFW/src/internal.h Dependencies/GLFW/src/mappings.h Dependencies/GLFW/src/monitor.c Dependencies/GLFW/src/osmesa_context.c Dependencies/GLFW/src/osmesa_context.h Dependencies/GLFW/src/vulkan.c Dependencies/GLFW/src/wgl_context.c Dependencies/GLFW/src/wgl_context.h Dependencies/GLFW/src/win32_init.c Dependencies/GLFW/src/win32_joystick.c Dependencies/GLFW/src/win32_joystick.h Dependencies/GLFW/src/win32_monitor.c Dependencies/GLFW/src/win32_platform.h Dependencies/GLFW/src/win32_thread.c Dependencies/GLFW/src/win32_time.c Dependencies/GLFW/src/win32_window.c Dependencies/GLFW/src/window.c 
OBJ_FILES=$(addprefix out/, $(notdir $(SOURCE_FILES:.c=.o)))
Build: $(OBJ_FILES)

out/%.o: Dependencies/GLFW/src/%.c
	clang -D _GLFW_WIN32 -Ofast -c $< -o $@ -IDependencies/GLFW/include -IDependencies/GLFW/include/GLFW 


out/%.h:
	$(info er)