using Godot;

namespace TerraBrush;

[Tool]
public partial class ObjectsZone : Node3D {
    private Camera3D _camera;
    private Vector3 _lastUpdatedPosition = Vector3.Zero;

    [NodePath] private Node3D _objectsContainer;

    [Export] public float MaximumDistance { get;set; }
    [Export] public int ZonesSize { get;set; }

    public Node3D ObjectsContainer => _objectsContainer;

    public override void _Ready() {
        base._Ready();
        this.RegisterNodePaths();

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
            if (GlobalPosition.DistanceTo(_camera.GlobalPosition) + (ZonesSize / 2.0) > MaximumDistance) {
                _objectsContainer.SetPhysicsProcess(false);
            } else {
                _objectsContainer.SetPhysicsProcess(true);
            }

            _lastUpdatedPosition = currentPosition;
        }
    }

    public void AddObject(ObjectItem objectItem) {
        _objectsContainer.AddChild(objectItem);
    }
}
