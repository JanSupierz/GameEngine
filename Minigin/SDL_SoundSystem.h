#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SDL_SoundSystem final: public SoundSystem
	{
	public:
		SDL_SoundSystem();
		virtual ~SDL_SoundSystem();

		virtual void Play(const int soundId, const float volume, const int nrLoops) override;
		virtual void Preload(const int soundId) override;
		virtual void AddSound(const std::string& fileName, int& soundId, bool isMusic) override;

	private:
		class SDL_SoundImpl;
		SDL_SoundImpl* m_pImpl;
	};
}

