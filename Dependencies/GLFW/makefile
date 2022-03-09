CCOMP=gcc
CFLAGS=-D _GLFW_WIN32
CXXCOMP=g++
CXXFLAGS=
INCLUDEDIRS=
LIBRARYDIRS=
LIBRARIES=-lopengl32 -lgdi32
EXE=libglfw.so
DLLLIB=libglfw.a
OUTPUTFOLDER=Out
INTFOLDER=Out\BIN
RESFILES=

SRCFILESc=src/context.c src/egl_context.c src/init.c src/input.c src/monitor.c src/osmesa_context.c src/vulkan.c src/wgl_context.c src/win32_init.c src/win32_joystick.c src/win32_monitor.c src/win32_thread.c src/win32_time.c src/win32_window.c src/window.c 
OBJFILESc=$(addprefix $(INTFOLDER)/, $(notdir $(SRCFILESc:.c=.o)))

#Do not edit below this line
.PHONY: build clean rebuild

build: $(OUTPUTFOLDER) $(INTFOLDER) $(OUTPUTFOLDER)/$(EXE)

rebuild: clean build

$(OUTPUTFOLDER)/$(EXE): $(OBJFILESc) 
	$(CXXCOMP) -shared -Wl,--out-implib,$(OUTPUTFOLDER)/$(DLLLIB) $(LIBRARIES) $(OBJFILESc) -o $(OUTPUTFOLDER)/$(EXE) $(LIBRARYDIRS) $(LIBRARIES) $(LIBRARIES)


$(INTFOLDER)/%.o: src/%.c
	$(CCOMP) $(CFLAGS) -c $< -o $@ $(INCLUDEDIRS)


$(OUTPUTFOLDER): 
	mkdir $(OUTPUTFOLDER)
$(INTFOLDER): 
	mkdir $(INTFOLDER)

clean: 
	rmdir /s /q $(OUTPUTFOLDER)