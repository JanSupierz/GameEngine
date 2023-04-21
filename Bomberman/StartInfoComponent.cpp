#include "StartInfoComponent.h"
#include "GameObject.h"
#include "imgui.h"

void dae::StartInfoComponent::Render()
{
    ImGui::Begin("Start Info");

    ImGui::Spacing();

    ImGui::Text("Player1: Controller");
    ImGui::Text("Place bombs wih 'A'");
    ImGui::Text("Move using the DPad");

    ImGui::Spacing();

    ImGui::Text("Player2: Keyboard");
    ImGui::Text("Place bombs wih 's'");
    ImGui::Text("Move using the arrows");

    ImGui::Spacing();

    if (ImGui::Button("OK"))
    {
        GetOwner()->Destroy();
    }

    ImGui::End();
}

dae::StartInfoComponent::StartInfoComponent(int priority)
    :Component(priority)
{
}
