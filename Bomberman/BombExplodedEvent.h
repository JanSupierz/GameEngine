#pragma once
#include "QueueEvent.h"
#include <glm/glm.hpp>

namespace dae
{
    class PlayerComponent;

    class BombExplodedEvent final:
        public QueueEvent
    {
    public:
        BombExplodedEvent(const glm::vec2& position, PlayerComponent* pPlayer);
        glm::vec2 GetPosition() const;
        PlayerComponent* GetPlayer() const;
    private:
        glm::vec2 m_Position{};
        PlayerComponent* m_pPlayer{};
    };
}

