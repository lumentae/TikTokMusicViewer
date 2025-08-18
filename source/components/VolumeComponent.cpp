#include "VolumeComponent.h"

#include "imgui.h"
#include "utils/DataStore.h"

void VolumeComponent::Render()
{
    auto& datastore = DataStore::GetInstance();
    int volume = datastore.GetVolume();

    ImGui::BeginGroup();
    if (ImGui::Button(volume == 0 ? "Unmute" : "Mute"))
    {
        if (volume != 0)
            datastore.SetVolume(0);
        else
            datastore.SetVolume(100);
        volume = datastore.GetVolume();
    }
    ImGui::SameLine();
    ImGui::SliderInt("Volume", &volume, 0, 100);
    ImGui::EndGroup();

    if (volume != datastore.GetVolume())
        datastore.SetVolume(volume);
}
