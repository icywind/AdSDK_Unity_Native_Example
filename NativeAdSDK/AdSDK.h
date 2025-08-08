#pragma once
#include <stdint.h> 
#include <mutex>
#include <string>
#include <cstring>
#include <thread>
#include <queue>
#include <unordered_map>

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
ADSDK_EXPORT void AdSDK_Dispose(const char* adUnitId);

#ifdef __cplusplus
}

struct CallbackTask {
    std::string adUnitId;
    std::string error;
    bool success;
    AdSuccessCallback successCb;
    AdFailureCallback failureCb;
};

class ADSDK_EXPORT AdSDK {
    std::mutex _cb_mutex;
    std::mutex _ad_mutex;
    std::queue<CallbackTask> g_callbacks;
    std::unordered_map<std::string, uint8_t*> _adThumbnails;
    static AdSDK* instance;

private:
    AdSDK();
    ~AdSDK() {        
      instance = nullptr;
    };

public:
    static AdSDK* getInstance();
    void Init(const char* appId);
    void PreloadAd(const std::string& adUnitId, AdSuccessCallback onSuccess, AdFailureCallback onFailure);
    void Update();
    void GetAdThumbnail(const std::string& adUnitId, AdThumbnailCallback cb);
    void Dispose(const char* adUnitId);
};

void AdSDK_Init(const char* appId) {
    AdSDK::getInstance()->Init(appId);
}

void AdSDK_PreloadAd(const char* adUnitId, AdSuccessCallback onSuccess, AdFailureCallback onFailure) {
    AdSDK::getInstance()->PreloadAd(adUnitId, onSuccess, onFailure);
}

void AdSDK_Update() {
    AdSDK::getInstance()->Update();
}

void AdSDK_GetAdThumbnail(const char* adUnitId, AdThumbnailCallback cb) {
    AdSDK::getInstance()->GetAdThumbnail(adUnitId, cb);
}

void AdSDK_Dispose(const char* adUnitId) {
    AdSDK::getInstance()->Dispose(adUnitId);
}

#endif
