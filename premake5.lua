workspace "TGL"
	architecture "x64"

	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "TGL/vendor/GLFW/include"
IncludeDir["Glad"] = "TGL/vendor/glad/include"
IncludeDir["glm"] = "TGL/vendor/glm"
IncludeDir["spdlog"] = "TGL/vendor/spdlog/include"
IncludeDir["stb_image"] = "TGL/vendor/stb_image"
IncludeDir["ImGui"] = "TGL/vendor/ImGui"
IncludeDir["entt"] = "TGL/vendor/entt/include"

group "Dependencies"
	include "TGL/vendor/GLFW"
	include "TGL/vendor/Glad"
	include "TGL/vendor/ImGui"

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
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

	includedirs {
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"GLFW_INCLUDE_NONE",
			"GL_PLATFORM_WINDOWS"
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
		
