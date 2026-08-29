workspace "HonseEngine"
   	configurations { "Debug", "Release" }
    architecture "x86_64"
    debugdir "%{wks.location}/bin/%{cfg.buildcfg}"

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
-- BOX2D
--------------------------------------------------------------------------------
project "Box2D"
    kind "StaticLib"
    language "C"
    pic "On"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    includedirs {
        "ext/box2d/include",
        "ext/box2d/src"
    }

    files {
        "ext/box2d/src/**.c",
        "ext/box2d/src/**.h"
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
        "ext/lecs/src",
        "include",
    }

    files {
		"ext/lecs/src/**.cpp"
	}


    filter {}

--------------------------------------------------------------------------------
-- FREETYPE
--------------------------------------------------------------------------------
project "FreeType"
    kind "StaticLib"
    language "C"
    pic "On"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    defines {
        "FT2_BUILD_LIBRARY",
    }

    includedirs {
        "ext/freetype/include",
        "ext/freetype/src",
    }

    files {
        "ext/freetype/include/ft2build.h",
        "ext/freetype/include/freetype/**.h",

        "ext/freetype/src/autofit/autofit.c",
        "ext/freetype/src/base/ftbase.c",
        "ext/freetype/src/base/ftbbox.c",
        "ext/freetype/src/base/ftbdf.c",
        "ext/freetype/src/base/ftbitmap.c",
        "ext/freetype/src/base/ftcid.c",
        "ext/freetype/src/base/ftfstype.c",
        "ext/freetype/src/base/ftgasp.c",
        "ext/freetype/src/base/ftglyph.c",
        "ext/freetype/src/base/ftgxval.c",
        "ext/freetype/src/base/ftinit.c",
        "ext/freetype/src/base/ftmm.c",
        "ext/freetype/src/base/ftotval.c",
        "ext/freetype/src/base/ftpatent.c",
        "ext/freetype/src/base/ftpfr.c",
        "ext/freetype/src/base/ftstroke.c",
        "ext/freetype/src/base/ftsynth.c",
        "ext/freetype/src/base/fttype1.c",

        "ext/freetype/src/bdf/bdf.c",
        "ext/freetype/src/cache/ftcache.c",
        "ext/freetype/src/cff/cff.c",
        "ext/freetype/src/cid/type1cid.c",
        "ext/freetype/src/lzw/ftlzw.c",
        "ext/freetype/src/psaux/psaux.c",
        "ext/freetype/src/pshinter/pshinter.c",
        "ext/freetype/src/psnames/psnames.c",
        "ext/freetype/src/raster/raster.c",
        "ext/freetype/src/sfnt/sfnt.c",
        "ext/freetype/src/smooth/smooth.c",
        "ext/freetype/src/truetype/truetype.c",
        "ext/freetype/src/type1/type1.c",
        "ext/freetype/src/type42/type42.c",
        "ext/freetype/src/winfonts/winfnt.c",

        -- Linux:
        "ext/freetype/builds/unix/ftsystem.c",
        "ext/freetype/src/base/ftdebug.c",
    }


    filter {}

--------------------------------------------------------------------------------
-- ENGINE; FMOD & LUMPACK
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
        "ext/freetype/include",
        "ext/glfw-3.4/include",
        "ext/box2d/include"
    }

    links {
        "GLFW",
        "LECS",
        "Box2D",
        "FreeType"  
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
            "cp -Lf ext/fmod/lib/core/x86_64/libfmod.so.14.14 %{cfg.targetdir}/libfmod.so.14",
            "cp -Lf ext/fmod/lib/studio/x86_64/libfmodstudio.so.14.14 %{cfg.targetdir}/libfmodstudio.so.14",
            "rm -f bin/Debug/*.a"
        }

        linkoptions {
            "-Wl,--no-undefined,-rpath,\\$$ORIGIN"
        }

    filter {}

--------------------------------------------------------------------------------
-- EXAMPLE GAME
--------------------------------------------------------------------------------
-- project "Pong"
-- 	kind "ConsoleApp"
--     language "C++"
--     cppdialect "C++17"
--     targetdir "bin/%{cfg.buildcfg}"
--     objdir "bin-int/%{cfg.buildcfg}"

--     defines { "FMOD_STUDIO" }

--     files {
--         "examples/pong/src/**.cpp"
--     }

--     includedirs {
--         "include",
--         "ext/lecs/include",
--         "ext/lumpack/include"
--     }

--     links {
--         "HonseEngine"
--     }