CXXCOMP=g++
CXXFLAGS=
INCLUDEDIRS=-IDependencies/ImGui/include/ImGui/ -IDependencies/GLFW/include/ 
LIBRARYDIRS=
LIBRARIES=
EXE=ImGui
OUTPUTFOLDER=Builds
INTFOLDER=Builds\BIN

SRCFILEScpp=Dependencies/ImGui/src/imgui.cpp Dependencies/ImGui/src/imgui_demo.cpp Dependencies/ImGui/src/imgui_draw.cpp Dependencies/ImGui/src/imgui_impl_glfw.cpp Dependencies/ImGui/src/imgui_impl_opengl3.cpp Dependencies/ImGui/src/imgui_stdlib.cpp Dependencies/ImGui/src/imgui_tables.cpp Dependencies/ImGui/src/imgui_widgets.cpp 
OBJFILEScpp=$(addprefix $(INTFOLDER)/, $(notdir $(SRCFILEScpp:.cpp=.o)))

#Do not edit below this line
Build: $(OUTPUTFOLDER) $(INTFOLDER) $(OUTPUTFOLDER)/$(EXE)

$(OUTPUTFOLDER)/$(EXE): $(OBJFILEScpp) 
	ar rcs $(OUTPUTFOLDER)/lib$(EXE).a $(OBJFILEScpp) 

$(INTFOLDER)/%.o: Dependencies/ImGui/src/%.cpp
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS) $(LIBRARYDIRS) $(LIBRARIES)


$(OUTPUTFOLDER): 
	mkdir $(OUTPUTFOLDER)
$(INTFOLDER): 
	mkdir $(INTFOLDER)

clean: 
	rmdir /s /q $(OUTPUTFOLDER)