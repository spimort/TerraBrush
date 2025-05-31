<h1 align="center">
  TerraBrush
  <br>
</h1>

<h4 align="center">A C# Terrain heightmap editor for <a href="https://godotengine.org/" target="_blank">Godot engine 4.4</a></h4>

<p align="center">
  <a href="https://godotengine.org/">
    <img src="https://img.shields.io/badge/GODOT-%23000000.svg?style=for-the-badge&logo=godot-engine"
         alt="Godot">
  </a>
</p>

<div align="center">

  [Disclaimer](#disclaimer) •
  [Key Features](#key-features) •
  [How To Use](#how-to-use) •
  [FAQ](#faq) •
  [Support](#support) •
  [License](#license)

</div>

<div align="center">

  ![screenshot](https://github.com/spimort/TerraBrush/blob/main/preview.gif?raw=true)

</div>

## Disclaimer

This project has been made mainly for my own project. I'm pretty new to gamedev so if something is not done the way it should, feel free to contact me!
<br>
I'm happy to share it with the community but it's possible that some features that you would expect are not there.
<br>
I'm gonna be happy to accept PR for new features if it fits.

## Godot 4.3 and lower (4.x)

The demo scene has been updated to Godot 4.4 which makes it break with older version of Godot.

## Key Features

* **Sculpt** - Sculpt your terrain with a heightmap
  - Increase the terrain
  - Decrease the terrain
  - Smooth the terrain
  - Flatten the terrain
  - Set height (paint to a specific height)
  - Set angle (paint from a starting point with a specific angle)
* **Paint textures** - Add colors to your terrain with the textures painting tool
  - Normal map and roughness texture are supported
* **Foliage** - Add foliage to your terrain (ex. Grass)
  - The foliage follows the main camera with a given maximum distance
* **Packed scenes** - Scatter packed scenes to the terrain
  - Multiple objects can be scattered with one brush
  - Random Y rotation is supported
  - The packed scenes will always follow the terrain
  - Live adjustment of the position while the terrain is sculpting
* **Water** - Add water to your terrain
  - The water will make the terrain go lower for the painted area
  - The packed scenes and the foliage will also get lower with the water
  - Support for custom shader
* **Water flow** - Paint the direction of the water
* **Snow** - Add thick snow to your terrain
  - The texture of the snow is configurable so it's easy to make something like sand
  - Support for custom shader
* **Hole** - Paint hole. Useful to create cave or juste to make a terrain that is not square.
* **Multiple brushes**
  - Custom brushes are also available
* **Pie menu**
* **Shortcuts**
  - The plugin has been made so it's really easy to use. A lot of shortcuts are available.
    - LShift - Reverse the tool. Usually, this option will go from "Add" to "Remove". For the sculpting option, this does the "Smooth" option.
    - V - Show the PieMenu for the current tool
    - B - Show the PieMenu for the current brush
    - N - Show the options for the current tool (ex. While painting the textures, the PieMenu will show textures)
    - G - Show a quick select for the brush size
    - H - Show a quick select for the brush strength
    - K - Toggle "Auto add zones"
    - X - Lock on X axis
    - Z - Lock on Z axis
  - Shortcuts can be Re-Assigned.  To Access them, Goto '**Project->Tools->TerraBrush Key bindings**' to access the keymap, and ability to re-assign keys. - Implemented by @eumario
* **Settings** - Some settings are available in the "ProjectSettings->TerraBrush" (ex. The decal color)
* **LOD** - The terrain is created using a custom clipmap mesh so less vertices are needed. https://youtu.be/BardvKC0HF0
* **MultiZones** - The terrain support creates multiple zones (or chunks or regions) to improve performances on bigger terrain. https://youtu.be/X_klfk-kdSE
* **Heightmap based blending** - Textures can be blended with heightmaps/bumpmaps with custom intensity. - Implemented by @ZaPx64
* **Paint locked areas** - Areas on the terrain can be locked so you don't modify completed parts
* **Paint meta info** - Define meta info on parts of the terrain. This can be retrieved using the `GetPositionInformation` function.
* **Import and Export** - Import and export terrain from/to various images.

## How To Use

Watch the tutorial video!
https://youtu.be/n5WeAqeea1c

> **Note**
> Make sure you have the **.Net** version of Godot Engine.

**Clone** this repository - If you only want to try out the demo scene, you can run this project in Godot directly. Else, copy the folder **"addons/terrabrush"** to your **"addons"** folder.

```bash
# Clone this repository
$ git clone https://github.com/spimort/TerraBrush
```

**Add a C# Solution** - To make sure that your project has C# enabled, go under "Project->Tools->C#->Create C# Solution".

**Compile** - It's really important to compile using this icon ![image](https://github.com/spimort/TerraBrush/assets/6035485/cf47eb92-3660-4038-9ef7-dfedd255dcb1) so Godot can run the plugin.

**Enable the plugin** - When the game has been compiled once, you should be able to enable the plugin. To do that, go under "Project->ProjectSettings->Plugins" and enable the checkbox next to TerraBrush.

**Add the node** - To add a terrain node, in the "Add node" screen, search for "Node3D->TerraBrush" (![image](https://github.com/spimort/TerraBrush/assets/6035485/627ab85c-e84e-443d-89b1-0bd7ac4ad73e))

**Update the terrain**

Some properties of the node require the terrain to refresh (mainly for the shaders and stuff). If something does not refresh, hit that "UpdateTerrain" button!
<br>
For example, if you add a new texture, add foliage, add an object, add a water definition or add snow, you'll have to hit the "UpdateTerrain" button.

**Node properties**

|Property|Description|
|-------------|-------------|
|**Terrain Settings**||
|Zones Size|This is the size of the zones (in meters). By default, it will be 256m. There is no limit for this value but having a really high value will decrease the performance. Each zone will be created with this size.|
|Resolution|This determines the terrain resolution. By default, the resolution is **1**. For each pixel in the heightmap image, **1m** of terrain is calculated (1px = 1m). Increasing the resolution means each pixel will cover more terrain: a resolution of 2 covers 2m per pixel, and a resolution of 4 covers 4m per pixel. This helps reduce the heightmap file size and create a terrain with fewer polygons.<br><br>**The resolution applies only to components affecting the terrain shape (Heightmap, Water, Snow)**. All other components maintain a resolution of 1 for precision painting. The resolution **must be either 1 or a power of 2**. When using a resolution higher than 1, the **ZonesSize must be a power of 2 plus 1** (e.g., 257). It is also recommended to set the **LODInitialCellWidth** to match the resolution.|
|Data Path|In order to work, TerraBrush needs to have somewhere to store some files. Make sure the option for "Data Path" is filled. If possible, the tool will fill in information by itself.|
|Collision Only|This option is useful for running for example a Game Server. This will only create the collisions of the terrain (the packed scenes will also be created since they could have a collision shape)|
|Visual Instance Layers|The godot layer on which the terrain will be displayed.|
|Custom Shader|Allow you to use a custom shader for the terrain.|
|Create Terrain|Create the terrain with the current settings (everything that has been done will be cleared).|
|Update Terrain|Update the terrain with the current settings (it will keep everything that has been painted). This option is useful if you modify something that has a direct impact on the map (ex. Add a new texture, foliage, packed scenes, etc.).|
|Remove Terrain|Remove the current terrain (everything that has been done will be cleared).|
|Lock all terrain|Lock all parts of the current Terrain.|
|Unlock all terrain|Unlock all parts of the current Terrain.|
|Import Terrain|Import terrain components from various images.|
|Export Terrain|Export all components of the terrain to images.|
|**LOD**||
|LOD Levels|The number of levels the clipmap will be made of. The default value is 5.|
|LOD Rows Per Level|The number of rows per level. This is a base number, depending on the level, there might be more rows (ex. the first level is a little bit different so it has more cells). The default value is 101.|
|LOD Initial Cell Width|The initial size of the cell. Each level will double the size of the previous level cell' size. the default value is 1.0 (1 meter)|
|**Collisions**||
|Create Collision In Thread|Create the collision shape in a thread. This helps a lot when editing the terrain. This also means that the collision could appear late compared to the terrain. The default value is true.|
|Collision Layers|The terrain collision layers|
|Collision Mask| The terrain collision mask|
|**Textures**||
|Texture Sets|Let you define the textures of the terrain. **Be aware, that all the textures must be in the same format (ex. Mipmaps, compression mode, etc.)**. **Make sure to hit the update terrain button when you modify this and the terrain has already been created**. You should create a **TextureSetsResource** which holds several **TextureSetResource**. A set will accept an Albedo, Normal and Roughness texture. You can create a resource file with the TextureSetsResource so you can reuse your textures with other terrain.|
|TextureSetResource[x].Name|The name of the texture. This information is useful when you want to query the terrain to know the texture at a specific position.|
|TextureSetResource[x].AlbedoTexture|The albedo texure of the set.|
|TextureSetResource[x].NormalTexture|The normal map texture of the set.|
|TextureSetResource[x].RoughnessTexture|The roughness texture of the set.|
|TextureSetResource[x].TextureDetail|This will determine how often your texture will be repeated on the terrain. A higher value means more repetitions. The default value is -1 to take the global TextureDetail of the terrain.|
|TextureSetResource[x].Triplanar|This option prevents texture stretching using a triplanar algorithm. This option is available per texture for better control over performance.|
|Texture Detail|This will determine how often your textures will be repeated on the terrain. A higher value means more repetitions. The default value is 20.|
|Use Anti Tile|This will determine if the textures will use an Anti Tile algorithm.|
|Height Blend Factor|The intensity (contrast) of the texture blending when using heightmaps/bumpmaps. 0 will use classic linear blending. The default value is 10. Negative or exaggerated values may produce interesting artistic effects.
|Nearest Texture Filter|Use nearest texture filter instead of linear filter.|
|Albedo Alpha Channel Usage|Allow the use of the alpha channel of the albedo texture for either roughness or height. The default value is none.|
|Normal Alpha Channel Usage|Allow the use of the alpha channel of the normal texture for either roughness or height. The default value is none.|
|Use Sharp Transitions|Use sharp transition between textures instead of blending them together. This will use the most dominant texture.|
|**Foliage**||
|Foliages|An array of FoliageResource. **Make sure to hit the update terrain button when you modify this and the terrain has already been created**.|
|FoliageResource[x].Definition|The definition of the foliage. Create a **FoliageDefinitionResource** to use it. You can create a resource of this definition to reuse it in other terrain.|
|FoliageResource[x].Definition.Strategy|The strategy used to generate the foliage (MultiMesh or GPUParticles). The default value is MultiMesh.|
|FoliageResource[x].Definition.Mesh|The mesh that will be used for the foliage. The mesh should have as few vertices as possible for better performance.|
|FoliageResource[x].Definition.MeshScale|The scale of the mesh.|
|FoliageResource[x].Definition.WindStrength|Creates some movement for the mesh. The default value is 0.1.|
|FoliageResource[x].Definition.NoiseTexture|This texture makes sure that the foliage placement is not too straight. If not specified, the default noise texture will be used.|
|FoliageResource[x].Definition.VisualInstanceLayers|The godot layer on which the foliage will be displayed.|
|FoliageResource[x].Definition.LODLevels|**(MultiMesh only)** The number of levels the clipmap will be made of. The default value is 3.|
|FoliageResource[x].Definition.LODRowsPerLevel|**(MultiMesh only)** The number of rows per level. This is a base number, depending on the level, there might be more rows (ex. the first level is a little bit different so it has more cells). The default value is 50.|
|FoliageResource[x].Definition.LODInitialCellWidth|**(MultiMesh only)** The initial size of the cell. Each level will double the size of the previous level cell' size. the default value is 1.0 (1 meter)|
|FoliageResource[x].Definition.Albedo|**(MultiMesh only)** The base color of the mesh. The default value is white|
|FoliageResource[x].Definition.AlbedoTextures|**(MultiMesh only)** A set of textures that will be chosen randomly for each instance.|
|FoliageResource[x].Definition.NearestTextureFilter|**(MultiMesh only)** Use nearest texture filter instead of linear filter.|
|FoliageResource[x].Definition.UseGroundColor|**(MultiMesh only)** If true, the color of the ground, depending on the position of the mesh, will be sent to the COLOR uniform and then to the color of the mesh. The default value is false.|
|FoliageResource[x].Definition.CastShadow|**(MultiMesh only)** If true, the shadow will be enabled on the multimesh. The default value is false.|
|FoliageResource[x].Definition.UseBrushScale|**(MultiMesh only)** If true, the more you paint at a specific location, the bigger the foliage will be (on a range of 0.0 to 1.0). The default value is true.|
|FoliageResource[x].Definition.ScaleNoiseTexture|**(MultiMesh only)** A texture to help get various height on the foliage.|
|FoliageResource[x].Definition.RandomPlacementRange|**(MultiMesh only)** A value that helps place randomly each mesh instance. The position will be between -RandomPlacementRange to RandomPlacementRange from the center of the original position in the clipmap. This helps blend each layers of the clipmap. The default value is 3.0.|
|FoliageResource[x].Definition.ApplyOnTextureIndexes|**(MultiMesh only)** A list of texture indexes where foliage should be applied.|
|FoliageResource[x].Definition.CustomShader|**(MultiMesh only)** A custom shader for the foliage.|
|FoliageResource[x].Definition.MeshMaterial|**(GPUParticles only)** The material that will be used on the mesh.|
|FoliageResource[x].Definition.MaximumRenderDistance|**(GPUParticles only)** This is the maximum distance the foliage will appear from the camera. A high value will decrease the performance.|
|FoliageResource[x].Definition.EditorMaximumRenderDistance|**(GPUParticles only)** This is the maximum distance the foliage will appear from the camera, but only for the editor. A high value will decrease the performances. Useful if you want to see far.|
|**Objects**||
|Default Object Frequency|This option is to define how often the objects will be placed on the terrain. This is the default for every objects. For example, a value of 10 will place an object every 10 meters. The default value is 10.|
|Object Loading Strategy|Drives how the objects are being loaded (in a thread or not). The default value is ThreadedInEditorOnly.|
|Objects|An array of ObjectResource. **Make sure to hit the update terrain button when you modify this and the terrain has already been created**.|
|ObjectResource[x].Definition|The definition of the object. Create a **ObjectDefinitionResource** to use it. You can create a resource of this definition to reuse it in other terrain.|
|ObjectResource[x].Definition.Strategy|This option lets you choose between instancing packed scenes or using multimeshes for the objects.|
|ObjectResource[x].Definition.ObjectFrequency|This option overrides the **Default Object Frequency** property if a value higher than -1 is set. This option is to define how often the objects will be placed on the terrain. For example, a value of 10 will place an object every 10 meters. The default value is 10.|
|ObjectResource[x].Definition.RandomRange|The range from which the random placement will be added from the original grid position.|
|ObjectResource[x].Definition.NoiseTexture|This texture makes sure that the object placement is not too straight. If not specified, the default noise texture will be used.|
|ObjectResource[x].Definition.RandomYRotation|This allows the objects to be rotated randomly on the Y axis.|
|ObjectResource[x].Definition.RandomSize|This allows the objects to be sized randomly.|
|ObjectResource[x].Definition.RandomSizeFactorMin|The minimum factor that will be multiplied with the scale of the object.|
|ObjectResource[x].Definition.RandomSizeFactorMax|The maximum factor that will be multiplied with the scale of the object.|
|ObjectResource[x].Definition.ObjectScenes|A list of packed scenes. A random one will be selected while painting.|
|ObjectResource[x].Definition.LODList|**(MultiMeshes only)** The list of LOD for the objects.|
|ObjectResource[x].Definition.LODMeshes|**(MultiMeshes only)** The list of meshes used with the LODList.|
|ObjectResource[x].Definition.UpdateDistanceThreshold|**(MultiMeshes only)** This sets the movement threshold before we need to check if the objects are changing LOD.|
|ObjectResource[x].Definition.UpdateTimeFrequency|**(MultiMeshes only)** This sets how often we check if the movement threshold has been reached.|
|ObjectResource[x].Definition.VisualInstanceLayers|**(MultiMeshes only)** The visual layers used with the MultiMeshes.|
|ObjectResource[x].Hide|Hide the whole layer of objects. This is useful when you want to see something on the terrain and the objects block the view.|
|**Water**||
|Water Definition|The definition of water. Create a **WaterResource** to use it. You can create a resource of this definition to reuse it in other terrain. **Make sure to hit the update terrain button when you modify this and the terrain has already been created**.|
|WaterResource.WaterFactor|This option lets you decide how deep the deepest water will be. A value of 1 will set the maximum deepness to 1m. The default value is 1.|
|WaterResource.WaterInnerOffset|This is an offset of the water going into the ground. The goal is to prevent having square-looking water edges.|
|WaterResource.WaterColor|The color of the water.|
|WaterResource.WaterFresnelColor|The color of the fresnel reflection color.|
|WaterResource.WaterMetallic|The metallic value of the material.|
|WaterResource.WaterRoughness|The roughness value of the material.|
|WaterResource.NormalMap|This is a normal map to create some waves in the water.|
|WaterResource.NormalMap2|This is another normal map to create some waves in the water. Having two normal maps helps to create better movement. Make sure it is different from the first one.|
|WaterResource.WaterTimeScale|This is how fast the water will move.|
|WaterResource.WaterStrength|This is how fast the normal map will move.|
|WaterResource.WaterWave|The Water Wave image is a noise texture that lets you drive how the waves will look like.|
|WaterResource.WaterNoiseScale|The scale of the noise texture.|
|WaterResource.WaterHeightScale|The height of the water waves created by the noise texture.|
|WaterResource.WaterColorDeep|The color of the deep water.|
|WaterResource.WaterColorShallow|The color of the Shallow water.|
|WaterResource.WaterBeersLaw|This option sets the beer law for the water.|
|WaterResource.WaterDeepOffset|This option sets how to calculate that the water is considered "Deep".|
|WaterResource.WaterEdgeScale|This option sets the size of the "foam" on the edges of the water.|
|WaterResource.WaterNear|This option sets how to calculate whether the water is close or not for the "foam".|
|WaterResource.WaterFar|This option sets how to calculate whether the water is close or not for the "foam".|
|WaterResource.WaterEdgeColor|This is the color of the "foam".|
|WaterResource.VisualInstanceLayers|The godot layer on which the water will be displayed.|
|WaterResource.CustomShader|A custom shader for ther water.|
|**Snow**||
|Snow Definition|The definition of the snow. Create a **SnowResource** to use it. You can create a resource of this definition to reuse it in other terrain. **Make sure to hit the update terrain button when you modify this and the terrain has already been created**.|
|SnowResource.SnowFactor|Sets how thick the snow will be.|
|SnowResource.SnowInnerOffset|This option is to decide the offset that the snow will get into the ground. This is to avoid having square edges of the snow.|
|SnowResource.SnowColorTexture|This sets the albedo texture that will be used for the snow.|
|SnowResource.SnowColorNormal|This sets the normal map texture that will be used for the snow.|
|SnowResource.SnowColorRoughness|This sets the roughness texture that will be used for the snow.|
|SnowResource.SnowColorDetail|This is to define how often the texture will be repeated.|
|SnowResource.Noise|The noise option allows the terrain to change shape a little bit to avoid having too straight a texture.|
|SnowResource.NoiseFactor|This option defines how much the noise will affect the snow.|
|SnowResource.Metallic|This is the metallic value of the material.|
|SnowResource.VisualInstanceLayers|The godot layer on which the snow will be displayed.|
|SnowResource.CustomShader|A custom shader for the snow.|
|**Meta**||
|ShowMetaInfo|Show the meta layers while in the editor. This has no effect in runtime (outside of the editor).|
|MetaInfoLayer[x].Name|The name of a meta layer. Useful to get the information about a location using the `GetPositionInformation` function.|
|MetaInfoLayer[x].Color|The color of a meta layer. This is the color that is gonna be used while in the editor to show the meta layer.|
|**Zones**||
|Zones[x].ZonePosition|The position of the zone. This position is multiplied by the zone's size to position it in the world. The zones can be disconnected from each other. **Make sure to hit the update terrain button when you modify this and the terrain has already been created**|
|Zones[x].HeightMapTexture|Unless you want to reuse an existing heightmap, leave this option to null. The tool will create it by itself.|
|Zones[x].SplatmapsTexture|Unless you have existing splatmaps, leave this option empty, the tool will create them by itself.|
|Zones[x].FoliagesTexture[x]|Unless you have existing foliage painting, leave this option empty, the tool will create it by itself.|
|Zones[x].ObjectsTexture[x]|Unless you have existing object painting, leave this option empty, the tool will create it by itself.|
|Zones[x].WaterTexture|Unless you have existing water painting, leave this option empty, the tool will create it by itself.|
|Zones[x].SnowTexture|Unless you have an existing snow painting, leave this option empty, the tool will create it by itself.|

**Node Signals**

|Signal Name|Description|
|---|---|
|**TerrainLoaded**|Occurs when the terrain is done loading all the different components.|

### Interact with the terrain

To interact with the terrain (for example, to make water ripple), you can call the function `AddInteractionPoint` on the TerraBrush node. This function requires an x and y position, corresponding to the x and y coordinates on the map, in meters.
<br>
To retrieve information from the map (for example, to get what texture the player is on), you can call the function `GetPositionInformation` on the TerraBrush node.

These two functions can be called from **GDScript** and **C#**.

#### C# Snippet
```c#
// Let's assume we have a variable called "TerraBrush" that is a TerraBrush Node.
// Let's validate that we are on the ground
if (IsOnFloor() && GetLastSlideCollision() != null) {
    // Get the current collision
    var collision = GetLastSlideCollision();

    // Let's make sure the collider is the collider of the Terrain
    if (collision?.GetCollider() == TerraBrush?.Terrain?.TerrainCollider) {
        // The variable playerX and playerZ are the variables for the global position of the player
        var result = TerraBrush.GetPositionInformation(playerX, playerZ);
        // If we don't get a result, it means we are out of the terrain
        if (result != null) {
            // The texture at position 0 is the most present one
            var mainTexture = result.Textures?.Length > 0 ? result.Textures?[0].Name : "";

            // Do something with the mainTexture information
            ...
        }
    }
}
```

#### GDScript Snippet
```gdscript
# Let's assume we have a variable called "%TerraBrush" that is a TerraBrush Node.
# Let's validate that we are on the ground
if is_on_floor() and get_last_slide_collision() != null:
    # Get the current collision
    var collision = get_last_slide_collision()

    # Let's make sure the collider is the collider of the Terrain
    if collision.get_collider() == %TerraBrush.Terrain.TerrainCollider:
        # The variable playerX and playerZ are the variables for the global position of the player
        var result = %TerraBrush.GetPositionInformation(playerX, playerZ)
        # If we don't get a result, it means we are out of the terrain
        if result != null:
            # The texture at position 0 is the most present one
            var mainTexture = result.Textures[0].Name if result.Textures.size() > 0 else ""
            # Do something with the mainTexture information
            ...
```

### Navigation mesh

In order to use a navigation mesh, make sure to set the **Parsed Geometry Type** to **Static Colliders** in your navigation mesh.

## FAQ

### Unable to load addon script
* Make sure to use the .Net version of Godot. See the following issue for more details : https://github.com/spimort/TerraBrush/issues/201

## Support

You want to support my work?

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/N4N1SC96Z)

## Demo credits
Trees: https://www.turbosquid.com/3d-models/3d-shapespark-low-poly-plants-kit-1826978
<br>
Grass: https://opengameart.org/content/stylized-grass-and-bush-textures-0
<br>
Textures: https://ambientcg.com/

## License

MIT

---
> Youtube [@spimortdev](https://www.youtube.com/@spimortdev) &nbsp;&middot;&nbsp;
> Twitter [@spimortdev](https://twitter.com/spimortdev)

