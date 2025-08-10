// Courtesy of @eumario
// Original file : https://github.com/eumario/GodotSharpExtras/blob/master/lib/Tools/Tools.cs

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Godot;

namespace TerraBrush;

public static class NodePathUtils {
    /// <summary>
    /// Processes all Attributes for NodePaths.
    /// </summary>
    /// <param name="node">The node.</param>
    public static void RegisterNodePaths<T>(this T node)
        where T : Node
    {
        var type = node.GetType();

        if (TypeMembers.TryGetValue(type, out var members) == false)
        {
            var bindingFlags = BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static;
            members = type.GetFields(bindingFlags).Select(fi => new MemberInfo(fi))
                        .Concat(type.GetProperties(bindingFlags).Select(pi => new MemberInfo(pi)))
                        .ToArray();
            TypeMembers[type] = members;
        }

        foreach (var member in members)
        {
            foreach (var attr in member.CustomAttributes)
            {
                switch(attr)
                {
                    case NodePathAttribute pathAttr:
                        AssignPathToMember(node, member, pathAttr.NodePath);
                        break;
                }
            }
        }
    }

    private static Node TryGetNode(Node node, List<string> names)
    {
        foreach(var name in names) {
            if (string.IsNullOrEmpty(name)) continue;
            var target = node.GetNodeOrNull(name);
            if (target != null)
                return target;
            if (node.Owner == null) continue;
            target = node.Owner.GetNodeOrNull(name);
            if (target != null)
                return target;
        }
        return null;
    }

    private static void AssignPathToMember(Node node, MemberInfo member, NodePath path)
    {
        var name1 = member.Name;
        if (!name1.StartsWith("_"))
            name1 = string.Empty;
        else
        {
            name1 = char.ToUpperInvariant(member.Name[1]) + member.Name[2..];
            // name1 = member.Name.Replace("_", string.Empty);
            // name1 = char.ToUpperInvariant(name1[0]) + name1.Substring(1);
        }

        List<string> names = new List<string>()
        {
            path.ToString(),
            member.Name,
            $"%{member.Name}",
            name1,
            string.IsNullOrEmpty(name1) ? "" : $"%{name1}",
            member.MemberType.Name
        };

        if (names.Contains(""))
            names.RemoveAll(string.IsNullOrEmpty);

        Node value = TryGetNode(node, names);

        if (value == null)
            throw new Exception($"AssignPathToMember on {node.GetType().FullName}.{member.Name} - Unable to find node with the following names: {string.Join(",", names.ToArray())}");
        try
        {
            member.SetValue(node,value);
        }
        catch (ArgumentException e)
        {
            throw new Exception($"AssignPathToMember on {node.GetType().FullName}.{member.Name} - cannot set value of type {value?.GetType().Name} on field type {member.MemberType.Name}", e);
        }
    }

    private readonly struct MemberInfo
    {
        public string Name { get; }
        public Type MemberType { get; }
        public IEnumerable<Attribute> CustomAttributes { get; }
        public Action<object, object> SetValue { get; }
        public Func<object, object> GetValue { get; }

        public MemberInfo(PropertyInfo pi)
        {
            this.Name = pi.Name;
            this.MemberType = pi.PropertyType;
            this.CustomAttributes = pi.GetCustomAttributes();
            this.SetValue = pi.SetValue;
            this.GetValue = pi.GetValue;
        }

        public MemberInfo(FieldInfo fi)
        {
            this.Name = fi.Name;
            this.MemberType = fi.FieldType;
            this.CustomAttributes = fi.GetCustomAttributes();
            this.SetValue = fi.SetValue;
            this.GetValue = fi.GetValue;
        }
    }

    private static readonly Dictionary<Type, MemberInfo[]> TypeMembers = new Dictionary<Type, MemberInfo[]>();
}
