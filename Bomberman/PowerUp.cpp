#include "PowerUp.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "BombermanManager.h"

dae::PowerUp::PowerUp(PowerUpType type, Subject<CollisionEvent>* pCollider)
    :Component(-8), m_pCollider{ pCollider }, m_Type{ type }
{
    m_pCollider->AddObserver(this);
}

dae::PowerUp::~PowerUp()
{
    if (m_pCollider)
    {
        m_pCollider->RemoveObserver(this);
    }
}

void dae::PowerUp::OnNotify(const CollisionEvent& event)
{
    ColliderComponent* pOther{ event.GetOtherCollider() };

    if (pOther)
    {
        auto pPlayer{ pOther->GetOwner()->GetComponent<PlayerComponent>() };

        if (pPlayer)
        {
            BombermanManager::GetInstance().AddPowerUp(m_Type, pPlayer->GetIndex());

            if (m_Type == PowerUpType::Detonator)
            {
                pPlayer->AddDetonateCommand();
            }

            GetOwner()->Destroy();
        }
    }
}

void dae::PowerUp::OnSubjectDestroy(Subject<CollisionEvent>*)
{
    m_pCollider = nullptr;
}