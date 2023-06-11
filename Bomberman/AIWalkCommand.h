#pragma once
#include "Command.h"
#include <glm/glm.hpp>
#include <vector>
#include "Direction.h"

namespace dae
{
    class GameObject;
    class SceneManager;
    class NavigationGrid;
    class NavigationNode;

    class AIWalkCommand final : public Command
    {
    public:
        AIWalkCommand(GameObject* pGameObject, float speed, bool isSmart);

        void SetTarget(GameObject* pTarget);
        void Execute() override;

    private:
        GameObject* m_pGameObject;
        SceneManager* m_pSceneManager{ nullptr };
        NavigationGrid* m_pGrid{ nullptr };
        NavigationNode* m_pCurrentNode{ nullptr };
        Direction m_Direction{};
        float m_Speed;
        GameObject* m_pTarget{ nullptr };
        std::vector<Direction> m_DirectionsToCheck{};
        bool m_IsSmart;
    };
}

