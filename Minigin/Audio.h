#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
	class Audio final
	{
		class NullSoundSystem final: public SoundSystem
		{
		public:
			virtual void Play(const int, const float, const int) {};
			virtual void Preload(const int) {};
			virtual void AddSound(const std::string&, int& soundId, bool) { soundId = -1; };
			virtual void ToggleMute() {};
		};

	public:
		static SoundSystem& Get();
		static void RegisterService(std::unique_ptr<SoundSystem>&& pService);

	private:
		static std::unique_ptr<SoundSystem> s_Instance;
	};
};

