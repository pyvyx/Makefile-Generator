project "MakefileGenerator"
    language "C++"
    cppdialect "C++17"

    files {
        "src/**.cpp",
        "src/**.h"
    }

    includedirs {
        "src",
        "vendor",
        "../Dependencies/glfw/include",
        "../Dependencies/imgui/include",
        "../Dependencies/nativefiledialog/include"
    }

    externalincludedirs {
        "vendor",
        "../Dependencies/imgui/include"
    }

    flags "FatalWarnings"
    
    links {
        "glfw",
        "nativefiledialog",
        "ImGui"
    }

    filter "system:windows"
        links {
            "gdi32",
            "opengl32"
        }

    filter "system:macosx"
        defines "GL_SILENCE_DEPRECATION"
        linkoptions "-framework AppKit -framework iokit -framework OpenGl"

    -- gcc* clang* msc*
    filter "toolset:msc*"
        warnings "High"
        externalwarnings "Default" -- Default
        disablewarnings {}
        buildoptions { "/sdl" }
        defines "MSC"

    filter { "toolset:gcc* or toolset:clang*" }
        enablewarnings {
            "cast-align",
            "cast-qual",
            "ctor-dtor-privacy",
            "disabled-optimization",
            "format=2",
            "init-self",
            "missing-declarations",
            "missing-include-dirs",
            "overloaded-virtual",
            "redundant-decls",
            "shadow",
            "sign-promo",
            "strict-overflow=5",
            "switch-default",
            "undef",
            "uninitialized",
            "unreachable-code",
            "unused",
            "alloca",
            "conversion",
            "deprecated",
            "null-dereference",
            "stack-protector",
            "vla",
            "shift-overflow"
        }
        disablewarnings { 
            "unused-parameter",
            "format-security",
            "deprecated-copy-with-user-provided-dtor",
            "deprecated-copy-with-user-provided-copy",
            "ignored-qualifiers",
            "sign-conversion" 
        }

    filter "toolset:gcc*"
        warnings "Extra"
        externalwarnings "Off"
        linkgroups "on" -- activate position independent linking
        enablewarnings {
            "noexcept",
            "strict-null-sentinel",
            "array-bounds=2",
            "duplicated-branches",
            "duplicated-cond",
            "logical-op",
            "arith-conversion",
            "stringop-overflow=4",
            "implicit-fallthrough=3",
            "trampolines"
        }
        defines "GCC"

    filter "toolset:clang*"
        warnings "Extra"
        externalwarnings "Everything"
        enablewarnings {
            "array-bounds",
            "long-long",
            "implicit-fallthrough", 
        }
        defines "CLANG"
    filter {}


    filter { "configurations:Debug" }
        kind "ConsoleApp"
    filter { "configurations:Release" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
