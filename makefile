CXXCOMP=g++
CCOMP=gcc
CXXFLAGS=-D _GLFW_WIN32 -Wl,--subsystem,windows -std=c++17 -O2 -s
INCLUDEDIRS=-IDependencies/ImGui/include/ImGui/ -IDependencies/ImGui/include/ -IDependencies/nativefiledialog/include/nfd/ -IDependencies/nativefiledialog/include/ -IDependencies/GLFW/include/ -IMakefileGenerator/src/ -IMakefileGenerator/vendor/ 
LIBRARYDIRS=
LIBRARIES=-lopengl32 -lgdi32 -lole32 -luuid
EXE=MakefileGenerator
OUTPUTFOLDER=Out
INTFOLDER=Out\BIN

SRCFILEScpp=Dependencies/ImGui/src/imgui.cpp Dependencies/ImGui/src/imgui_demo.cpp Dependencies/ImGui/src/imgui_draw.cpp Dependencies/ImGui/src/imgui_impl_glfw.cpp Dependencies/ImGui/src/imgui_impl_opengl3.cpp Dependencies/ImGui/src/imgui_stdlib.cpp Dependencies/ImGui/src/imgui_tables.cpp Dependencies/ImGui/src/imgui_widgets.cpp Dependencies/nativefiledialog/src/example.cpp Dependencies/nativefiledialog/src/nfd_win.cpp MakefileGenerator/src/FileHandler.cpp MakefileGenerator/src/Generator.cpp MakefileGenerator/src/main.cpp MakefileGenerator/src/Timer.cpp MakefileGenerator/src/GUI/ImGui/ImGuiApplication.cpp MakefileGenerator/src/GUI/ImGui/ImGuiWindow.cpp MakefileGenerator/src/GUI/ImGui/Widgets/ImGuiButton.cpp MakefileGenerator/src/GUI/ImGui/Widgets/ImGuiComboBox.cpp MakefileGenerator/src/GUI/ImGui/Widgets/ImGuiTextInputWithHint.cpp MakefileGenerator/src/GUI/FileDialog/FileDialog.cpp 
OBJFILEScpp=$(addprefix $(INTFOLDER)/, $(notdir $(SRCFILEScpp:.cpp=.o)))
SRCFILESc=Dependencies/GLFW/src/context.c Dependencies/GLFW/src/egl_context.c Dependencies/GLFW/src/init.c Dependencies/GLFW/src/input.c Dependencies/GLFW/src/monitor.c Dependencies/GLFW/src/osmesa_context.c Dependencies/GLFW/src/vulkan.c Dependencies/GLFW/src/wgl_context.c Dependencies/GLFW/src/win32_init.c Dependencies/GLFW/src/win32_joystick.c Dependencies/GLFW/src/win32_monitor.c Dependencies/GLFW/src/win32_thread.c Dependencies/GLFW/src/win32_time.c Dependencies/GLFW/src/win32_window.c Dependencies/GLFW/src/window.c Dependencies/nativefiledialog/src/nfd_common.c 
OBJFILESc=$(addprefix $(INTFOLDER)/, $(notdir $(SRCFILESc:.c=.o)))

#Do not edit below this line
Build: $(OUTPUTFOLDER) $(INTFOLDER) $(OUTPUTFOLDER)/$(EXE)

$(OUTPUTFOLDER)/$(EXE): $(OBJFILEScpp) $(OBJFILESc) 
	$(CXXCOMP) $(CXXFLAGS) $(OBJFILEScpp) $(OBJFILESc)  -o $(OUTPUTFOLDER)/$(EXE) $(INCLUDEDIRS) $(LIBRARYDIRS) $(LIBRARIES)

$(INTFOLDER)/%.o: Dependencies/GLFW/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/Widgets/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/ImGui/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/nativefiledialog/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/FileDialog/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)


$(INTFOLDER)/%.o: Dependencies/GLFW/src/%.c
	$(CCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/Widgets/%.c
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/%.c
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/ImGui/src/%.c
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: Dependencies/nativefiledialog/src/%.c
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/ImGui/%.c
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)

$(INTFOLDER)/%.o: MakefileGenerator/src/GUI/FileDialog/%.c
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS)


$(OUTPUTFOLDER): 
	mkdir $(OUTPUTFOLDER)
$(INTFOLDER): 
	mkdir $(INTFOLDER)

clean: 
	rmdir /s /q $(OUTPUTFOLDER)