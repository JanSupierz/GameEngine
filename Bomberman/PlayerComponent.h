#pragma once
#include "Component.h"
#include "Observer.h"
#include "DeathEvent.h"
#include "Eventlistener.h"
#include <vector>

namespace dae
{
	class BombComponent;
	class CollisionEvent;
	class NavigationNode;

	class PlayerComponent final : public Component, public Observer<CollisionEvent>
	{
	public:
		PlayerComponent(int index,bool canDetonate, Subject<CollisionEvent>*, int priority = 0);
		virtual ~PlayerComponent();
		PlayerComponent(const PlayerComponent& other) = default;
		PlayerComponent(PlayerComponent&& other) = default;
		PlayerComponent& operator=(const PlayerComponent& other) = default;
		PlayerComponent& operator=(PlayerComponent&& other) = default;

		virtual void OnNotify(const CollisionEvent& event) override;
		virtual void OnSubjectDestroy(Subject<CollisionEvent>*) override;

		int GetIndex() const;

		NavigationNode* GetNode() const;
		void SetNode(NavigationNode* pNode);

		void AddDetonateCommand();
		void Kill(DeathType type, PlayerComponent* pOther = nullptr);
		static void SetDeathSound(const int soundId);

		void DetonateOldestBomb();
		void AddBomb(BombComponent* pBomb);
		void RemoveBomb(BombComponent* pBomb);
	private:
		void CreateDetonateCommands();

		Subject<CollisionEvent>* m_pCollider{};
		NavigationNode* m_pCurrentNode{ nullptr };

		int m_Index;
		bool m_Killed;
		bool m_CanDetonate;

		static int s_PlayerDeathSound;

		std::vector<BombComponent*> m_pBombs;
	};
}