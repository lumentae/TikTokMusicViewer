#include "ImageComponent.h"
#include "../utils/File.h"

void ImageComponent::Render(const std::string& filename)
{
    std::filesystem::path cachePath = File::GetCachePath();
    ImGui::GetForegroundDrawList()->AddImage(DataStore::GetInstance().GetTexture(filename), {0, 0}, {1, 1});
}