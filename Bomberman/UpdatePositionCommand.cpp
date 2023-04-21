#include "UpdatePositionCommand.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace dae;

UpdatePositionCommand::UpdatePositionCommand(GameObject* pGameObject, const glm::vec2& direction)
    : m_pGameObject(pGameObject),  
    m_Direction(direction), 
    m_pSceneManager(&SceneManager::GetInstance())
{};

void UpdatePositionCommand::Execute()
{
    if (!m_pGameObject) return;

    glm::vec2 oldPosition = m_pGameObject->GetLocalPosition();

    glm::vec2 newPosition = oldPosition + m_Direction * m_pSceneManager->GetDeltaTime();
    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}


