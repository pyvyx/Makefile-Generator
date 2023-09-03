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
        "../Dependencies/GLFW/include",
        "../Dependencies/ImGui/include",
        "../Dependencies/nativefiledialog/include"
    }

    externalincludedirs {
        "vendor",
        "../Dependencies/ImGui/include"
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
            "opengl32",
            "shell32",
            "ole32",
            "uuid"
        }

    filter "system:linux"
        links {
            "GL",
            "gtk-3",
            "glib-2.0",
            "gobject-2.0",
            "X11"
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
            "sign-promo",
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
            "format-nonliteral",
            "ignored-qualifiers",
            "sign-conversion",
            "missing-declarations"
        }


    filter { "toolset:gcc* or toolset:clang*", "system:macosx" }
            disablewarnings {
                "deprecated-copy-with-user-provided-dtor",
                "deprecated-copy-with-user-provided-copy"
            }


    filter "toolset:gcc*"
        warnings "Extra"
        externalwarnings "Off"
        linkgroups "on" -- activate position independent linking
        enablewarnings {
            "noexcept",
            "strict-null-sentinel",
            "array-bounds=2",
            "duplicated-cond",
            "logical-op",
            "arith-conversion",
            "stringop-overflow=4",
            "implicit-fallthrough=3",
            "trampolines"
        }

    filter "toolset:clang*"
        warnings "Extra"
        externalwarnings "Everything"
        enablewarnings {
            "array-bounds",
            "long-long",
            "implicit-fallthrough", 
        }
        disablewarnings {
            "deprecated-copy-with-user-provided-dtor",
            "deprecated-copy-with-user-provided-copy"
        }
    filter {}


    filter { "configurations:Debug" }
        kind "ConsoleApp"
    filter { "configurations:Release" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
