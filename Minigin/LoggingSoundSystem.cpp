#include "LoggingSoundSystem.h"
#include "LoggingSystem.h"
#include "ServiceLocator.h"
#include <sstream>

void LoggingSoundSystem::Play(const int soundId, const float volume)
{
	m_pRealSoundSystem->Play(soundId, volume);

	std::stringstream ss{};
	ss << "Playing sound with ID " << soundId << " at volume " << volume;

	ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
}

void LoggingSoundSystem::AddSound(const std::string& fileName, int& soundId)
{
	m_pRealSoundSystem->AddSound(fileName, soundId);

	std::stringstream ss{};
	ss << "Sound added: " << fileName << " (ID: " << soundId << ")";

	ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
}

void LoggingSoundSystem::Preload(const int soundId)
{
	m_pRealSoundSystem->Preload(soundId);

	std::stringstream ss{};
	ss << "Preloaded sound with ID " << soundId;

	ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
}
