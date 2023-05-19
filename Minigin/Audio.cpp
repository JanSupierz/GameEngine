#include "Audio.h"

using namespace dae;

std::unique_ptr<SoundSystem> Audio::s_Instance{ std::make_unique<NullSoundSystem>() };

SoundSystem& Audio::Get()
{
	return *s_Instance;
}

void Audio::RegisterService(std::unique_ptr<SoundSystem>&& pService)
{
	s_Instance = (pService == nullptr ? std::make_unique<NullSoundSystem>() : std::move(pService));
}