#include "StartInfoComponent.h"
#include "GameObject.h"
#include "imgui.h"

void dae::StartInfoComponent::Render()
{
    ImGui::Begin("Start Info");

    ImGui::Spacing();

    ImGui::Text("Controller:");
    ImGui::Text("Place bombs wih 'A'");
    ImGui::Text("Detonate bombs wih 'B'");
    ImGui::Text("(Power up needed)");
    ImGui::Text("Move using the DPad");

    ImGui::Spacing();

    ImGui::Text("Keyboard:");
    ImGui::Text("Place bombs wih 's'");
    ImGui::Text("Detonate bombs wih 'e'");
    ImGui::Text("(Power up needed)");
    ImGui::Text("Move using the arrows");

    ImGui::Spacing();

    ImGui::Text("Goal:");
    ImGui::Text("Kill all enemies");
    ImGui::Text("Find the door");

    ImGui::Spacing();

    ImGui::Text("Hazards:");
    ImGui::Text("Some enemies may follow you");

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
