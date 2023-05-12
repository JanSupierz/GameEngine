#pragma once
#include "ServiceLocator.h"
#include <string>

class SoundSystem
{
public:
	virtual ~SoundSystem() = default;
	virtual void Play(const int soundId, const float volume) = 0;
	virtual void AddSound(const std::string& fileName, int& soundId) = 0;
};

template <class Service>
class NullService;

template <>
class NullService<SoundSystem> : public SoundSystem {
public:
	virtual void Play(const int, const float) {};
	virtual void AddSound(const std::string&, int& soundId) { soundId = -1; };
};