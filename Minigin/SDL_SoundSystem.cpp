#include "SDL_SoundSystem.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include "ResourceManager.h"
#include <unordered_map>
#include "LoggingSystem.h"
#include <algorithm>
using namespace dae;

//SDL_mixer tutorial
//https://www.youtube.com/watch?v=o0nyxxWRPnA

class SDL_SoundSystem::SDL_SoundImpl final
{
public:
	SDL_SoundImpl()
	{
        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            std::stringstream ss{};
            ss << "SDL_Init() failed: " << SDL_GetError() << '\n';

            ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
        }

        if (Mix_Init(MIX_INIT_MP3) < 0)
        {
            std::stringstream ss{};
            ss << "Mix_Init() failed: " << Mix_GetError() << '\n';

            ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
        }

        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
        {
            std::stringstream ss{};
            std::cout << "Mix_OpenAudio() failed: " << Mix_GetError() << '\n';

            ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
        }
	}	

    ~SDL_SoundImpl()
    {
        //Free chunks
        for (auto& sound : m_Sounds)
        {
            if (sound.second.first)
            {
                Mix_FreeChunk(sound.second.first);
                sound.second.first = nullptr;
            }
        }

        Mix_CloseAudio();
    }

    void Play(const int soundId, const float volume)
    {
        auto& sound{ m_Sounds[soundId] };

        //Load chunk if null
        if (sound.first == nullptr)
        {
            sound.first = Mix_LoadWAV(sound.second.c_str());

            //Log error
            if (sound.first == nullptr)
            {
                std::stringstream ss{};
                ss << "Mix_LoadWAV() failed: " << Mix_GetError();

                ServiceLocator<LoggingSystem>::GetService().Log(ss.str());
                return;
            }
        }

        //Set volume
        const int playVolume{ static_cast<int>(MIX_MAX_VOLUME * std::clamp(volume, 0.f, 1.f)) };
        Mix_Volume(-1, playVolume);

        //Play sound
        Mix_PlayChannel(-1, sound.first, 0);
    }

    void AddSound(const std::string& fileName, int& soundId)
    {
        const std::string path{ ResourceManager::GetInstance().GetDataPath() + "Sounds/" + fileName };

        soundId = static_cast<int>(m_Sounds.size());
        m_Sounds[soundId] = std::make_pair(nullptr, path);
    }

private:
    std::unordered_map<int, std::pair<Mix_Chunk*, std::string>> m_Sounds{};
};

SDL_SoundSystem::SDL_SoundSystem()
	:m_pImpl(new SDL_SoundImpl{})
{
}

dae::SDL_SoundSystem::~SDL_SoundSystem()
{
	delete m_pImpl;
}

void dae::SDL_SoundSystem::Play(const int soundId, const float volume)
{
    m_pImpl->Play(soundId, volume);
}

void dae::SDL_SoundSystem::AddSound(const std::string& fileName, int& soundId)
{
    m_pImpl->AddSound(fileName, soundId);
}


