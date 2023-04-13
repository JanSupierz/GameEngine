#pragma once
#include "Value2DCommand.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
    class SceneManager;

    class UpdatePosition2DCommand final : public Value2DCommand
    {
    public:
        UpdatePosition2DCommand(GameObject* pGameObject, float speed);

        void Execute() override;

    private:
        GameObject* m_pGameObject;
        SceneManager* m_pSceneManager{ nullptr };
        const float m_Speed;
    };
}

