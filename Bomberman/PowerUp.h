#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class CollisionEvent;

	enum class PowerUpType
	{
		ExtraBomb, Detonator, Flames
	};

	class PowerUp final : public Component, public Observer<CollisionEvent>
	{
	public:
		PowerUp(PowerUpType type, Subject<CollisionEvent>* pCollider);
		virtual ~PowerUp();
		PowerUp(const PowerUp& other) = default;
		PowerUp(PowerUp&& other) = default;
		PowerUp& operator=(const PowerUp& other) = default;
		PowerUp& operator=(PowerUp&& other) = default;

		void OnNotify(const CollisionEvent& event) override;
		void OnSubjectDestroy(Subject<CollisionEvent>*) override;

		static void SetSound(int id);
	private:
		Subject<CollisionEvent>* m_pCollider;
		PowerUpType m_Type;
		static int s_PowerUpSoundId;
	};
}

