#include "AdSDK.h"
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <cstring>

struct CallbackTask {
    std::string adUnitId;
    std::string error;
    bool success;
    AdSuccessCallback successCb;
    AdFailureCallback failureCb;
};


class AdSDK {
    std::mutex g_mutex;
    std::queue<CallbackTask> g_callbacks;
    static AdSDK* instance;

    AdSDK() {}

public:
    ~AdSDK() {
        instance = nullptr;
    }

    static AdSDK* getInstance() {
        if (!instance) {
            instance = new AdSDK();
        }
        return instance;
    }

    void Init(const char* appId) {
    }
    void PreloadAd(const std::string& adUnitId, AdSuccessCallback onSuccess, AdFailureCallback onFailure) {
        std::thread([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            CallbackTask task;
            task.adUnitId = adUnitId;
            task.successCb = onSuccess;
            task.failureCb = onFailure;
            task.success = true;
            task.error = "";
            std::lock_guard<std::mutex> lock(g_mutex);
            g_callbacks.push(task);
        }).detach();
    }
    void Update() {
        std::lock_guard<std::mutex> lock(g_mutex);
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
    void GetAdThumbnail(const std::string& adUnitId, AdThumbnailCallback cb) {
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
        }).detach();
    }
    void FreeBuffer(const uint8_t* buffer) {
        delete[] buffer;
    }
};
