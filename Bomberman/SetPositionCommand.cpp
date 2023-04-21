#include "SetPositionCommand.h"
#include "GameObject.h"

using namespace dae;

SetPositionCommand::SetPositionCommand(GameObject* pGameObject, const glm::vec3& direction)
    : Command{}, m_pGameObject(pGameObject), m_Direction(direction) {};

void SetPositionCommand::Execute()
{
    if (!m_pGameObject) return;

    glm::vec3 oldPosition = m_pGameObject->GetLocalPosition();

    glm::vec3 newPosition = oldPosition + m_Direction;
    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}


