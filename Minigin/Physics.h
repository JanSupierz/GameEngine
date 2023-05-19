#pragma once
#include <vector>
#include "Singleton.h"

namespace dae
{
    class ColliderComponent;

    class Physics final : public Singleton<Physics>
    {
    public:
        void AddCollider(ColliderComponent* pCollider);
        void RemoveCollider(ColliderComponent* pCollider);

        void LayerChanged(ColliderComponent* pCollider, unsigned int oldLayer);

        const std::vector<ColliderComponent*>& GetColliders(unsigned int layer = 0) const;

    private:
        std::vector<std::vector<ColliderComponent*>> m_pCollidersByLayer;

        friend class Singleton<Physics>;
        Physics() = default;
    };
}