workspace "TGL"
	architecture "x86_64"

	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TGL/vendor/glfw/include"
IncludeDir["Glad"] = "TGL/vendor/glad/include"
IncludeDir["glm"] = "TGL/vendor/glm"
IncludeDir["spdlog"] = "TGL/vendor/spdlog/include"
IncludeDir["stb_image"] = "TGL/vendor/stb_image"

group "Dependencies"
	include "TGL/vendor/glfw"
	include "TGL/vendor/Glad"

group ""

project "TGL"
	location "TGL"
	kind "ConsoleApp"
	language "C++"
	cppdialect "c++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "tglpch.h"
	pchsource "%{prj.name}/src/tglpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.glm}/glm/**.hpp",
		"%{IncludeDir.glm}/glm/**.inl",
		"%{IncludeDir.stb_image}/**.h",
		"%{IncludeDir.stb_image}/**.cpp"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
	}

	includedirs {
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links {
		"GLFW",
		"Glad",
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"GL_PLATFORM_WINDOWS"
		}
        links {
            "opengl32.lib"
        }

    filter "system:macosx"
        defines {
            "GL_PLATFORM_MACOSX"
        }

        links {
            "CoreFoundation.framework",
            "Cocoa.framework",
            "IOKit.framework",
            "CoreVideo.framework",
            "OpenGL.framework"
        }

	filter "configurations:Debug"
		defines {
			"ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		
