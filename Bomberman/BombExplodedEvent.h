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
        BombExplodedEvent(glm::vec3 position, PlayerComponent* pPlayer);
        glm::vec3 GetPosition() const;
        PlayerComponent* GetPlayer() const;
    private:
        glm::vec3 m_Position{};
        PlayerComponent* m_pPlayer{};
    };
}

