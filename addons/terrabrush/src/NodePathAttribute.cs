// Courtesy of @eumario
// Original file : https://github.com/eumario/GodotSharpExtras/blob/master/lib/Classes/NodePathAttribute.cs

using System;

namespace TerraBrush;

/// <summary>
/// Sets the value of a field to the node at the given path.
/// </summary>
[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property, AllowMultiple = false, Inherited = true)]
public class NodePathAttribute : Attribute {
    public string NodePath { get;set; }

    /// <summary>
    /// Constructs a <see cref="NodePathAttribute"/>
    /// </summary>
    /// <param name="path">The path to the node, relative to the current node.</param>
    public NodePathAttribute(string path = "") {
        this.NodePath = path;
    }
}
