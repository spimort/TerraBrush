namespace TerraBrush;

public static class Utils {
    public static float GetNextFloatWithSeed(int seed, float minValue, float maxValue) {
        var randomGenerator = new Godot.RandomNumberGenerator();
        randomGenerator.Seed = (ulong) seed;

        return randomGenerator.RandfRange(minValue, maxValue);
    }

    public static int GetNextIntWithSeed(int seed, int minValue, int maxValue) {
        var randomGenerator = new Godot.RandomNumberGenerator();
        randomGenerator.Seed = (ulong) seed;

        return randomGenerator.RandiRange(minValue, maxValue);
    }

    public static string PathCombineForwardSlash(string directory, string path) {
        if (!directory.EndsWith("/")) {
            directory += "/";
        }

        if (path.StartsWith("/")) {
            path = path.Substring(0, 1);
        }

        directory += path;

        return directory;
    }
}
