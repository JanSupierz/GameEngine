#include "SDL_SoundSystem.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include "ResourceManager.h"
#include <unordered_map>
#include "LoggingSystem.h"
#include <algorithm>

//SoundQueue
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <tuple>

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

        //Start a sound thread
        m_SoundThread = std::jthread(&SDL_SoundImpl::SoundThreadFunction, this);
	}	

    ~SDL_SoundImpl()
    {
        Stop();

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

    void PlaySound(const int soundId, const float volume)
    {
        //Add sound to the queue
        std::unique_lock<std::mutex> lock(m_SoundQueueMutex);

        m_SoundQueue.push(std::make_tuple(SoundEvent::Play, soundId, volume));
        lock.unlock();
        m_SoundQueueCondition.notify_one();
    }

    void PreloadSound(const int soundId)
    {
        //Add sound to the queue
        std::unique_lock<std::mutex> lock(m_SoundQueueMutex);

        m_SoundQueue.push(std::make_tuple(SoundEvent::Preload, soundId, 0.f));
        lock.unlock();
        m_SoundQueueCondition.notify_one();
    }

    void AddSound(const std::string& fileName, int& soundId)
    {
        const std::string path{ ResourceManager::GetInstance().GetDataPath() + "Sounds/" + fileName };

        //Add sounds to the map
        std::lock_guard<std::mutex> lock(m_SoundQueueMutex);

        soundId = static_cast<int>(m_Sounds.size());
        m_Sounds[soundId] = std::make_pair(nullptr, path);
    }

private:
    enum class SoundEvent
    {
        Preload, Play
    };

    void Stop()
    {
        m_Quit = true;
        m_SoundQueueCondition.notify_one();
    }

    void Play(const int soundId, const float volume)
    {
        auto& sound{ m_Sounds[soundId] };

        //Load chunk if null
        if (sound.first == nullptr)
        {
            sound.first = Mix_LoadWAV(sound.second.c_str());

            //Return if loading failed
            if (sound.first == nullptr)
            {
                return;
            }
        }

        //Set volume
        const int playVolume{ static_cast<int>(MIX_MAX_VOLUME * std::clamp(volume, 0.f, 1.f)) };
        Mix_Volume(-1, playVolume);

        //Play sound
        Mix_PlayChannel(-1, sound.first, 0);
    }

    void Preload(const int soundId)
    {
        auto& sound{ m_Sounds[soundId] };

        //Load chunk if null
        if (sound.first == nullptr)
        {
            sound.first = Mix_LoadWAV(sound.second.c_str());

            //Return if loading failed
            if (sound.first == nullptr)
            {
                return;
            }
        }
    }

    void SoundThreadFunction()
    {
        while (!m_Quit)
        {
            std::unique_lock<std::mutex> lock(m_SoundQueueMutex);
            m_SoundQueueCondition.wait(lock, [this] { return !m_SoundQueue.empty() || m_Quit; });

            while (!m_SoundQueue.empty())
            {
                const auto& sound{ m_SoundQueue.front() };

                //Event type
                switch (std::get<0>(sound))
                {
                    case SoundEvent::Play:
                    {
                        //Sound Id and Volume
                        Play(std::get<1>(sound), std::get<2>(sound));
                    }
                    break;
                    case SoundEvent::Preload:
                    {
                        //Sound Id
                        Preload(std::get<1>(sound));
                    }
                    break;
                    default:
                        break;
                }

                m_SoundQueue.pop();
            }
        }
    }

    std::unordered_map<int, std::pair<Mix_Chunk*, std::string>> m_Sounds{};

    std::atomic<bool> m_Quit{ false };
    std::jthread m_SoundThread;
    std::mutex m_SoundQueueMutex;
    std::condition_variable m_SoundQueueCondition;
    std::queue<std::tuple<SoundEvent, int, float>> m_SoundQueue;
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
    m_pImpl->PlaySound(soundId, volume);
}

void dae::SDL_SoundSystem::Preload(const int soundId)
{
    m_pImpl->PreloadSound(soundId);
}

void dae::SDL_SoundSystem::AddSound(const std::string& fileName, int& soundId)
{
    m_pImpl->AddSound(fileName, soundId);
}


