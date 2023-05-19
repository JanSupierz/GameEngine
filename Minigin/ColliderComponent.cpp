#include "ColliderComponent.h"
#include "GameObject.h"
#include "Physics.h"
#include "EventManager.h"

dae::ColliderComponent::ColliderComponent(const glm::vec2& size, int priority)
	:Component(priority), m_Size{ size }
{
    Physics::GetInstance().AddCollider(this);
    m_pCollisionEvent = std::make_unique<CollisionEvent>();
}

dae::ColliderComponent::~ColliderComponent()
{
    Physics::GetInstance().RemoveCollider(this);
}

void dae::ColliderComponent::Update()
{
    // Check for collisions with other colliders
    for (auto& pCollider : Physics::GetInstance().GetColliders(m_Layer))
    {
        if (pCollider != this && CheckCollision(pCollider->GetOwner()->GetWorldPosition(), pCollider->GetSize()))
        {
            m_pCollisionEvent->NotifyObservers(this, pCollider);
        }
    }
}

bool dae::ColliderComponent::CheckCollision(const glm::vec2& otherPosition, const glm::vec2& otherSize)
{
    constexpr float margin{ 0.1f };
    constexpr float half{ 0.5f };

    const float multiplier{ half - margin };

    //Calculate the AABB for the other object
    glm::vec2 otherMin{ otherPosition - otherSize * multiplier };
    glm::vec2 otherMax{ otherPosition + otherSize * multiplier };

    //Calculate the AABB for this ColliderComponent
    glm::vec2 position{ GetOwner()->GetWorldPosition() };

    glm::vec2 min{ position - m_Size * multiplier };
    glm::vec2 max{ position + m_Size * multiplier };

    //Check for intersection
    return (otherMin.x <= max.x && otherMax.x >= min.x) && (otherMin.y <= max.y && otherMax.y >= min.y);
}

void dae::ColliderComponent::SetLayer(unsigned int layer)
{
    unsigned int oldLayer{ m_Layer };
    m_Layer = layer;

    Physics::GetInstance().LayerChanged(this, oldLayer);
}
