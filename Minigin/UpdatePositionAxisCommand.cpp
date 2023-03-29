#include "UpdatePositionAxisCommand.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace dae;

UpdatePositionAxisCommand::UpdatePositionAxisCommand(GameObject* pGameObject, float speed)
    : AxisValueCommand{}, m_pGameObject(pGameObject),
    m_OldPosition(pGameObject->GetLocalPosition()), m_Speed(speed),
    m_pSceneManager(&SceneManager::GetInstance())
{};

void UpdatePositionAxisCommand::Execute()
{
    if (!m_pGameObject) return;

    m_OldPosition = m_pGameObject->GetLocalPosition();

    glm::vec2 newPosition = m_OldPosition + m_Value * m_Speed * m_pSceneManager->GetDeltaTime();
    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}

void UpdatePositionAxisCommand::Undo()
{
    if (!m_pGameObject) return;

    m_pGameObject->SetPosition(m_OldPosition.x, m_OldPosition.y);
}


