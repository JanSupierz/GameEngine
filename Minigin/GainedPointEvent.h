#pragma once
#include "Subject.h"

namespace dae
{
	class PlayerComponent;

	class GainedPointEvent final : public Subject<GainedPointEvent>
	{
	public:
		void operator()(PlayerComponent* pPlayer);
		PlayerComponent* GetPlayer() const;
	private:
		PlayerComponent* m_pPlayer;
	};
}

