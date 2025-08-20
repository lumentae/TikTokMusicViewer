#pragma once
#include <functional>
#include <iostream>
#include <memory>

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

    [[nodiscard]] ma_sound* GetCurrentlyPlaying() const;
    void PlaySound(long long id, int index = -1);
    void Loop();
    [[nodiscard]] bool GetLooping() const;
    void StopSound();
    void Reset();

    [[nodiscard]] int GetVolume() const { return mVolume; }
    void SetVolume(const int volume)
    {
        mVolume = volume;
        ma_engine_set_volume(&mEngine, mVolume / 100.0f);
    }

    void SetAutoPlay(const bool autoPlay) { mAutoPlay = autoPlay; }
    [[nodiscard]] bool GetAutoPlay() const { return mAutoPlay; }

    void SetCurrentlyPlayingId(const long long id) { mCurrentlyPlayingId = id; }
    [[nodiscard]] long long GetCurrentlyPlayingId() const { return mCurrentlyPlayingId; }

    void SetCurrentlyPlayingIndex(const int index) { mCurrentlyPlayingIndex = index; }
    [[nodiscard]] int GetCurrentlyPlayingIndex() const { return mCurrentlyPlayingIndex; }

private:
    DataStore() = default;
    ~DataStore();

    ma_sound* mCurrentlyPlaying = nullptr;
    long long mCurrentlyPlayingId = 0;
    long long mCurrentlyPlayingIndex = 0;
    ma_engine mEngine{};
    bool mAutoPlay = false;

    bool mLooping = false;
    int mVolume{100};
};
