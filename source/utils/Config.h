#pragma once
#include <nlohmann/json.hpp>

#include "File.h"

namespace Config
{
    extern std::string Cookie;
    extern std::string SecUid;
    extern int Tutorial;

    void Init();
    void Load();
    void Save();
};

#define INTERNAL_LOAD_VALUE(name) \
    name = json[#name];

#define INTERNAL_SAVE_VALUE(name) \
    json[#name] = name;

#define CONFIG_SET_VALUE(name, value) \
    Config::name = value; \
    Config::Save()