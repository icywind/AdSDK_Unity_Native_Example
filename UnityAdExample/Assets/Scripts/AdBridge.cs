using System;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.UI;
using AOT;

public class AdBridge : MonoBehaviour
{
    [DllImport("AdSDK")]
    private static extern void AdSDK_Init(string appId);

    [DllImport("AdSDK")]
    private static extern void AdSDK_PreloadAd(
        string adUnitId,
        AdSuccessCallback onSuccess,
        AdFailureCallback onFailure);

    [DllImport("AdSDK")]
    private static extern void AdSDK_Update();

    [DllImport("AdSDK")]
    private static extern void AdSDK_GetAdThumbnail(
        string adUnitId,
        AdThumbnailCallback callback);

    [DllImport("AdSDK")]
    private static extern void AdSDK_Dispose(string adUnitId);

    public delegate void AdSuccessCallback(string adUnitId);
    public delegate void AdFailureCallback(string adUnitId, string error);
    private delegate void AdThumbnailCallback(string adUnitId, IntPtr rgbaData, int width, int height);

    private static Texture2D adTexture;

    public string AppId = "12345";
    public string AdUnitId = "video_ad_001";

    void Start()
    {
        AdSDK_Init(AppId);
        AdSDK_PreloadAd(AdUnitId, OnAdSuccess, OnAdFailure);
    }

    void Update()
    {
        AdSDK_Update();
    }

    private static void OnAdSuccess(string adUnitId)
    {
        Debug.Log($"[Ad] Loaded: {adUnitId}");
    }

    private static void OnAdFailure(string adUnitId, string error)
    {
        Debug.LogError($"[Ad] Failed: {adUnitId}, Error: {error}");
    }

    [MonoPInvokeCallback(typeof(AdThumbnailCallback))]
    private static void OnThumbnailReceived(string adUnitId, IntPtr data, int width, int height)
    {
        if (adTexture == null || adTexture.width != width || adTexture.height != height)
        {
            adTexture = new Texture2D(width, height, TextureFormat.RGBA32, false);
        }

        byte[] managedBuffer = new byte[width * height * 4];
        Marshal.Copy(data, managedBuffer, 0, managedBuffer.Length);
        adTexture.LoadRawTextureData(managedBuffer);
        adTexture.Apply();

        AdSDK_Dispose(adUnitId);
        GameObject.Find("AdThumbnail").GetComponent<RawImage>().texture = adTexture;
    }
}