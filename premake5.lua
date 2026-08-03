workspace "HonseEngine"
   	configurations { "Debug", "Release" }
    architecture "x86_64"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter {}

--------------------------------------------------------------------------------
-- GLFW (STATIC, X11 ONLY) + GLAD
--------------------------------------------------------------------------------
project "GLFW"
    kind "StaticLib"
    language "C"
    pic "On"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    includedirs {
        "ext/glfw-3.4/include",
        "ext/glfw-3.4/src",
        "ext/glad/include"
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
        "ext/glad/glad.c"
    }

    filter "system:linux"
        defines {
            "_GLFW_X11"
        }

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


        files {
            "ext/glfw-3.4/src/x11_*.c",
            "ext/glfw-3.4/src/glx_context.c",
            "ext/glfw-3.4/src/xkb_unicode.c",
            "ext/glfw-3.4/src/egl_context.c",
            "ext/glfw-3.4/src/osmesa_context.c",
            "ext/glfw-3.4/src/linux_joystick.c",
            "ext/glfw-3.4/src/posix_*.c"
        }

		removefiles {
			"ext/glfw-3.4/src/wl_*.c",
			"ext/glfw-3.4/src/win32_*.c",
			"ext/glfw-3.4/src/cocoa_*.c",
    	}

    

    filter {}


--------------------------------------------------------------------------------
-- LECS
--------------------------------------------------------------------------------
project "LECS"
    kind "StaticLib"
    language "C++"
    pic "On"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    includedirs {
        "ext/lecs/include",
        "ext/lecs/src",
        "include",
    }

    files {
		"ext/lecs/src/**.cpp"
	}


    filter {}


--------------------------------------------------------------------------------
-- ENGINE + FMOD
--------------------------------------------------------------------------------
project "HonseEngine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    pic "On"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    files {
        "internal/**.cpp",
        "src/**.h",
        "src/**.cpp",
    }

    includedirs {
        "include",
        "internal",
        "ext/lecs/include",
        "ext/fmod/include",
        "ext/glad/include",
        "ext/fmod/include/fmod/core",
        "ext/fmod/include/fmod/studio",
        "ext/glfw-3.4/include"
    }

    links {
        "GLFW",
        "LECS"     
    }

    filter "system:linux"
        defines { "_LINUX" }

        libdirs {
            "ext/fmod/lib/core/x86_64",
            "ext/fmod/lib/studio/x86_64"
        }

        links {
            "fmodstudio",
            "fmod"
        }

        postbuildcommands {
            "{COPY} ext/fmod/lib/core/x86_64/*.so* %{cfg.targetdir}",
            "{COPY} ext/fmod/lib/studio/x86_64/*.so* %{cfg.targetdir}"
        }

        linkoptions {
            "-Wl,--no-undefined,-rpath,\\$$ORIGIN"
        }

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
        "ext/lecs/include"
    }

    links {
        "HonseEngine"
    }