#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class LevelLoadComponent final : public Component
	{
	public:
		LevelLoadComponent();
		virtual ~LevelLoadComponent();
		LevelLoadComponent(const LevelLoadComponent& other) = default;
		LevelLoadComponent(LevelLoadComponent&& other) = default;
		LevelLoadComponent& operator=(const LevelLoadComponent& other) = default;
		LevelLoadComponent& operator=(LevelLoadComponent&& other) = default;

		virtual void Update() override;

		static void SetSound(int startId);
	private:
		float m_TimeLeft;
		bool m_IsInit;

		static int s_StartSoundId;
	};
}

