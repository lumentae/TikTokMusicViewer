#pragma once
#include <functional>
#include <iostream>

#include "miniaudio.h"

#ifdef PlaySound
#undef PlaySound
#endif

class DataStore
{
public:
    using SoundPtr = std::unique_ptr<ma_sound, std::function<void(ma_sound*)>>;

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

    [[nodiscard]] ma_sound* GetCurrentlyPlaying() const;
    [[nodiscard]] long long GetCurrentlyPlayingId() const;
    void PlaySound(long long id);
    void Loop();
    bool GetLooping() const;
    void StopSound();

private:
    DataStore() = default;
    ~DataStore();

    ma_sound* mCurrentlyPlaying = nullptr;
    long long mCurrentlyPlayingId = 0;
    ma_engine mEngine;

    bool mLooping = false;
};
