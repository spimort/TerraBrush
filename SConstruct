#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")
sources += Glob("src/editor_nodes/*.cpp")
sources += Glob("src/editor_resources/*.cpp")
sources += Glob("src/editor_tools/*.cpp")
sources += Glob("src/misc/*.cpp")
sources += Glob("src/nodes/*.cpp")
sources += Glob("src/octree/*.cpp")

targetEnv = "debug"
if env["target"] == "template_release":
    targetEnv = "release"

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/addons/terrabrush/bin/libterrabrush.{}.{}.framework/terrabrush.{}.{}".format(
            env["platform"], targetEnv, env["platform"], targetEnv
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "demo/addons/terrabrush/bin/terrabrush.{}.{}.simulator.a".format(env["platform"], targetEnv),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "demo/addons/terrabrush/bin/terrabrush.{}.{}.a".format(env["platform"], targetEnv),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "demo/addons/terrabrush/bin/libterrabrush.{}.{}.{}{}".format(env["platform"], targetEnv, env["arch"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
