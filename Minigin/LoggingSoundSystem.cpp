#include "LoggingSoundSystem.h"
#include "Logger.h"
#include <sstream>

using namespace dae;

void LoggingSoundSystem::Play(const int soundId, const float volume)
{
	m_pRealSoundSystem->Play(soundId, volume);

	std::stringstream ss{};
	ss << "Playing sound with ID " << soundId << " at volume " << volume;

	Logger::Get().Log(ss.str());
}

void LoggingSoundSystem::AddSound(const std::string& fileName, int& soundId)
{
	m_pRealSoundSystem->AddSound(fileName, soundId);

	std::stringstream ss{};
	ss << "Sound added: " << fileName << " (ID: " << soundId << ")";

	Logger::Get().Log(ss.str());
}

void LoggingSoundSystem::Preload(const int soundId)
{
	m_pRealSoundSystem->Preload(soundId);

	std::stringstream ss{};
	ss << "Preloaded sound with ID " << soundId;

	Logger::Get().Log(ss.str());
}
