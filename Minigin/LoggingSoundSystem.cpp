#include "LoggingSoundSystem.h"
#include "LoggingSystem.h"
#include "ServiceLocator.h"
#include <sstream>

void LoggingSoundSystem::Play(const int soundId, const float volume)
{
	m_pRealSoundSystem->Play(soundId, volume);

	std::stringstream ss{};
	ss << "Playing " << soundId << " at volume " << volume;

	ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
}

void LoggingSoundSystem::AddSound(const std::string& fileName, int& soundId)
{
	m_pRealSoundSystem->AddSound(fileName, soundId);

	std::stringstream ss{};
	ss << "Added sound: " << fileName << ", sound id: " << soundId;

	ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
}
