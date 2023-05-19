#pragma once
#include "SoundSystem.h"
#include "LoggingSystem.h"

namespace dae
{
	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem)
			:m_pRealSoundSystem{ std::move(pSoundSystem) } {};

		virtual ~LoggingSoundSystem() = default;

		virtual void Play(const int soundId, const float volume) override;
		virtual void Preload(const int soundId) override;
		virtual void AddSound(const std::string& fileName, int& soundId) override;
	private:
		std::unique_ptr<SoundSystem> m_pRealSoundSystem;
	};
}

