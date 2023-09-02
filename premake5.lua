workspace "MakefileGenerator"
    configurations {
        "Debug",
        "Release"
    }
    startproject "MakefileGenerator"

outputdir = "/BIN/%{cfg.toolset}/%{cfg.shortname}/%{prj.name}/"
cwd = os.getcwd() -- get current working directory
targetdir(cwd .. outputdir .. "bin")
objdir(cwd .. outputdir .. "bin-int")

filter "system:windows"
    platforms { "x64", "x86" }
    defines "WINDOWS"
filter "system:linux"
    platforms "x64"
    defines "LINUX"
filter "system:macosx"
    defines "MAC_OS"

filter { "platforms:x64" }
    architecture "x64"
    defines "X64"
filter { "platforms:x86" }
    architecture "x86"
    defines "X86"

filter { "configurations:Debug" }
    runtime "Debug"
    symbols "on"
    optimize "off"
    defines "DEBUG"
filter { "configurations:Release" }
    runtime "Release"
    symbols "off"
    optimize "Speed"
    defines "RELEASE"
filter {}

-- only for visual studio
flags {
    "MultiProcessorCompile"
}
staticruntime "on"
removeunreferencedcodedata "on"
defines "USING_IMGUI"

include "MakefileGenerator"
include "Dependencies/glfw"
include "Dependencies/imgui"
include "Dependencies/nativefiledialog"
