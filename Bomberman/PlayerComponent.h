#pragma once
#include "Component.h"
#include "EventListener.h"
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Observer.h"
#include "DeathEvent.h"

namespace dae
{
	class CollisionEvent;
	class BombExplodedEvent;
	class NavigationNode;

	class PlayerComponent final : public Component, public Observer<CollisionEvent>
	{
	public:
		PlayerComponent(const std::string& name, Subject<CollisionEvent>*, int priority = 0);
		virtual ~PlayerComponent();
		PlayerComponent(const PlayerComponent& other) = default;
		PlayerComponent(PlayerComponent&& other) = default;
		PlayerComponent& operator=(const PlayerComponent& other) = default;
		PlayerComponent& operator=(PlayerComponent&& other) = default;

		virtual void OnNotify(const CollisionEvent& event) override;
		virtual void OnSubjectDestroy(Subject<CollisionEvent>*) override;

		std::string GetName() const;

		NavigationNode* GetNode() const;
		void SetNode(NavigationNode* pNode);

		void Kill(DeathType type, PlayerComponent* pOther = nullptr);
		static void SetDeathSound(const int soundId);

	private:
		const std::string m_Name;

		Subject<CollisionEvent>* m_pCollider{};
		NavigationNode* m_pCurrentNode{ nullptr };

		bool m_Killed;

		static int s_PlayerDeathSound;
	};
}