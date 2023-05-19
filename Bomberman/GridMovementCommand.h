#pragma once
#include "Value2DCommand.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
    class SceneManager;
    class NavigationGrid;
    class PlayerComponent;

    class GridMovementCommand final : public Value2DCommand
    {
    public:
        GridMovementCommand(GameObject* pGameObject, const glm::vec2& direction, PlayerComponent* pPlayer, bool useValue2D = false);
        void Execute() override;

    private:
        GameObject* m_pGameObject;
        PlayerComponent* m_pPlayer;
        SceneManager* m_pSceneManager{ nullptr };
        NavigationGrid* m_pGrid{ nullptr };
        glm::vec2 m_Direction;
        float m_Speed;
        const bool m_UseValue2D;
    };
}

