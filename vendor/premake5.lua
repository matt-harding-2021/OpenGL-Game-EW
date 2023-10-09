project "React3D"
	location "React3D"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
	systemversion "latest"
    
	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../build/" .. outputdir .. "/%{prj.name}")

	files {
		"React3D/src/**.h",
		"React3D/src/**.cpp"
	}
	
	includedirs {
		"React3D/src",
		"React3D/src/body",
		"React3D/src/collision",
		"React3D/src/constraint",
		"React3D/src/containers",
		"React3D/src/engine",
		"React3D/src/mathematics",
		"React3D/src/memory",
		"React3D/src/utils"
	} 
    
	filter "system:windows"
		defines { 
			"WIN32",
			"_WINDOWS"
		}
		
	filter { "system:windows", "configurations:Debug" }
		
		defines "DEBUG"
		runtime "Debug"
		symbols "On"
		
    filter { "system:windows", "configurations:Release" }
		defines "NDEBUG"
		runtime "Release"
		optimize "On"

