CCOMP=gcc
CFLAGS=-fPIC -D USING_IMGUI -D _GLFW_WIN32 -Ofast -s  -static-libgcc -static-libstdc++
CXXCOMP=g++
CXXFLAGS=-fPIC -D USING_IMGUI -Ofast -s -Wl,--subsystem,windows  -static-libgcc -static-libstdc++
INCLUDEDIRS=-IDependencies/GLFW/include/ -IDependencies/ImGui/include/ImGui/ -IDependencies/ImGui/include/ -IDependencies/nativefiledialog/include/ -IDependencies/nativefiledialog/include/nfd/ -IMakefileGenerator/vendor/ -IMakefileGenerator/src/ 
LIBRARYDIRS=
LIBRARIES=-lopengl32 -lgdi32 -lole32 -luuid
EXE=Makefile-Generator.exe
SHAREDLIB=libmake.a
OUTPUTFOLDER=Out
INTFOLDER=Out\BIN
RESFILES=MakefileGenerator/icon/gcc.res 

SRCFILESc=Dependencies/GLFW/src/context.c Dependencies/GLFW/src/egl_context.c Dependencies/GLFW/src/init.c Dependencies/GLFW/src/input.c Dependencies/GLFW/src/monitor.c Dependencies/GLFW/src/osmesa_context.c Dependencies/GLFW/src/vulkan.c Dependencies/GLFW/src/wgl_context.c Dependencies/GLFW/src/win32_init.c Dependencies/GLFW/src/win32_joystick.c Dependencies/GLFW/src/win32_monitor.c Dependencies/GLFW/src/win32_thread.c Dependencies/GLFW/src/win32_time.c Dependencies/GLFW/src/win32_window.c Dependencies/GLFW/src/window.c Dependencies/nativefiledialog/src/nfd_common.c 
OBJFILESc=$(addprefix $(INTFOLDER)/, $(notdir $(SRCFILESc:.c=.o)))
SRCFILEScpp=MakefileGenerator/src/FileHandler.cpp MakefileGenerator/src/Generator.cpp MakefileGenerator/src/main.cpp MakefileGenerator/src/GUI/ImGui/ImGuiApplication.cpp MakefileGenerator/src/GUI/ImGui/ImGuiWindow.cpp MakefileGenerator/src/GUI/ImGui/Widgets/ImGuiButton.cpp MakefileGenerator/src/GUI/ImGui/Widgets/ImGuiCheckBox.cpp MakefileGenerator/src/GUI/ImGui/Widgets/ImGuiComboBox.cpp MakefileGenerator/src/GUI/ImGui/Widgets/ImGuiTextInputWithHint.cpp MakefileGenerator/src/GUI/FileDialog/FileDialog.cpp Dependencies/ImGui/src/imgui.cpp Dependencies/ImGui/src/imgui_demo.cpp Dependencies/ImGui/src/imgui_draw.cpp Dependencies/ImGui/src/imgui_impl_glfw.cpp Dependencies/ImGui/src/imgui_impl_opengl3.cpp Dependencies/ImGui/src/imgui_stdlib.cpp Dependencies/ImGui/src/imgui_tables.cpp Dependencies/ImGui/src/imgui_widgets.cpp Dependencies/nativefiledialog/src/example.cpp Dependencies/nativefiledialog/src/nfd_win.cpp MakefileGenerator/src/Application.cpp MakefileGenerator/src/GUI/ImGui/ApplicationWindows/ImGuiControlWindow.cpp MakefileGenerator/src/GUI/ImGui/ApplicationWindows/ImGuiFileViewWindow.cpp MakefileGenerator/src/GUI/ImGui/Widgets/ImGuiMessageBox.cpp 
OBJFILEScpp=$(addprefix $(INTFOLDER)/, $(notdir $(SRCFILEScpp:.cpp=.o)))

#Do not edit below this line
.PHONY: build clean rebuild

build: $(OUTPUTFOLDER) $(INTFOLDER) $(OUTPUTFOLDER)/$(EXE)

rebuild: clean build

$(OUTPUTFOLDER)/$(EXE): $(OBJFILESc) $(OBJFILEScpp) 
	$(CXXCOMP) -shared -Wl,--out-implib,$(OUTPUTFOLDER)/$(SHAREDLIB) $(LIBRARIES) $(OBJFILESc) $(OBJFILEScpp) -o $(OUTPUTFOLDER)/$(EXE) $(LIBRARYDIRS) $(LIBRARIES) $(LIBRARIES)


$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/Widgets/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/ImGui/src/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/icon/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/FileDialog/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/GLFW/src/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/nativefiledialog/src/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/ApplicationWindows/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)


$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/Widgets/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/ImGui/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/icon/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/FileDialog/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/GLFW/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/nativefiledialog/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/ApplicationWindows/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)


$(OUTPUTFOLDER): 
	mkdir $(OUTPUTFOLDER)
$(INTFOLDER): 
	mkdir $(INTFOLDER)

clean: 
	rmdir /s /q $(OUTPUTFOLDER)