#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;

    class SetPositionCommand final : public Command
    {
    public:
        SetPositionCommand(GameObject* pGameObject, const glm::vec2& direction);
        void Execute() override;
    private:
        GameObject* m_pGameObject;
        glm::vec2 m_Direction;
    };
}

