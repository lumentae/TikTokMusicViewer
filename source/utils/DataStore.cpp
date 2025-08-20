#include "DataStore.h"

#include <filesystem>
#include <iostream>
#include <ranges>

#include "File.h"
#include "screens/MainScreen.h"

#ifdef PlaySound
#undef PlaySound
#endif

ma_sound* DataStore::GetCurrentlyPlaying() const
{
    return mCurrentlyPlaying;
}

void onSoundEnd(void* pUserData, ma_sound* pSound)
{
    auto& datastore = DataStore::GetInstance();
    datastore.StopSound();

    if (!datastore.GetAutoPlay() || datastore.GetLooping())
    {
        return;
    }

    if (auto currentIndex = datastore.GetCurrentlyPlayingIndex(); currentIndex != -1)
    {
        datastore.SetCurrentlyPlayingIndex(currentIndex + 1);
        currentIndex++;
        if (const auto& musicList = MainScreen::GetInstance().GetMusicList(); currentIndex < musicList.size())
        {
            const auto& music = musicList["musicList"][currentIndex];
            const long long id = std::stoll(music["music"]["id"].get<std::string>());

            datastore.PlaySound(id, currentIndex);
        }
        else
        {
            datastore.Reset();
        }
    }
}

void DataStore::PlaySound(const long long id, const int index)
{
    std::cout << "Playing sound " << id << std::endl;
    if (index != -1)
        mCurrentlyPlayingIndex = index;

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

    Reset();

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

    ma_sound_set_end_callback(sound, onSoundEnd, nullptr);

    if (const ma_result r = ma_sound_start(sound); r != MA_SUCCESS)
    {
        Reset();
        delete sound;
        throw std::runtime_error("ma_sound_start failed");
    }

    ma_sound_set_looping(sound, mLooping);

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
        if (!mAutoPlay)
        {
            ma_sound_stop(mCurrentlyPlaying);
            ma_sound_uninit(mCurrentlyPlaying);
            delete mCurrentlyPlaying;
            mCurrentlyPlaying = nullptr;
        }
        mCurrentlyPlayingId = 0;
    }
}

DataStore::~DataStore()
{
    Reset();
    ma_engine_uninit(&mEngine);
}
