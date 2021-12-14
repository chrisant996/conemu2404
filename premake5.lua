local to = ".build/"..(_ACTION or "nullaction")

--------------------------------------------------------------------------------
local function setup_cfg(cfg)
    configuration(cfg)
        targetdir(to.."/bin/"..cfg)
        objdir(to.."/obj/")

    configuration({cfg, "x32"})
        targetsuffix("_x86")

    configuration({cfg, "x64"})
        targetsuffix("_x64")
end

--------------------------------------------------------------------------------
local function define_project(name)
    project(name)
    flags("fatalwarnings")
    language("c++")
end

--------------------------------------------------------------------------------
local function define_exe(name)
    define_project(name)
    kind("consoleapp")
end

--------------------------------------------------------------------------------
workspace("conemu2404")
    configurations({"debug", "release"})
    platforms({"x32", "x64"})
    location(".")

    characterset("MBCS")
    flags("NoManifest")
    staticruntime("on")
    rtti("off")
    symbols("on")
    exceptionhandling("off")

    setup_cfg("release")
    setup_cfg("debug")

    configuration("debug")
        optimize("off")
        defines("DEBUG")
        defines("_DEBUG")

    configuration("release")
        optimize("full")
        defines("NDEBUG")

    configuration("vs*")
        defines("_HAS_EXCEPTIONS=0")
        defines("_CRT_SECURE_NO_WARNINGS")
        defines("_CRT_NONSTDC_NO_WARNINGS")

--------------------------------------------------------------------------------
define_exe("repro")
files("*.cpp")
