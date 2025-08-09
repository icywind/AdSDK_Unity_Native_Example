using System;
using System.Collections.Concurrent;
using UnityEngine;

public class Dispatcher : MonoBehaviour
{
    private static Dispatcher _instance;
    private static readonly ConcurrentQueue<Action> _queue = new ConcurrentQueue<Action>();

    /// <summary>
    /// Ensures a single instance of Dispatcher exists in the scene.
    /// </summary>
    public static void EnsureExists()
    {
        if (_instance == null)
        {
            var obj = new GameObject("MainThreadDispatcher");
            _instance = obj.AddComponent<Dispatcher>();
            DontDestroyOnLoad(obj);
        }
    }

    /// <summary>
    /// Queue an action to run on Unity's main thread.
    /// </summary>
    public static void RunOnMainThread(Action action)
    {
        if (action == null) return;
        _queue.Enqueue(action);
    }

    void Update()
    {
        while (_queue.TryDequeue(out var action))
        {
            try
            {
                action.Invoke();
            }
            catch (Exception ex)
            {
                Debug.LogError($"Dispatcher exception: {ex}");
            }
        }
    }
}