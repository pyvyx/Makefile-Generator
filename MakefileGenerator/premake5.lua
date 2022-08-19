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

    flags "FatalWarnings"

    links {
        "glfw",
        "nativefiledialog",
        "ImGui",
        "gdi32",
        "opengl32"
    }

    -- gcc* clang* msc*
    --filter "toolset:msc*"
    --    warnings "Everything"
    --    externalwarnings "Default" -- Default
    --    disablewarnings { 
    --        "4820", -- disable warning C4820: 'added padding'
    --        "4626", -- C6264 assignment operator was deleted
    --        "5027", -- C5027 move assignment operator was deleted
    --        "5045", -- C5045 Spectre mitigation
    --        "4710", -- C4710 function not inlined
    --        "4711", -- C4711 function 'function' selected for automatic inline expansion
    --        "4191" -- C4191 pointer conversion e.g. reinterpret_cast is unsafe
    --        --"4201"  -- C4201 nonstandard extension used: nameless struct/union
    --    }
    --    buildoptions { "/sdl" }
    --    defines "MSC"
--
    --filter { "toolset:gcc* or toolset:clang*" }
    --    enablewarnings {
    --        "cast-align",
    --        "cast-qual",
    --        "ctor-dtor-privacy",
    --        "disabled-optimization",
    --        "format=2",
    --        "init-self",
    --        "missing-declarations",
    --        "missing-include-dirs",
    --        "old-style-cast",
    --        "overloaded-virtual",
    --        "redundant-decls",
    --        "shadow",
    --        "sign-conversion",
    --        "sign-promo",
    --        "strict-overflow=5",
    --        "switch-default",
    --        "undef",
    --        "uninitialized",
    --        "unreachable-code",
    --        "unused",
    --        "alloca",
    --        "conversion",
    --        "deprecated",
    --        "format-security",
    --        "null-dereference",
    --        "stack-protector",
    --        "vla",
    --        "shift-overflow"
    --    }
--
    --filter "toolset:gcc*"
    --    warnings "Extra"
    --    externalwarnings "Off"
    --    linkgroups "on" -- activate position independent linking
    --    enablewarnings {
    --        "noexcept",
    --        "strict-null-sentinel",
    --        "array-bounds=2",
    --        "duplicated-branches",
    --        "duplicated-cond",
    --        "logical-op",
    --        "arith-conversion",
    --        "stringop-overflow=4",
    --        "implicit-fallthrough=3",
    --        "trampolines"
    --    }
    --    defines "GCC"
--
    --filter "toolset:clang*"
    --    warnings "Extra"
    --    externalwarnings "Everything"
    --    enablewarnings {
    --        "array-bounds",
    --        "long-long",
    --        "implicit-fallthrough", 
    --    }
    --    defines "CLANG"
    --filter {}


    filter { "configurations:Debug" }
        kind "ConsoleApp"
    filter { "configurations:Release" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"