extends CharacterBody3D

const Speed: float = 15.0
const JumpVelocity: float = 10.0
const MouseSensitivity: float = 0.2

var _gravity: float = ProjectSettings.get_setting("physics/3d/default_gravity")

@export var _camera: Camera3D
@export var _terrain: TerraBrush
@export var _debugLabel: Label
@export var _shapeCast: ShapeCast3D

func _input(event: InputEvent) -> void:
	if event.is_pressed() and not event.is_echo() and Input.is_key_pressed(KEY_ESCAPE):
		if Input.mouse_mode == Input.MOUSE_MODE_CAPTURED:        
			Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
		else:
			Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
			
	if event is InputEventMouseMotion:
		rotate_y(deg_to_rad(-event.relative.x * MouseSensitivity))
		
		_camera.rotate_x(deg_to_rad(-event.relative.y * MouseSensitivity))
		
		var resultRotation = _camera.rotation
		resultRotation.x = clamp(resultRotation.x, deg_to_rad(-85), deg_to_rad(85))
		_camera.rotation = resultRotation
	
func _physics_process(delta: float) -> void:
	if not is_on_floor():
		velocity.y -= _gravity * delta
		
	if Input.is_action_just_pressed("ui_accept") && is_on_floor():
		velocity.y = JumpVelocity
	
	var inputDir: Vector2 = Input.get_vector("left", "right", "forward", "backward")
	var direction = (basis * Vector3(inputDir.x, 0, inputDir.y)).normalized()
	if direction != Vector3.ZERO:
		velocity.x = direction.x * Speed
		velocity.z = direction.z * Speed
	else :
		velocity.x = move_toward(velocity.x, 0, Speed)
		velocity.z = move_toward(velocity.z, 0, Speed)

	move_and_slide()
	
	var playerX = global_position.x
	var playerZ = global_position.z
	
	var debugText = "[Press ESC to grab/release mouse] FPS: " + String.num(Engine.get_frames_per_second(), 2)
	
	if _shapeCast.is_colliding():
		var collision = _shapeCast.get_collider(0)
		
		if collision == _terrain.getTerrainCollider():
			var result = _terrain.getPositionInformation(playerX, playerZ)
			if result != null:
				debugText = "%s
					Current collision :
					    Water : %s,
					    Deep : %s
					    Snow : %s - Height : %s
					    Main Texture : %s - %s
					    Meta Info : %s - %s
					" % [
						debugText,
						str(result.get_waterFactor()),
						str(result.get_waterDeepness()),
						str(result.get_snowFactor()),
						str(result.get_snowHeight()),
						str(result.get_textures()[0].get_factor()) if result.get_textures().size() > 0 else "",
						str(result.get_textures()[0].get_name()) if result.get_textures().size() > 0 else "",
						result.get_metaInfoIndex(),
						result.get_metaInfoName()
					]
			else:
				debugText = "%s : No zone" % [debugText]
		else:
			debugText = "%s : Not colliding with terrain" % [debugText]
			
	if is_on_floor():
		_terrain.addInteractionPoint(playerX, playerZ)
		
	_debugLabel.text = debugText

func _on_terra_brush_terrain_loaded() -> void:
	print("Terrain Loaded")
