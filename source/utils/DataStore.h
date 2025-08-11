#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "miniaudio.h"

#ifdef PlaySound
#undef PlaySound
#endif

class DataStore
{
public:
    static DataStore& GetInstance()
    {
        static DataStore instance;
        return instance;
    }

    void Init()
    {
        if (const ma_result result = ma_engine_init(nullptr, &mEngine); result != MA_SUCCESS)
        {
            throw std::runtime_error("Failed to initialize miniaudio engine");
        }
    }

    GLuint GetTexture(const std::string& name);
    void AddTexture(const std::string& name, GLuint texture);
    void AddSound(const std::string& name, const std::filesystem::path& path);
    ma_sound* GetCurrentlyPlaying() const;
    void PlaySound(long long id);

private:
    DataStore() = default;
    ~DataStore();

    std::unordered_map<std::string, GLuint> mTextures{};

    using SoundPtr = std::unique_ptr<ma_sound, std::function<void(ma_sound*)>>;
    std::unordered_map<std::string, SoundPtr> mSounds{};
    ma_sound* mCurrentlyPlaying = nullptr;
    ma_engine mEngine;
};
