#include "DetonateBombCommand.h"
#include "PlayerComponent.h"

dae::DetonateBombCommand::DetonateBombCommand(PlayerComponent* pPlayer)
	:m_pPlayer{ pPlayer }
{
}

void dae::DetonateBombCommand::Execute()
{
	m_pPlayer->DetonateOldestBomb();
}
