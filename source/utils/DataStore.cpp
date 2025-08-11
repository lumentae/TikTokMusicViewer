#include "DataStore.h"

#include <filesystem>
#include <iostream>
#include <ranges>

#include "File.h"

#ifdef PlaySound
#undef PlaySound
#endif

GLuint DataStore::GetTexture(const std::string& name)
{
    return mTextures[name];
}

void DataStore::AddTexture(const std::string& name, const GLuint texture)
{
    mTextures[name] = texture;
}

void DataStore::AddSound(const std::string& name, const std::filesystem::path& path)
{
    if (mSounds.contains(name)) return; // already loaded

    const auto p = new ma_sound();
    if (const ma_result r = ma_sound_init_from_file(&mEngine, path.string().c_str(), 0, nullptr, nullptr, p); r != MA_SUCCESS) {
        std::cerr << "Failed to load sound " << path << ": " << ma_result_description(r) << "\n";
        delete p;
        return;
    }

    // custom deleter ensures ma_sound_uninit gets called
    SoundPtr sp(p, [](ma_sound* s) {
        if (s) {
            ma_sound_uninit(s);
            delete s;
        }
    });

    mSounds[name] = std::move(sp);
    std::cout << "Loaded sound: " << path << " (key=" << name << ")\n";
}

ma_sound* DataStore::GetCurrentlyPlaying() const
{
    return mCurrentlyPlaying;
}

void DataStore::PlaySound(const long long id)
{
    const std::string key = std::to_string(id); // must match how you called AddSound
    const auto it = mSounds.find(key);
    if (it == mSounds.end()) {
        std::cerr << "Sound not found for id=" << id << "\n";
        return;
    }

    // stop previous (if any)
    if (mCurrentlyPlaying) {
        if (const ma_result r = ma_sound_stop(mCurrentlyPlaying); r != MA_SUCCESS) {
            std::cerr << "ma_sound_stop failed: " << ma_result_description(r) << "\n";
        }
        mCurrentlyPlaying = nullptr;
    }

    // start the stored sound (don't copy it)
    ma_sound* soundPtr = it->second.get();
    if (const ma_result r = ma_sound_start(soundPtr); r != MA_SUCCESS) {
        std::cerr << "ma_sound_start failed: " << ma_result_description(r) << "\n";
        return;
    }

    mCurrentlyPlaying = soundPtr;
}

DataStore::~DataStore()
{
    ma_engine_uninit(&mEngine);
    if (!mTextures.empty())
    {
        std::cout << "Deleting " << mTextures.size() << " textures...\n";
        for (auto& texID : mTextures | std::views::values)
        {
            glDeleteTextures(1, &texID);
        }
    }
    mSounds.clear();
    if (mCurrentlyPlaying) {
        ma_sound_stop(mCurrentlyPlaying);
        mCurrentlyPlaying = nullptr;
    }
}
