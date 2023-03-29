#pragma once
#include "SingleValueCommand.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
    class SceneManager;

    class UpdatePositionTriggerCommand final : public SingleValueCommand
    {
    public:
        UpdatePositionTriggerCommand(GameObject* pGameObject, const glm::vec2& direction);

        void Execute() override;
        void Undo();

    private:
        GameObject* m_pGameObject;
        SceneManager* m_pSceneManager{ nullptr };
        glm::vec2 m_OldPosition;
        const glm::vec2 m_Direction;
    };
}

