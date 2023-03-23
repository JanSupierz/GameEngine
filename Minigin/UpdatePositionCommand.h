#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;

    class UpdatePositionCommand : public Command
    {
    public:
        UpdatePositionCommand(GameObject* pGameObject, const glm::vec3& direction);

        void Execute() override;
        void Undo() override;

    private:
        GameObject* m_pGameObject;
        glm::vec3 m_OldPosition;
        glm::vec3 m_Direction;
    };
}

