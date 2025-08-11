#pragma once

#include <filesystem>
#include <fstream>
#include <curl/curl.h>

#include "imgui.h"
#include "httplib.h"
#include "stb_image.h"
#include "DataStore.h"

class File
{
public:
    static std::filesystem::path GetCachePath()
    {
        std::filesystem::path cachePath = "cache";
        if (!std::filesystem::exists(cachePath))
        {
            std::filesystem::create_directory(cachePath);
        }
        return cachePath;
    }

    static std::filesystem::path GetFileFromCache(const std::string& filename)
    {
        const auto cachePath = GetCachePath();
        return cachePath / filename;
    }

    static std::filesystem::path GetFileFromCacheByName(const std::string& filenameWithoutExtension)
    {
        const auto cachePath = GetCachePath();
        for (const auto& entry : std::filesystem::directory_iterator(cachePath))
        {
            if (!entry.is_regular_file())
                continue;

            if (entry.path().filename().string().find(filenameWithoutExtension) != std::string::npos)
                return entry.path();
        }
        return cachePath / filenameWithoutExtension;
    }

    static std::string GetExtensionFromMime(const std::string& mime)
    {
        if (mime == "audio/mpeg") return ".mp3";
        if (mime == "audio/ogg") return ".ogg";
        if (mime == "audio/wav" || mime == "audio/x-wav") return ".wav";
        if (mime == "audio/flac") return ".flac";
        if (mime == "audio/mp4") return ".m4a";
        std::cout << "Unknown mime type: " << mime << "\n";
        return "";
    }

    static std::string ReadFile(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + path.string());
        }

        std::string content((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
        return content;
    }

    static void WriteFile(const std::filesystem::path& path, const std::string& content)
    {
        std::ofstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + path.string());
        }
        file << content;
        file.close();
    }

    static size_t WriteData(void* ptr, const size_t size, const size_t nmemb, void* userdata)
    {
        auto* ofs = static_cast<std::ofstream*>(userdata);
        ofs->write(static_cast<char*>(ptr), size * nmemb);
        return size * nmemb;
    }

    static bool DownloadFile(const std::string& url, const std::string& output_path)
    {
        std::string outMime;
        return DownloadFile(url, output_path, outMime);
    }

    static bool DownloadFile(const std::string& url, const std::string& output_path, std::string& outMime)
    {
        CURL* curl = curl_easy_init();
        if (!curl)
        {
            std::cerr << "Failed to initialize libcurl\n";
            return false;
        }

        std::ofstream ofs(output_path, std::ios::binary);
        if (!ofs)
        {
            std::cerr << "Failed to open output file: " << output_path << "\n";
            curl_easy_cleanup(curl);
            return false;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);

        if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            ofs.close();
            curl_easy_cleanup(curl);
            return false;
        }

        char* contentType = nullptr;
        if (curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType) == CURLE_OK && contentType) {
            outMime = contentType;
        }

        ofs.close();
        curl_easy_cleanup(curl);
        return true;
    }

    // Do not use
    static bool LoadTextureFromMemory(const void* data, const size_t data_size, GLuint* out_texture, int* out_width,
                                      int* out_height)
    {
        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load_from_memory(static_cast<const unsigned char*>(data),
                                                          static_cast<int>(data_size), &image_width, &image_height,
                                                          nullptr, 4);
        if (image_data == nullptr)
            return false;

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        return true;
    }

    // Do not use
    static bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height)
    {
        FILE* f;
        fopen_s(&f, file_name, "rb");
        if (f == nullptr)
            return false;

        fseek(f, 0, SEEK_END);
        const auto file_size = static_cast<size_t>(ftell(f));
        if (file_size == -1)
            return false;

        fseek(f, 0, SEEK_SET);
        void* file_data = IM_ALLOC(file_size);
        fread(file_data, 1, file_size, f);
        fclose(f);

        const bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width, out_height);
        IM_FREE(file_data);
        return ret;
    }

    static GLuint LoadTextureFromFile(const std::filesystem::path& file, int* out_width, int* out_height)
    {
        GLuint out_texture;
        if (!LoadTextureFromFile(file.string().c_str(), &out_texture, out_width, out_height))
            return -1;

        DataStore::GetInstance().AddTexture(file.filename().string(), out_texture);
        return out_texture;
    }

    static void LoadTexturesFromCache()
    {
        for (const auto cachePath = GetCachePath(); const auto& entry : std::filesystem::directory_iterator(cachePath))
        {
            if (!entry.is_regular_file())
                continue;

            int width, height;
            const auto texture = LoadTextureFromFile(entry.path(), &width, &height);
            if (texture == -1)
                continue;

            DataStore::GetInstance().AddTexture(entry.path().filename().string(), texture);
        }
    }

    static void LoadSoundsFromCache()
    {
        for (const auto cachePath = GetCachePath(); const auto& entry : std::filesystem::directory_iterator(cachePath))
        {
            if (!entry.is_regular_file())
                continue;

            auto name = entry.path().filename().string();
            std::cout << "Loading sound " << name << " from cache...\n";
            DataStore::GetInstance().AddSound(name, entry.path());
        }
    }

    static void DownloadTextureAndLoad(const std::string& url)
    {
        // TODO: Differentiate textures and sounds
        const std::filesystem::path cachePath = GetCachePath();
        if (const auto filename = cachePath / url.substr(url.find_last_of('/') + 1); !std::filesystem::exists(filename))
        {
            std::cout << "Downloading " << url << " to " << filename << "...\n";
            DownloadFile(url, filename.string());
            LoadTextureFromFile(filename, nullptr, nullptr);
        }
        else
        {
            std::cout << "Skipping " << url << " (already downloaded)\n";
        }
    }

    static void DownloadSoundAndLoad(const std::string& url, const long long id) {
        const std::filesystem::path cachePath = GetCachePath();

        // Temporary path without extension
        std::filesystem::path tempFile = GetFileFromCacheByName(std::to_string(id));

        if (!std::filesystem::exists(tempFile) && !std::filesystem::exists(tempFile.string() + ".mp3")) {
            std::string mimeType;
            std::cout << "Downloading " << url << "...\n";

            if (!DownloadFile(url, tempFile.string(), mimeType)) {
                std::cerr << "Failed to download " << url << "\n";
                return;
            }

            if (const std::string ext = GetExtensionFromMime(mimeType); !ext.empty()) {
                const std::filesystem::path newFile = tempFile.string() + ext;
                std::filesystem::rename(tempFile, newFile);
                tempFile = newFile;
            }
        }

        DataStore::GetInstance().AddSound(std::to_string(id), tempFile);
    }
    // static void DownloadSoundAndLoad(const std::string& url, const long long id)
    // {
    //     const std::filesystem::path cachePath = GetCachePath();
    //     if (const auto filename = cachePath / std::to_string(id).append(""); !std::filesystem::exists(filename))
    //     {
    //         std::cout << "Downloading " << url << " to " << filename << "...\n";
    //         std::string mimeType;
    //         DownloadFile(url, filename.string(), mimeType);
    //         DataStore::GetInstance().AddSound(std::to_string(id), filename);
    //     }
    //     else
    //     {
    //         std::cout << "Skipping " << url << " (already downloaded)\n";
    //         DataStore::GetInstance().AddSound(std::to_string(id), filename);
    //     }
    // }
};