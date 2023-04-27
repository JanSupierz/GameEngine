#include "SetPositionCommand.h"
#include "GameObject.h"

using namespace dae;

SetPositionCommand::SetPositionCommand(GameObject* pGameObject, const glm::vec2& direction)
    : Command{}, m_pGameObject(pGameObject), m_Direction(direction) {};

void SetPositionCommand::Execute()
{
    if (!m_pGameObject) return;

    glm::vec2 oldPosition = m_pGameObject->GetLocalPosition();

    glm::vec2 newPosition = oldPosition + m_Direction;
    m_pGameObject->SetPosition(newPosition.x, newPosition.y);
}


