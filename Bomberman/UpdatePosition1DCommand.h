#pragma once
#include "Value1DCommand.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
    class SceneManager;

    class UpdatePosition1DCommand final : public Value1DCommand
    {
    public:
        UpdatePosition1DCommand(GameObject* pGameObject, const glm::vec2& direction);

        void Execute() override;

    private:
        GameObject* m_pGameObject;
        SceneManager* m_pSceneManager{ nullptr };
        const glm::vec2 m_Direction;
    };
}

