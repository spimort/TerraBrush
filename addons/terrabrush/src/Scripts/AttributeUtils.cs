using System;
using System.Linq;

namespace TerraBrush;

public static class AttributeUtils {
    public static T GetAttribute<T>(object value) {
        var valueType = value.GetType();

        var memberInfos = valueType.GetMember(value.ToString());
        var valueMemberInfo = memberInfos?.FirstOrDefault(member => member.DeclaringType == valueType);
        var valueAttributes = valueMemberInfo?.GetCustomAttributes(typeof(T), false);

        if (valueAttributes?.Length > 0) {
            return ((T)valueAttributes[0]);
        }

        return default;
    }
}
