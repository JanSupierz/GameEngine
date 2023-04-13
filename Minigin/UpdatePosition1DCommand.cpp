#include "UpdatePosition1DCommand.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace dae;

UpdatePosition1DCommand::UpdatePosition1DCommand(GameObject* pGameObject,const glm::vec2& direction)
    : Value1DCommand{}, m_pGameObject(pGameObject),
    m_pSceneManager(&SceneManager::GetInstance()), m_Direction( direction )
{};

void UpdatePosition1DCommand::Execute()
{
    if (!m_pGameObject) return;

    glm::vec2 oldPosition = m_pGameObject->GetLocalPosition();

    glm::vec2 newPosition = oldPosition + m_Direction * m_Value * m_pSceneManager->GetDeltaTime();
    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}


