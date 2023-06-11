#include "LoggingSoundSystem.h"
#include "Logger.h"
#include <sstream>

using namespace dae;

void LoggingSoundSystem::Play(const int soundId, const float volume, const int nrLoops)
{
	m_pRealSoundSystem->Play(soundId, volume, nrLoops);

	std::stringstream ss{};
	ss << "Playing sound with ID " << soundId << " at volume " << volume << " (Nr Loops: " << nrLoops << ")";

	Logger::Get().Log(ss.str());
}

void LoggingSoundSystem::AddSound(const std::string& fileName, int& soundId, bool isMusic)
{
	m_pRealSoundSystem->AddSound(fileName, soundId, isMusic);

	std::stringstream ss{};
	ss << (isMusic ? "Music" : "Sound") << " added: " << fileName << " (ID: " << soundId << ")";

	Logger::Get().Log(ss.str());
}

void dae::LoggingSoundSystem::ToggleMute()
{
	m_pRealSoundSystem->ToggleMute();
	Logger::Get().Log("Mute value changed");
}

void LoggingSoundSystem::Preload(const int soundId)
{
	m_pRealSoundSystem->Preload(soundId);

	std::stringstream ss{};
	ss << "Preloaded sound with ID " << soundId;

	Logger::Get().Log(ss.str());
}
