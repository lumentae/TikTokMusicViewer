#pragma once

#include <filesystem>
#include <fstream>
#include <curl/curl.h>

#include "imgui.h"
#include "httplib.h"

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
        file.close();
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
            throw std::runtime_error("Failed to initialize curl");
        }

        std::ofstream ofs(output_path, std::ios::binary);
        if (!ofs)
        {
            curl_easy_cleanup(curl);
            throw std::runtime_error("Failed to open file: " + output_path);
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);

        if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK)
        {
            ofs.close();
            curl_easy_cleanup(curl);
            throw std::runtime_error("curl_easy_perform() failed");
        }

        char* contentType = nullptr;
        if (curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType) == CURLE_OK && contentType) {
            outMime = contentType;
        }

        ofs.close();
        curl_easy_cleanup(curl);
        return true;
    }

    static void DownloadSoundAndLoad(const std::string& url, const long long id) {
        // Temporary path without extension
        if (const std::filesystem::path tempFile = GetFileFromCacheByName(std::to_string(id)); !std::filesystem::exists(tempFile) && !std::filesystem::exists(tempFile.string() + ".mp3")) {
            std::string mimeType;
            std::cout << "Downloading " << url << "...\n";

            DownloadFile(url, tempFile.string(), mimeType);

            if (const std::string ext = GetExtensionFromMime(mimeType); !ext.empty()) {
                const std::filesystem::path newFile = tempFile.string() + ext;
                std::filesystem::rename(tempFile, newFile);
            }
        }
    }

    static void ConvertFile(const std::filesystem::path& inputPath, const std::filesystem::path& outputPath)
    {
        const std::string cmd = "ffmpeg -y -v error -i \"" + inputPath.string() + "\" \"" + outputPath.string() + "\"";

        if (const int result = std::system(cmd.c_str()); result != 0) {
            throw std::runtime_error("FFmpeg failed to convert: " + inputPath.string());
        }
    }

    static void DeleteFilesInCacheWithPrefix(const std::string& inputPath)
    {
        for (const auto cachePath = GetCachePath(); const auto& entry : std::filesystem::directory_iterator(cachePath))
        {
            if (!entry.is_regular_file())
                continue;

            if (entry.path().filename().string().find(inputPath) != std::string::npos)
                std::filesystem::remove(entry.path());
        }
    }
};