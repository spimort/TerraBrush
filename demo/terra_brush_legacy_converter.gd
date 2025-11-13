@tool
extends Node

@export var _terraBrushScene: PackedScene
@export_dir var _terraBrushDataFolder: String

@export_tool_button("Convert", "Callable") var convert_action = convert

func convert():
	var scene_file = FileAccess.open(_terraBrushScene.get_path(), FileAccess.READ)	
	var result_scene = process_scene(scene_file.get_as_text())
	scene_file = FileAccess.open(_terraBrushScene.get_path(), FileAccess.WRITE)	
	scene_file.seek(0)
	scene_file.store_string(result_scene)
	scene_file.close()
	
	var terrain_files = DirAccess.get_files_at(_terraBrushDataFolder)
	process_image_files(terrain_files)
	
	print("Done!")

func process_scene(input: String) -> String:
	var lines = input.split("\n")

	var script_types_matching = {
		"TerraBrush.cs": "TerraBrush",
		"TextureSetResource.cs": "TextureSetResource",
		"TextureSetsResource.cs": "TextureSetsResource",
		"FoliageDefinitionResource.cs": "FoliageDefinitionResource",
		"FoliageResource.cs": "FoliageResource",
		"SnowResource.cs": "SnowResource",
		"WaterResource.cs": "WaterResource",
		"ObjectDefinitionResource.cs": "ObjectDefinitionResource",
		"ObjectResource.cs": "ObjectResource",
		"ObjectOctreeLODDefinitionResource.cs": "ObjectOctreeLODDefinitionResource",
		"ObjectOctreeLODMeshDefinitionResource.cs": "ObjectOctreeLODMeshDefinitionResource",
		"ObjectOctreeLODMeshesDefinitionResource.cs": "ObjectOctreeLODMeshesDefinitionResource",
		"MetaInfoLayer.cs": "MetaInfoLayerResource",
		"ZonesResource.cs": "ZonesResource",
		"ZoneResource.cs": "ZoneResource",
	}

	var found_scripts = {}
	var ext_resource_regex = RegEx.new()
	ext_resource_regex.compile(r"\[ext_resource\s+type=\"Script\".*path=\"([^\"]+\.cs)\".*id=\"([^\"]+)\"(?:.*uid=\"([^\"]+)\")?")	

	var type_regex = RegEx.new()
	type_regex.compile('type="[^"]*"')

	# Process the lines
	var new_lines = []
	for i in range(lines.size()):
		var line = lines[i]
		
		var match = ext_resource_regex.search(line)
		if match:			
			var full_file_name = match.get_string(1)
			var splitValues = full_file_name.split("/")
			var file_name = splitValues[splitValues.size() - 1]
			var resource_type = script_types_matching.get(file_name, "")
			if resource_type != "":
				var id = match.get_string(2)
				found_scripts[id] = {"type": resource_type}				
			else:
				new_lines.push_back(line)
		else:
			new_lines.push_back(line)
			
	lines = new_lines

	# Iterate through found scripts and update lines
	for id in found_scripts.keys():
		var type_info = found_scripts[id]
		var line_indexes = []

		# Find lines containing ExtResource(id)
		for i in range(lines.size()):
			if lines[i].find("ExtResource(\"" + id + "\")") != -1:
				line_indexes.append(i)

		for line_index in line_indexes:
			if line_index > 0:
				var node_line = lines[line_index - 1]
				lines[line_index - 1] = type_regex.sub(node_line, "type=\"" + type_info["type"] + "\"")

				for i in range(line_index + 1, lines.size()):
					var property_line = lines[i]
					if property_line == "":
						break

					if property_line.begins_with("LOD"):
						lines[i] = property_line.substr(0, 3).to_lower() + property_line.substr(3, property_line.length())
					else:
						lines[i] = property_line.substr(0, 1).to_lower() + property_line.substr(1, property_line.length())

					if type_info["type"] == "ZoneResource":
						lines[i] = lines[i].replace("Texture", "Image")

		new_lines = []
		# Remove the ExtResource references from the lines
		for line in lines:
			if not line.contains("ExtResource(\"" + id + "\")"):
				new_lines.push_back(line)
				
		lines = new_lines

	return "\n".join(lines)

func process_image_files(files: PackedStringArray) -> void:
	for file in files:
		var full_file = _terraBrushDataFolder + "/" + file
		var resource = ResourceLoader.load(full_file)
		if is_instance_of(resource, ImageTexture):
			var image_texture_resource: ImageTexture = resource
			var image = image_texture_resource.get_image()
			
			ResourceSaver.save(image, full_file)
