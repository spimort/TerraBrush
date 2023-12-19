<h1 align="center">
  TerraBrush
  <br>
</h1>

<h4 align="center">A minimal Terrain heightmap editor for <a href="[http://electron.atom.io](https://godotengine.org/)" target="_blank">Godot engine</a>.</h4>

<p align="center">  
  <a href="https://godotengine.org/">
    <img src="https://img.shields.io/badge/GODOT-%23000000.svg?style=for-the-badge&logo=godot-engine"
         alt="Godot">
  </a>
</p>

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#credits">Credits</a> •
  <a href="#related">Related</a> •
  <a href="#license">License</a>
</p>

<div align="center">
  
  ![screenshot](https://github.com/spimort/TerraBrush/blob/main/preview.gif?raw=true)
  
</div>

## Key Features

* **Sculpt** - Sculpt your terrain with a heightmap
  - Increase the terrain
  - Decrease the terrain
  - Smooth the terrain
* **Paint textures** - Add colors to your terrain with the textures painting tool
  - Normal map and roughness texture are supported
* **Foliage** - Add foliage to your map (ex. Grass)
  - The foliage follow the main camera with a given maximum distance
* **Packed scenes** - Scatter packed scenes to the terrain
  - Multiple objects can be scattered with one brush
  - Random Y rotation is supported
  - The packed scenes will always follow the terrain
  - Live adjustment of the position while the terrain is sculpting
* **Water** - Add water to your terrain
  - The water will make the terrain go lower for the painted area
  - The packed scenes and the foliage will also get lower with the water
* **Water flow** - Paint the direction of the water
* **Snow** - Add thick snow to your terrain
  - The texture of the snow is configurable so it's easy to make something like sand
* **Multiple brushes**
  - Custom brushes are also available
* **Pie menu**
* **Shortcuts**
  - The plugin has been made so it's really easy to use. A lot of shortcuts are available.
    - LShift - Reverse the tool. Usually this option will go from "Add" to "Remove". For the sculpting option, this does the "Smooth" option.
    - V - Show the PieMenu for the current tool
    - B - Show the PieMenu for the current brush
    - N - Show the PieMenu for the current tool option (ex. While painting the textures, the PieMenu will show textures)
    - G - Show a quick select for the brush size
    - H - Show a quick select for the brush strength
* **Settings** - Some settings are available in the "ProjectSettings->TerraBrush" (ex. The decal color)

## How To Use

> **Note**
> Make sure you have the **.Net** version of Godot Engine.

**Clone** this repository - If you only want to try out the demo scene, you can run this project in Godot directly. Else, copy the folder **"addons/terrabrush"** to your **"addons"** folder.

```bash
# Clone this repository
$ git clone https://github.com/spimort/TerraBrush
```

**Compile** - It's really important to compile using this icon ![image](https://github.com/spimort/TerraBrush/assets/6035485/cf47eb92-3660-4038-9ef7-dfedd255dcb1) so Godot can run the plugin.
It might be required to have a C# script somewhere in your project in order to have this icon available.

**Enable the plugin** - When the game has been compiled once, you should be able to enable the plugin. To do that, go under "Project->ProjectSettings->Plugins" and enable the checkbox next to TerraBrush.

**Add the node** - To add a terrain node, in the "Add node" screen, search for "Node3D->TerraBrush" (![image](https://github.com/spimort/TerraBrush/assets/6035485/627ab85c-e84e-443d-89b1-0bd7ac4ad73e))

**TerraBrush properties**
* **Terrain Size** - This is the size of the terrain (in meters). By default, it will be of 256. There is no limit for this value but having a really high value will decrease the performance.
* **HeightMap** - Unless you want to reuse an existing heightmap, leave this option to null. The tool will create it by itself.
* **Data Path** - In order to work, TerraBrush needs to have somewhere to store some files. Make sure the option for "Data Path" is filled. If possible, the tool will fill the information by itself.
* **Visual Instance Layers** - The godot layer on which the map will be displayed.
* **Create Terrain** - Create the terrain with the current settings (everything that has been done will be cleared).
* **Update Terrain** - Update the terrain with the current settings (it will keep everything that has been painted). This option is useful if you modify something that has a direct impact on the map (ex. Add a new texture, foliage, packed scenes, etc.).
* **Remove Terrain** - Remove the current terrain (everything that has been done will be cleared).

## Support

You like what you see?

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/N4N1SC96Z)

## License

MIT

---
> Youtube [@spimortdev](https://www.youtube.com/@spimortdev) &nbsp;&middot;&nbsp;
> Twitter [@spimortdev](https://twitter.com/spimortdev)

