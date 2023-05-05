#pragma once
#include "Component.h"
#include "EventListener.h"

namespace dae
{
	class NavigationNode;
	class BombExplodedEvent;

	class BlockingComponent final : public Component, public EventListener<BombExplodedEvent>
	{
	public:
		BlockingComponent(NavigationNode* pNode, int priority = 0);
		virtual ~BlockingComponent();
		BlockingComponent(const BlockingComponent& other) = default;
		BlockingComponent(BlockingComponent&& other) = default;
		BlockingComponent& operator=(const BlockingComponent& other) = default;
		BlockingComponent& operator=(BlockingComponent&& other) = default;

		void OnEvent(const BombExplodedEvent& event);
	private:
		NavigationNode* m_pNode;
	};
}

