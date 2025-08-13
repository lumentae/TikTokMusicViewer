#include "DataStore.h"

#include <filesystem>
#include <iostream>
#include <ranges>

#include "File.h"

#ifdef PlaySound
#undef PlaySound
#endif

ma_sound* DataStore::GetCurrentlyPlaying() const
{
    return mCurrentlyPlaying;
}

void DataStore::PlaySound(const long long id)
{
    std::cout << "Playing sound " << id << std::endl;

    const std::string key = std::to_string(id);
    auto path = File::GetFileFromCacheByName(key);

    if (path.extension() == ".m4a")
    {
        const std::filesystem::path replacement{".mp3"};
        std::filesystem::path replacementPath = path;
        replacementPath.replace_extension(replacement);

        if (!std::filesystem::exists(replacementPath))
        {
            File::ConvertFile(path, replacementPath);
        }
        path = replacementPath;
    }

    const auto sound = new ma_sound();
    if (const auto r = ma_sound_init_from_file(&mEngine, path.string().c_str(), MA_SOUND_FLAG_STREAM, nullptr, nullptr, sound); r != MA_SUCCESS)
    {
        Reset();
        delete sound;
        throw std::runtime_error("Failed to load sound " + path.string() + ": " + ma_result_description(r));
    }

    if (const ma_result r = ma_sound_seek_to_second(sound, 0); r != MA_SUCCESS)
    {
        Reset();
        delete sound;
        throw std::runtime_error("ma_sound_start failed");
    }

    if (const ma_result r = ma_sound_start(sound); r != MA_SUCCESS)
    {
        Reset();
        delete sound;
        throw std::runtime_error("ma_sound_start failed");
    }

    ma_sound_set_looping(sound, mLooping);

    Reset();
    mCurrentlyPlaying = sound;
    mCurrentlyPlayingId = id;
}

void DataStore::Loop()
{
    mLooping = !mLooping;
    if (mCurrentlyPlaying)
        ma_sound_set_looping(mCurrentlyPlaying, mLooping);
    std::cout << "Looping is now " << (mLooping ? "enabled" : "disabled") << std::endl;
}

bool DataStore::GetLooping() const
{
    return mLooping;
}

void DataStore::StopSound()
{
    if (mCurrentlyPlaying)
    {
        ma_sound_stop(mCurrentlyPlaying);
        mCurrentlyPlayingId = 0;
    }
}

void DataStore::Reset()
{
    if (mCurrentlyPlaying)
    {
        ma_sound_stop(mCurrentlyPlaying);
        ma_sound_uninit(mCurrentlyPlaying);
        delete mCurrentlyPlaying;
        mCurrentlyPlayingId = 0;
    }
}

long long DataStore::GetCurrentlyPlayingId() const
{
    return mCurrentlyPlayingId;
}

DataStore::~DataStore()
{
    StopSound();
    ma_engine_uninit(&mEngine);
}
