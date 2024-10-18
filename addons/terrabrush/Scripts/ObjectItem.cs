using Godot;

namespace TerraBrush;

[Tool]
public partial class ObjectItem : Node3D {
    private Camera3D _camera;
    private Vector3 _lastUpdatedPosition = Vector3.Zero;

    [Export] public float MaximumDistance { get;set; }
    [Export] public PackedScene ObjectPackedScene { get;set; }

    public override void _Ready() {
        base._Ready();

        if (MaximumDistance <= 0) {
            SetPhysicsProcess(false);
            return;
        }

        if (Engine.IsEditorHint()) {
            _camera = EditorInterface.Singleton.GetEditorViewport3D().GetCamera3D();
        } else {
            _camera = GetViewport()?.GetCamera3D();
        }
    }

    public override void _PhysicsProcess(double delta) {
        base._PhysicsProcess(delta);

        var currentPosition = _camera.GlobalPosition;
        if (currentPosition.DistanceTo(_lastUpdatedPosition) > MaximumDistance / 3.0) {
            if (GlobalPosition.DistanceTo(_camera.GlobalPosition) > MaximumDistance) {
                foreach (var instanceNode in GetChildren()) {
                    instanceNode.QueueFree();
                }
            } else {
                if (GetChildCount() == 0) {
                    AddChild(ObjectPackedScene.Instantiate());
                }
            }

            _lastUpdatedPosition = currentPosition;
        }
    }
}
