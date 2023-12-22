using Godot;

public partial class Demo : CharacterBody3D {
	public const float Speed = 15.0f;
	public const float JumpVelocity = 10f;
	public const float MouseSensitivity = 0.2f;

	public float gravity = ProjectSettings.GetSetting("physics/3d/default_gravity").AsSingle();

	[Export] private Camera3D Camera { get;set; }
	[Export] public TerraBrush.TerraBrush Terrain { get;set; }
	[Export] public Label DebugLabel { get;set; }

    public override void _Ready(){
        base._Ready();
    }

	public override void _Input(InputEvent e) {
		if (e.IsPressed() && !e.IsEcho() && Input.IsKeyPressed(Key.Escape)) {
			Input.MouseMode = Input.MouseMode == Input.MouseModeEnum.Captured ? Input.MouseModeEnum.Visible : Input.MouseModeEnum.Captured;
		}

        if (e is InputEventMouseMotion) {
            var mouseMotion = (InputEventMouseMotion) e;
            RotateY(Mathf.DegToRad(-mouseMotion.Relative.X * MouseSensitivity));

            Camera.RotateX(Mathf.DegToRad(-mouseMotion.Relative.Y * MouseSensitivity));

            var resultRotation = Camera.Rotation;
            resultRotation.X = Mathf.Clamp(resultRotation.X, Mathf.DegToRad(-85), Mathf.DegToRad(85));
            Camera.Rotation = resultRotation;
        }
	}

	public override void _PhysicsProcess(double delta) {
		Vector3 velocity = Velocity;

		if (!IsOnFloor()) {
			velocity.Y -= gravity * (float)delta;
		}

		if (Input.IsActionJustPressed("ui_accept") && IsOnFloor()) {
			velocity.Y = JumpVelocity;
		}

		Vector2 inputDir = Input.GetVector("left", "right", "forward", "backward");
		Vector3 direction = (Transform.Basis * new Vector3(inputDir.X, 0, inputDir.Y)).Normalized();
		if (direction != Vector3.Zero) {
			velocity.X = direction.X * Speed;
			velocity.Z = direction.Z * Speed;
		} else {
			velocity.X = Mathf.MoveToward(Velocity.X, 0, Speed);
			velocity.Z = Mathf.MoveToward(Velocity.Z, 0, Speed);
		}

		Velocity = velocity;
		MoveAndSlide();

		var playerX = GlobalPosition.X;
		var playerZ = GlobalPosition.Z;

		var debugText = $"[Press ESC to grab/release mouse] FPS: {Engine.GetFramesPerSecond()}";
		if (IsOnFloor() && GetLastSlideCollision() != null) {
			var collision = GetLastSlideCollision();

			if (collision?.GetCollider() == Terrain?.Terrain?.TerrainCollider) {
				var result = Terrain.GetPositionInformation(playerX, playerZ);
				debugText = $"{debugText} | Current collision : Water {result.WaterFactor}, Deep : {result.WaterDeepness} | Snow {result.SnowFactor}, Height : {result.SnowHeight} | Main Texture {(result.Textures?.Length > 0 ? result.Textures?[0].Factor : "")} - {(result.Textures?.Length > 0 ? result.Textures?[0].Name : "" )}";

			} else {
				debugText = $"{debugText} : Not colliding with terrain";
			}
		}

		if (IsOnFloor()) {
			Terrain.AddInteractionPoint(playerX, playerZ);
		}

		DebugLabel.Text = debugText;
	}
}
