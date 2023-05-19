#include "SDL_SoundSystem.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include "ResourceManager.h"
#include <unordered_map>
#include "Logger.h"
#include <algorithm>

//SoundQueue
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>

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

            Logger::Get().Log(ss.str());
        }

        if (Mix_Init(MIX_INIT_MP3) < 0)
        {
            std::stringstream ss{};
            ss << "Mix_Init() failed: " << Mix_GetError() << '\n';

            Logger::Get().Log(ss.str());
        }

        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
        {
            std::stringstream ss{};
            std::cout << "Mix_OpenAudio() failed: " << Mix_GetError() << '\n';

            Logger::Get().Log(ss.str());
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
        {
            const std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
            m_EventQueue.push(SoundEvent{ SoundEventType::Play, soundId, volume });
        }

        m_SoundQueueCondition.notify_one();
    }

    void PreloadSound(const int soundId)
    {
        //Add sound to the queue
        {
            const std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
            m_EventQueue.push(SoundEvent{ SoundEventType::Preload, soundId, 0.f });
        }

        m_SoundQueueCondition.notify_one();
    }

    void AddSound(const std::string& fileName, int& soundId)
    {
        const std::string path{ ResourceManager::GetInstance().GetDataPath() + "Sounds/" + fileName };

        soundId = m_NextId;
        ++m_NextId;

        //Add sound to the queue
        {
            const std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
            m_EventQueue.push(SoundEvent{ SoundEventType::Add, soundId, 0.f, path });
        }

        m_SoundQueueCondition.notify_one();
    }

private:
    enum class SoundEventType
    {
        Preload, Play, Add
    };

    struct SoundEvent
    {
        SoundEventType type;
        int id;
        float volume;
        std::string path;
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

    void Add(const int soundId, const std::string& path)
    {
        m_Sounds[soundId] = std::make_pair(nullptr, path);
    }

    void SoundThreadFunction()
    {
        while (!m_Quit)
        {
            std::unique_lock<std::mutex> lock(m_SoundQueueMutex);
            m_SoundQueueCondition.wait(lock, [this] { return !m_EventQueue.empty() || m_Quit; });

            if (m_Quit) return;

            const auto e{ m_EventQueue.front() };
            m_EventQueue.pop();

            lock.unlock();

            //Event type
            switch (e.type)
            {
                case SoundEventType::Play:
                    Play(e.id, e.volume);
                break;
                case SoundEventType::Preload:
                    Preload(e.id);           
                break;
                case SoundEventType::Add:
                    Add(e.id, e.path);
                    break;
                default:
                    break;
            }
        }
    }

    std::unordered_map<int, std::pair<Mix_Chunk*, std::string>> m_Sounds{};

    std::atomic<bool> m_Quit{ false };
    std::jthread m_SoundThread;
    std::mutex m_SoundQueueMutex;
    std::condition_variable m_SoundQueueCondition;
    std::queue<SoundEvent> m_EventQueue;

    int m_NextId{};
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


