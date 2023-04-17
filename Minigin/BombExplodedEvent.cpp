#include "BombExplodedEvent.h"
#include "BombComponent.h"

using dae::BombComponent;

void dae::BombExplodedEvent::operator()(BombComponent* pExploded)
{
    m_pExploded = pExploded;
    Notify(*this);
}

BombComponent* dae::BombExplodedEvent::GetExpoloded() const
{
    return m_pExploded;
}
