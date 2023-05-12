#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SDL_SoundSystem final: public SoundSystem
	{
	public:
		SDL_SoundSystem();
		~SDL_SoundSystem();

		virtual void Play(const int soundId, const float volume) override;
		virtual void Preload(const int soundId) override;
		virtual void AddSound(const std::string& fileName, int& soundId) override;
	private:
		class SDL_SoundImpl;
		SDL_SoundImpl* m_pImpl;
	};
}
