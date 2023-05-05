#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
    class SceneManager;
    class NavigationGrid;
    class NavigationNode;

    class GridMovementCommand final : public Command
    {
    public:
        GridMovementCommand(GameObject* pGameObject, const glm::vec2& direction);
        void Execute() override;

    private:
        GameObject* m_pGameObject;
        SceneManager* m_pSceneManager{ nullptr };
        NavigationGrid* m_pGrid{ nullptr };
        NavigationNode* m_pCurrentNode{ nullptr };
        glm::vec2 m_Direction;
        float m_Speed;
    };
}

