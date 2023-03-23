#include "UpdatePositionCommand.h"
#include "GameObject.h"

using namespace dae;

UpdatePositionCommand::UpdatePositionCommand(GameObject* pGameObject, const glm::vec3& direction)
    : Command{}, m_pGameObject(pGameObject), m_OldPosition(pGameObject->GetLocalPosition()), m_Direction(direction) {};

void UpdatePositionCommand::Execute()
{
    if (!m_pGameObject) return;

    m_OldPosition = m_pGameObject->GetLocalPosition();
    glm::vec3 newPosition = m_OldPosition + m_Direction;

    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}

void UpdatePositionCommand::Undo()
{
    if (!m_pGameObject) return;

    m_pGameObject->SetPosition(m_OldPosition.x, m_OldPosition.y);
}


