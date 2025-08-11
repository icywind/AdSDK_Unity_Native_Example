#include "AdSDK.h"

AdSDK* AdSDK::instance = nullptr;

AdSDK::AdSDK() {}

AdSDK* AdSDK::getInstance() {
    if (!instance) {
        instance = new AdSDK();
    }
    return instance;
}

void AdSDK::Init(const char* appId) {
}

void AdSDK:: PreloadAd(const std::string& adUnitId, AdSuccessCallback onSuccess, AdFailureCallback onFailure) {
        std::thread([=]() {
            // pretend to load ad for some time
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            CallbackTask task;
            task.adUnitId = adUnitId;
            task.successCb = onSuccess;
            task.failureCb = onFailure;
            task.success = true;
            task.error = "";
            std::lock_guard<std::mutex> lock(_cb_mutex);
            g_callbacks.push(task);
        }).detach();
    }

void AdSDK:: Update() {
        std::lock_guard<std::mutex> lock(_cb_mutex);
        while (!g_callbacks.empty()) {
            CallbackTask task = g_callbacks.front();
            g_callbacks.pop();
            if (task.success) {
                if (task.successCb) task.successCb(task.adUnitId.c_str());
            } else {
                if (task.failureCb) task.failureCb(task.adUnitId.c_str(), task.error.c_str());
            }
        }
    }
    void AdSDK::GetAdThumbnail(const std::string& adUnitId, AdThumbnailCallback cb) {
        std::thread([=]() {
            int width = 256, height = 256;
            uint8_t* buffer = new uint8_t[width * height * 4];
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x) {
                    int i = (y * width + x) * 4;
                    buffer[i] = (x + y) % 2 == 0 ? 255 : 0;
                    buffer[i+1] = (x + y) % 2 == 0 ? 255 : 0;
                    buffer[i+2] = (x + y) % 2 == 0 ? 255 : 0;
                    buffer[i+3] = 255;
                }
            cb(adUnitId.c_str(), buffer, width, height);
            std::lock_guard<std::mutex> lock(_ad_mutex);
            _adThumbnails[adUnitId] = buffer;
        }).detach();
    }
    void AdSDK::Dispose(const char* adUnitId) {
        std::lock_guard<std::mutex> lock(_ad_mutex);
        delete[] _adThumbnails[adUnitId];
        _adThumbnails.erase(adUnitId);
    }
