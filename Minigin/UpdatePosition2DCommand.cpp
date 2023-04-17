#include "UpdatePosition2DCommand.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace dae;

UpdatePosition2DCommand::UpdatePosition2DCommand(GameObject* pGameObject, float speed)
    : Value2DCommand{}, m_pGameObject(pGameObject), m_Speed(speed),
    m_pSceneManager(SceneManager::GetInstance().get())
{};

void UpdatePosition2DCommand::Execute()
{
    if (!m_pGameObject) return;

    glm::vec2 oldPosition = m_pGameObject->GetLocalPosition();

    glm::vec2 newPosition = oldPosition + m_Value * m_Speed * m_pSceneManager->GetDeltaTime();
    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}


