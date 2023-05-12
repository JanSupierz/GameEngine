#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
    class SceneManager;
    class NavigationGrid;
    class PlayerComponent;

    class GridMovementCommand final : public Command
    {
    public:
        GridMovementCommand(GameObject* pGameObject, const glm::vec2& direction, PlayerComponent* pPlayer);
        void Execute() override;

    private:
        GameObject* m_pGameObject;
        PlayerComponent* m_pPlayer;
        SceneManager* m_pSceneManager{ nullptr };
        NavigationGrid* m_pGrid{ nullptr };
        glm::vec2 m_Direction;
        float m_Speed;
    };
}

