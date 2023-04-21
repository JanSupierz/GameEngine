#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
    class SceneManager;

    class UpdatePositionCommand final : public Command
    {
    public:
        UpdatePositionCommand(GameObject* pGameObject, const glm::vec2& direction);
        void Execute() override;

    private:
        GameObject* m_pGameObject;
        SceneManager* m_pSceneManager{ nullptr };
        glm::vec2 m_Direction;
    };
}

