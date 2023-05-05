#pragma once
#include "QueueEvent.h"

namespace dae
{
    class PlayerComponent;
    class NavigationNode;

    class BombExplodedEvent final:
        public QueueEvent
    {
    public:
        BombExplodedEvent(NavigationNode* pNode, PlayerComponent* pPlayer);
        NavigationNode* GetNode() const;
        PlayerComponent* GetPlayer() const;
    private:
        NavigationNode* m_pNode{};
        PlayerComponent* m_pPlayer{};
    };
}

