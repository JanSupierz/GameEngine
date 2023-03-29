#pragma once
#include "AxisValueCommand.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
    class SceneManager;

    class UpdatePositionAxisCommand final : public AxisValueCommand
    {
    public:
        UpdatePositionAxisCommand(GameObject* pGameObject, float speed);

        void Execute() override;
        void Undo();

    private:
        GameObject* m_pGameObject;
        SceneManager* m_pSceneManager{ nullptr };
        glm::vec2 m_OldPosition;
        const float m_Speed;
    };
}

