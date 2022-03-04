CXXCOMP=gcc
CXXFLAGS=
INCLUDEDIRS=
LIBRARYDIRS=
LIBRARIES=
EXE=myApp
OUTPUTFOLDER=Out
INTFOLDER=Out/BIN/
SRCFILESc=context.c egl_context.c init.c input.c monitor.c osmesa_context.c vulkan.c wgl_context.c win32_init.c win32_joystick.c win32_monitor.c win32_thread.c win32_time.c win32_window.c 
OBJFILESc=$(addprefix out/, $(notdir $(SRCFILESc:.c=.o)))

#Do not edit below this line
Build: $(OUTPUTFOLDER)/$(EXE): $(OBJFILESc) 
	$(OUTPUTFOLDER)/$(EXE): $(OBJFILESc)
	$(CXXCOMP) $(CXXFLAGS) $(OBJFILESc) -o $(OUTPUTFOLDER)/$(EXE) $(INCLUDEDIRS) $(LIBRARYDIRS) $(LIBRARIES)

out/%.o: %.c
	$(CXXCOMP) $(CXXFLAGS) -c $< -o $@ $(INCLUDEDIRS) $(LIBRARYDIRS) $(LIBRARIES)


