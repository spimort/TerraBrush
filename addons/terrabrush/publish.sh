#!/bin/bash

# Remove the build directory
rm -rf ./build

if [ -z "$1" ]; then
  echo "Error: No platform specified. Please provide 'windows', 'macos', or 'linux'."
  exit 1
fi

# Execute commands based on the platform argument
case "$1" in
  "macos")
    echo "Publishing for macOS..."
    dotnet publish -v:detailed --self-contained -c Release -r osx-arm64
    dotnet publish -v:detailed --self-contained -c Release -r osx-x64
    ;;

  "windows")
    echo "Publishing for Windows..."
    dotnet publish -v:detailed --self-contained -c Release -r win-x64 --property WarningLevel=0
    ;;

  "linux")
    echo "Publishing for Linux..."
    dotnet publish -v:detailed --self-contained -c Release -r linux-x64
    dotnet publish -v:detailed --self-contained -c Release -r linux-arm64
    ;;

  *)
    echo "Error: Invalid platform specified. Use 'windows', 'macos', or 'linux'."
    exit 1
    ;;
esac

echo "Build finished successfully."
exit 0


# ./Godot_v4.4-dev7_win64_console.exe -e "F:\Prog\Godot\Projects\TerraBrush\project.godot"