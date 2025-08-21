#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <nlohmann/json.hpp>

// Singleton
class ApiManager
{
public:
    static ApiManager& GetInstance()
    {
        static ApiManager instance;
        return instance;
    }

    void Init();
    nlohmann::json MusicList(long long cursor = 0, long long count = 35);

    void SetEnableApi(const bool value) { mEnableApi = value; }
    [[nodiscard]] bool GetEnableApi() const { return mEnableApi; }

private:
    ApiManager() = default;
    ~ApiManager() = default;

    httplib::Client mClient{""};
    httplib::Headers mHeaders{};
    httplib::Params mParams{};

    bool mEnableApi = false;
};