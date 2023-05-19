#include "Physics.h"
#include "ColliderComponent.h"

void dae::Physics::AddCollider(ColliderComponent* pCollider)
{
    const unsigned int layer{ pCollider->GetLayer() };

    //Resize number layers if needed
    if (layer >= m_pCollidersByLayer.size())
    {
        m_pCollidersByLayer.resize(layer + 1);
    }
    
    //Add to the correct layer
    m_pCollidersByLayer[layer].push_back(pCollider);
}

void dae::Physics::RemoveCollider(ColliderComponent* pCollider)
{
    //Remove collider form layer
    const unsigned int layer{ pCollider->GetLayer() };

    if (layer < m_pCollidersByLayer.size())
    {
        auto& pColliders{ m_pCollidersByLayer[layer] };
        pColliders.erase(std::remove(pColliders.begin(), pColliders.end(), pCollider), pColliders.end());
    }
}

void dae::Physics::LayerChanged(ColliderComponent* pCollider, unsigned int oldLayer)
{
    //Remove collider form layer
    const unsigned int layer{ pCollider->GetLayer() };

    //Layer is the same
    if (layer == oldLayer) return;

    //Remove from old layer
    if (oldLayer < m_pCollidersByLayer.size())
    {
        auto& pColliders{ m_pCollidersByLayer[layer] };
        pColliders.erase(std::remove(pColliders.begin(), pColliders.end(), pCollider), pColliders.end());
    }

    //Resize number layers if needed
    if (layer >= m_pCollidersByLayer.size())
    {
        m_pCollidersByLayer.resize(layer + 1);
    }

    //Add to the correct layer
    m_pCollidersByLayer[layer].push_back(pCollider);
}

const std::vector<dae::ColliderComponent*>& dae::Physics::GetColliders(unsigned int layer) const
{
    //Return an empty layer
    if (layer >= m_pCollidersByLayer.size())
    {
        const static std::vector<ColliderComponent*> emptyVector;
        return emptyVector;
    }
    
    return m_pCollidersByLayer[layer];
}
