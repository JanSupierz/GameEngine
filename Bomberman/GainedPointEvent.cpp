#include "GainedPointEvent.h"

dae::GainedPointEvent::GainedPointEvent(PlayerComponent* pPlayer)
    :m_pPlayer{ pPlayer }
{
}

dae::PlayerComponent* dae::GainedPointEvent::GetPlayer() const
{
    return m_pPlayer;
}
