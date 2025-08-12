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
    nlohmann::json MusicList(int cursor = 0, int count = 35);
    [[nodiscard]] std::string GetCookie() const;
    void SetCookie(const std::string& cookie);

private:
    ApiManager() = default;
    ~ApiManager() = default;

    httplib::Client mClient{""};
    httplib::Headers mHeaders{};
    httplib::Params mParams{};
    std::string mCookie;
};