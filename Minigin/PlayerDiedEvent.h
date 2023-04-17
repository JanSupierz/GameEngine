#pragma once
#include "Subject.h"

namespace dae
{
	class PlayerComponent;

	class PlayerDiedEvent final : public Subject<PlayerDiedEvent>
	{
	public:
		void operator()(PlayerComponent* pKiller, PlayerComponent* pKilled);

		PlayerComponent* GetKiller() const;
		PlayerComponent* GetKilled() const;

	private:
		PlayerComponent* m_pKiller{};
		PlayerComponent* m_pKilled{};
	};
}

