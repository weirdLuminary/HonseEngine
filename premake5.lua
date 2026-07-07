workspace "HonseEngine"
   	configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter {}

--------------------------------------------------------------------------------
-- GLFW (STATIC, X11 ONLY)
--------------------------------------------------------------------------------
project "GLFW"
    kind "StaticLib"
    language "C"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    defines {
        "_GLFW_X11"
    }

    includedirs {
        "ext/glfw-3.4/include",
        "ext/glfw-3.4/src"
    }

    files {
		"ext/glfw-3.4/src/context.c",
		"ext/glfw-3.4/src/init.c",
		"ext/glfw-3.4/src/input.c",
		"ext/glfw-3.4/src/monitor.c",
		"ext/glfw-3.4/src/platform.c",
		"ext/glfw-3.4/src/window.c",
		"ext/glfw-3.4/src/vulkan.c",
		"ext/glfw-3.4/src/null_*.c",

        -- X11 backend
        "ext/glfw-3.4/src/x11_*.c",
		"ext/glfw-3.4/src/glx_context.c",
		"ext/glfw-3.4/src/xkb_unicode.c",

		-- REQUIRED context backends
		"ext/glfw-3.4/src/egl_context.c",
		"ext/glfw-3.4/src/osmesa_context.c",

		-- POSIX helpers
		"ext/glfw-3.4/src/linux_joystick.c",
		"ext/glfw-3.4/src/posix_*.c"
	}

    filter "system:linux"
        links {
			"GL",
            "X11",
            "Xrandr",
            "Xi",
            "Xcursor",
            "Xinerama",
            "dl",
            "pthread",
            "m"
        }

		removefiles {
			"ext/glfw-3.4/src/wl_*.c",
			"ext/glfw-3.4/src/win32_*.c",
			"ext/glfw-3.4/src/cocoa_*.c",
    	}

    filter "system:windows"
        links {
            "GLFW",
            "opengl32",
            "gdi32",
            "user32",
            "shell32"
        }

    filter {}

--------------------------------------------------------------------------------
-- ENGINE
--------------------------------------------------------------------------------
project "HonseEngine"
	kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "internal/**.cpp",
        "src/**.h",
        "src/**.cpp",
		"ext/glad.c"
    }

    includedirs {
        "include",
        "internal"
    }

    links {
		"GLFW"
	}

    filter "system:windows" 
        defines { "_WIN" }

    filter "system:linux"
        defines { "_LINUX" } 

    
    filter {}

--------------------------------------------------------------------------------
-- EXAMPLE GAME
--------------------------------------------------------------------------------
project "Pong"
	kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "examples/pong/src/**.cpp"
    }

    includedirs {
        "include",
    }

    links {
		"HonseEngine",
        "GLFW"
	}