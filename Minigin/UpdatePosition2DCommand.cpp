#include "UpdatePosition2DCommand.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace dae;

UpdatePosition2DCommand::UpdatePosition2DCommand(GameObject* pGameObject, float speed)
    : Value2DCommand{}, m_pGameObject(pGameObject),
    m_OldPosition(pGameObject->GetLocalPosition()), m_Speed(speed),
    m_pSceneManager(&SceneManager::GetInstance())
{};

void UpdatePosition2DCommand::Execute()
{
    if (!m_pGameObject) return;

    m_OldPosition = m_pGameObject->GetLocalPosition();

    glm::vec2 newPosition = m_OldPosition + m_Value * m_Speed * m_pSceneManager->GetDeltaTime();
    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}

void UpdatePosition2DCommand::Undo()
{
    if (!m_pGameObject) return;

    m_pGameObject->SetPosition(m_OldPosition.x, m_OldPosition.y);
}


