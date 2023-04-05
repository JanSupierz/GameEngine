#include "UpdatePosition1DCommand.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace dae;

UpdatePosition1DCommand::UpdatePosition1DCommand(GameObject* pGameObject,const glm::vec2& direction)
    : Value1DCommand{}, m_pGameObject(pGameObject),
    m_OldPosition(pGameObject->GetLocalPosition()),
    m_pSceneManager(&SceneManager::GetInstance()), m_Direction( direction )
{};

void UpdatePosition1DCommand::Execute()
{
    if (!m_pGameObject) return;

    m_OldPosition = m_pGameObject->GetLocalPosition();

    glm::vec2 newPosition = m_OldPosition + m_Direction * m_Value * m_pSceneManager->GetDeltaTime();
    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}

void UpdatePosition1DCommand::Undo()
{
    if (!m_pGameObject) return;

    m_pGameObject->SetPosition(m_OldPosition.x, m_OldPosition.y);
}


