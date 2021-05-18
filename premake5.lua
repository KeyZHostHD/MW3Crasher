dependencies = {
	basePath = "./deps"
}

function dependencies.load()
	dir = path.join(dependencies.basePath, "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function dependencies.imports()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.import()
		end
	end
end

function dependencies.projects()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.project()
		end
	end
end

newoption {
	trigger = "copy-to",
	description = "Optional, copy the EXE to a custom folder after build, define the path here if wanted.",
	value = "PATH"
}

dependencies.load()

workspace "mw3-crasher"
location "./build"
objdir "%{wks.location}/obj"
targetdir "%{wks.location}/bin/%{cfg.platform}/%{cfg.buildcfg}"

configurations {"Debug", "Release"}

architecture "x86"
platforms "x86"

buildoptions "/std:c++latest"
systemversion "latest"
symbols "On"
staticruntime "On"
editandcontinue "Off"
warnings "Extra"
characterset "ASCII"

flags {"NoIncrementalLink", "NoMinimalRebuild", "MultiProcessorCompile", "No64BitChecks"}

configuration "windows"
defines {"_WINDOWS", "WIN32"}

configuration "Release"
optimize "Size"
buildoptions {"/GL"}
linkoptions { "/IGNORE:4702", "/LTCG" }

defines {"NDEBUG"}

flags {"FatalCompileWarnings"}

configuration "Debug"
optimize "Debug"

defines {"DEBUG", "_DEBUG"}

configuration {}

project "mw3-crasher"
kind "SharedLib"
language "C++"

files {"./src/**.hpp", "./src/**.cpp"}

includedirs {"./src/utils", "%{prj.location}/src"}

dependencies.imports()

group "Dependencies"
dependencies.projects()