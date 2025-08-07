#pragma once
#include <stdint.h> 

#ifdef _WIN32
#define ADSDK_EXPORT __declspec(dllexport)
#else
#define ADSDK_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*AdSuccessCallback)(const char* adUnitId);
typedef void(*AdFailureCallback)(const char* adUnitId, const char* error);
typedef void(*AdThumbnailCallback)(const char* adUnitId, const uint8_t* rgbaData, int width, int height);

ADSDK_EXPORT void AdSDK_Init(const char* appId);
ADSDK_EXPORT void AdSDK_PreloadAd(const char* adUnitId, AdSuccessCallback onSuccess, AdFailureCallback onFailure);
ADSDK_EXPORT void AdSDK_Update();
ADSDK_EXPORT void AdSDK_GetAdThumbnail(const char* adUnitId, AdThumbnailCallback cb);
ADSDK_EXPORT void AdSDK_FreeBuffer(const uint8_t* buffer);

#ifdef __cplusplus
}
#endif


