using System.Collections.Generic;
using Godot;

namespace TerraBrush;

public interface IObjectsNode {
    public int ObjectsIndex { get;set; }
    public ObjectDefinitionResource Definition { get;set; }
    public ZonesResource TerrainZones { get;set; }
    public int ZonesSize { get;set; }
    public int Resolution { get;set; }
    public float WaterFactor { get;set; }
    public bool LoadInThread { get;set; }
    public int DefaultObjectFrequency { get;set;}

    public void AddRemoveObjectFromTool(bool add, int x, int y, ZoneResource zone, Image heightmapImage, Image waterImage, Image noiseImage);
    public void UpdateMeshesFromTool();
    public void UpdateObjectsHeight(List<ZoneResource> zones);
}
