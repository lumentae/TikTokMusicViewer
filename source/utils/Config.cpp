#include "Config.h"

namespace Config
{
    std::string Cookie;
    std::string SecUid;
    int Tutorial;

    void Init()
    {
        auto json = nlohmann::json::parse(File::ReadFile("config.json"));
        INTERNAL_LOAD_VALUE(Cookie);
        INTERNAL_LOAD_VALUE(SecUid);
        INTERNAL_LOAD_VALUE(Tutorial);
    }

    void Save()
    {
        nlohmann::json json;
        INTERNAL_SAVE_VALUE(Cookie);
        INTERNAL_SAVE_VALUE(SecUid);
        INTERNAL_SAVE_VALUE(Tutorial);

        File::WriteFile("config.json", json.dump(4));
    }
}