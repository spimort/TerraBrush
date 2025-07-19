using System;
using System.Threading;
using System.Threading.Tasks;
using Godot;

namespace TerraBrush;

public static class AsyncUtils {
    public static async Task<T> LoadResourceAsync<T>(string path, CancellationToken token) where T: Resource {
        return await Task.Factory.StartNew(() => {
            var error = ResourceLoader.Singleton.LoadThreadedRequest(path);
            if (error != Error.Ok) {
                throw new Exception(error.ToString());
            }

            do {
                var status = ResourceLoader.Singleton.LoadThreadedGetStatus(path);
                if (status == ResourceLoader.ThreadLoadStatus.Failed) {
                    throw new Exception($"Resource '{path}' load failed");
                } else if (status == ResourceLoader.ThreadLoadStatus.InvalidResource) {
                    throw new Exception($"Invalid resource '{path}'");
                } else if (status == ResourceLoader.ThreadLoadStatus.Loaded) {
                    return (T) ResourceLoader.Singleton.LoadThreadedGet(path);
                }
            } while (!token.IsCancellationRequested);

            return null;
        });
    }
}
