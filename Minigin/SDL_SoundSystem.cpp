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

        //The sounds are RAII objects
        m_Sounds.clear();

        Mix_CloseAudio();
        Mix_Quit();
    }

    void PlaySound(const int soundId, const float volume, const int nrLoops)
    {
        //Add sound to the queue
        {
            const std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
            m_EventQueue.push(SoundEvent{ SoundEventType::Play, soundId, (m_IsMuted ? 0.f : volume), nrLoops});
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

    void AddSound(const std::string& fileName, int& soundId, bool isMusic)
    {
        const std::string path{ ResourceManager::GetInstance().GetDataPath() + "Sounds/" + fileName };

        soundId = m_NextId;
        ++m_NextId;

        //Add sound to the queue
        {
            const std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
            m_EventQueue.push(SoundEvent{ (isMusic ? SoundEventType::AddMusic : SoundEventType::Add), soundId, 0.f, 0, path });
        }

        m_SoundQueueCondition.notify_one();
    }

    void MuteSound()
    {
        m_IsMuted = !m_IsMuted;

        //Add sound to the queue
        {
            const std::lock_guard<std::mutex> lock(m_SoundQueueMutex);
            m_EventQueue.push(SoundEvent{ SoundEventType::Mute, -1, (m_IsMuted ? 0 : 0.8f) });
        }

        m_SoundQueueCondition.notify_one();
    }

private:
    enum class SoundEventType
    {
        Preload, Play, Add, AddMusic, Mute, UnMute
    };

    struct SoundEvent
    {
        SoundEventType type;
        int id;
        float volume;
        int nrLoops;
        std::string path;
    };

    void Stop()
    {
        m_Quit = true;
        m_SoundQueueCondition.notify_one();
    }

    void Play(const int soundId, const float volume, const int nrLoops)
    {
        auto& sound{ m_Sounds[soundId] };

        //Music
        if (sound.isMusic)
        {
            //Load chunk if null
            if (sound.audio == nullptr)
            {
                sound.audio = Mix_LoadMUS(sound.path.c_str());

                //Return if loading failed
                if (sound.audio == nullptr)
                {
                    return;
                }
            }

            //Set volume
            const int playVolume{ static_cast<int>(MIX_MAX_VOLUME * std::clamp(volume, 0.f, 1.f)) };
            Mix_VolumeMusic(playVolume);

            //Play sound
            Mix_PlayMusic(static_cast<Mix_Music*>(sound.audio), nrLoops);
        }
        //Sound effect
        else
        {
            //Load chunk if null
            if (sound.audio == nullptr)
            {
                sound.audio = Mix_LoadWAV(sound.path.c_str());

                //Return if loading failed
                if (sound.audio == nullptr)
                {
                    return;
                }
            }

            //Set volume
            const int playVolume{ static_cast<int>(MIX_MAX_VOLUME * std::clamp(volume, 0.f, 1.f)) };
            Mix_Volume(-1, playVolume);

            //Play sound
            Mix_PlayChannel(-1, static_cast<Mix_Chunk*>(sound.audio), nrLoops);
        }
    }

    void Mute(const float volume)
    {
        const int playVolume{ static_cast<int>(MIX_MAX_VOLUME * std::clamp(volume, 0.f, 1.f)) };
        Mix_Volume(-1, playVolume);
        Mix_VolumeMusic(playVolume);
    }

    void Preload(const int soundId)
    {
        auto& sound{ m_Sounds[soundId] };

        //Load chunk if null
        if (sound.audio == nullptr)
        {
            if (sound.isMusic)
            {
                sound.audio = Mix_LoadMUS(sound.path.c_str());
            }
            else
            {
                sound.audio = Mix_LoadWAV(sound.path.c_str());
            }
              
            //Return if loading failed
            if (sound.audio == nullptr)
            {
                return;
            }
        }
    }

    void Add(const int soundId, const std::string& path, bool isMusic)
    {
        m_Sounds[soundId] = Sound(nullptr, path, isMusic);
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
                    Play(e.id, e.volume, e.nrLoops);
                break;
                case SoundEventType::Preload:
                    Preload(e.id);           
                break;
                case SoundEventType::Add:
                    Add(e.id, e.path, false);
                    break;
                case SoundEventType::AddMusic:
                    Add(e.id, e.path, true);
                    break;
                case SoundEventType::Mute:
                    Mute(e.volume);
                default:
                    break;
            }
        }
    }

    struct Sound
    {
        Sound(void* pAudio, const std::string& filePath, bool isMusicFile)
            :audio{ pAudio }, path{ filePath }, isMusic{ isMusicFile } {};

        Sound() :audio{}, path{}, isMusic{} {};

        ~Sound()
        {
            if (audio)
            {
                (isMusic ? Mix_FreeMusic(static_cast<Mix_Music*>(audio)) : Mix_FreeChunk(static_cast<Mix_Chunk*>(audio)));
            }
        }

        std::string path;
        void* audio;
        bool isMusic;
    };

    std::unordered_map<int, Sound> m_Sounds{};

    std::atomic<bool> m_Quit{ false };
    std::jthread m_SoundThread;
    std::mutex m_SoundQueueMutex;
    std::condition_variable m_SoundQueueCondition;
    std::queue<SoundEvent> m_EventQueue;


    //Main thread
    int m_NextId{};
    bool m_IsMuted{};
};

SDL_SoundSystem::SDL_SoundSystem()
	:m_pImpl(new SDL_SoundImpl{})
{
}

dae::SDL_SoundSystem::~SDL_SoundSystem()
{
	delete m_pImpl;
}

void dae::SDL_SoundSystem::Play(const int soundId, const float volume, const int nrLoops)
{
    m_pImpl->PlaySound(soundId, volume, nrLoops);
}

void dae::SDL_SoundSystem::Preload(const int soundId)
{
    m_pImpl->PreloadSound(soundId);
}

void dae::SDL_SoundSystem::AddSound(const std::string& fileName, int& soundId, bool isMusic)
{
    m_pImpl->AddSound(fileName, soundId, isMusic);
}

void dae::SDL_SoundSystem::ToggleMute()
{
    m_pImpl->MuteSound();
}