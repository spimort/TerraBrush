#!/usr/bin/env python
import os
import sys
import subprocess

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
    # For iOS, we need to combine TerraBrush static library with godot-cpp static library
    # because static libraries don't auto-link dependencies like shared libraries do

    # Determine the godot-cpp library path based on target and architecture
    godot_cpp_target = env["target"]  # template_debug or template_release
    godot_cpp_arch = env["arch"]
    if env["ios_simulator"]:
        godot_cpp_lib = "godot-cpp/bin/libgodot-cpp.ios.{}.{}.simulator.a".format(godot_cpp_target, godot_cpp_arch)
    else:
        godot_cpp_lib = "godot-cpp/bin/libgodot-cpp.ios.{}.{}.a".format(godot_cpp_target, godot_cpp_arch)

    # Build TerraBrush static library first (intermediate - stored in build/ to keep bin/ clean)
    if env["ios_simulator"]:
        terrabrush_intermediate = "build/libterrabrush_only.{}.{}.simulator.a".format(env["platform"], targetEnv)
        combined_output = "demo/addons/terrabrush/bin/libterrabrush.{}.{}.simulator.a".format(env["platform"], targetEnv)
    else:
        terrabrush_intermediate = "build/libterrabrush_only.{}.{}.a".format(env["platform"], targetEnv)
        combined_output = "demo/addons/terrabrush/bin/libterrabrush.{}.{}.a".format(env["platform"], targetEnv)

    terrabrush_lib = env.StaticLibrary(terrabrush_intermediate, source=sources)

    # Create a command to combine both static libraries using libtool
    def combine_static_libs(target, source, env):
        """Combine multiple static libraries into one using libtool"""
        target_path = str(target[0])
        source_paths = [str(s) for s in source]
        cmd = ["libtool", "-static", "-o", target_path] + source_paths
        print("Combining static libraries: {}".format(" ".join(cmd)))
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            print("libtool error: {}".format(result.stderr))
            return 1
        return 0

    # Create the combined library
    combined_lib = env.Command(
        combined_output,
        [terrabrush_lib, godot_cpp_lib],
        combine_static_libs
    )
    library = combined_lib
else:
    library = env.SharedLibrary(
        "demo/addons/terrabrush/bin/libterrabrush.{}.{}.{}{}".format(env["platform"], targetEnv, env["arch"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
