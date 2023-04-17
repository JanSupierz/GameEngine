#include "GainedPointEvent.h"

void dae::GainedPointEvent::operator()(PlayerComponent* pPlayer)
{
    m_pPlayer = pPlayer;

    Notify(*this);
}

dae::PlayerComponent* dae::GainedPointEvent::GetPlayer() const
{
    return m_pPlayer;
}
