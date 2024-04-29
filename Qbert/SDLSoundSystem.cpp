#include "SDLSoundSystem.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <map>
#include <cassert>
#include <array>
#include <algorithm>
#include <execution>
#include <mutex>
#include <future>
#include <memory>
#include <condition_variable>

struct SoundMessage
{
	dae::SoundId id;
	float volume;
};

class SDLSoundSystem::SDLSoundSystemImpl final
{
public:
	SDLSoundSystemImpl();
	~SDLSoundSystemImpl();

	SDLSoundSystemImpl(const SDLSoundSystemImpl& other) = delete;
	SDLSoundSystemImpl(SDLSoundSystemImpl&& other) noexcept = delete;
	SDLSoundSystemImpl& operator=(const SDLSoundSystemImpl& other) = delete;
	SDLSoundSystemImpl& operator=(SDLSoundSystemImpl&& other) noexcept = delete;

	void Play(const dae::SoundId id, const float volume = 1.f);
	void ProcessSounds();
private:
	void LoadSound(const std::string& fileName, dae::SoundId soundId);

	struct DeleteSound
	{
		void operator()(Mix_Chunk* pSound) { Mix_FreeChunk(pSound); }
	};
	std::map<dae::SoundId, std::unique_ptr<Mix_Chunk, DeleteSound>> m_vSounds;

	int m_Head{ 0 };
	int m_Tail{ 0 };

	static const int MAX_PENDING{ 16 };
	std::array<SoundMessage, MAX_PENDING> m_Queue{};

	std::jthread m_SoundThread{};
	std::condition_variable m_WaitForNewSound{};
	std::mutex m_Mutex{};

	bool m_StopProcessing{ false };
};

SDLSoundSystem::SDLSoundSystemImpl::SDLSoundSystemImpl()
{
	Mix_Init(MIX_INIT_WAVPACK);
	SDL_Init(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_Mixer couldnt init. Err: %s\n", Mix_GetError());
		return;
	}

	LoadSound("Change Selection.wav",		static_cast<dae::SoundId>(Sounds::ChangeSelection));
	LoadSound("Clear Disks.wav",			static_cast<dae::SoundId>(Sounds::ClearDisks));
	LoadSound("Coily Egg Jump.wav",			static_cast<dae::SoundId>(Sounds::CoilyEggJump));
	LoadSound("Coily Fall.wav",				static_cast<dae::SoundId>(Sounds::CoilyFall));
	LoadSound("Coily Snake Jump.wav",		static_cast<dae::SoundId>(Sounds::CoilySnakeJump));
	LoadSound("Disk Land.wav",				static_cast<dae::SoundId>(Sounds::DiskLand));
	LoadSound("Disk Lift.wav",				static_cast<dae::SoundId>(Sounds::DiskLift));
	LoadSound("Level Screen Tune.wav",		static_cast<dae::SoundId>(Sounds::LevelScreenTune));
	LoadSound("Other Foes Jump.wav",		static_cast<dae::SoundId>(Sounds::OtherFoesJump));
	LoadSound("QBert Fall.wav",				static_cast<dae::SoundId>(Sounds::QbertFall));
	LoadSound("QBert Hit.wav",				static_cast<dae::SoundId>(Sounds::QbertHit));
	LoadSound("QBert Jump.wav",				static_cast<dae::SoundId>(Sounds::QbertJump));
	LoadSound("Round Complete Tune.wav",	static_cast<dae::SoundId>(Sounds::RoundCompleteTune));
	LoadSound("SlickSam Caught.wav",		static_cast<dae::SoundId>(Sounds::SlickSamCaught));
	LoadSound("Swearing.wav",				static_cast<dae::SoundId>(Sounds::Swearing));

	m_SoundThread = std::jthread(&SDLSoundSystem::SDLSoundSystemImpl::ProcessSounds, this);
}

SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
{
	m_StopProcessing = true;
	m_WaitForNewSound.notify_all();
	Mix_Quit();
}

void SDLSoundSystem::SDLSoundSystemImpl::Play(const dae::SoundId id, const float volume)
{
	std::lock_guard lock(m_Mutex);
	assert((m_Tail + 1) % MAX_PENDING != m_Head && m_Tail >= 0);

	for (int i{ m_Head }; i != m_Tail; i = ++i % MAX_PENDING)
	{
		assert(i >= 0 && i < MAX_PENDING);
		if (m_Queue[i].id == id)
		{
			if (volume > m_Queue[i].volume)
				m_Queue[i].volume = volume;
			return;
		}
	}

	m_Queue[m_Tail].id = id;
	m_Queue[m_Tail].volume = volume;
	m_Tail = ++m_Tail % MAX_PENDING;
	m_WaitForNewSound.notify_all();
}

void SDLSoundSystem::SDLSoundSystemImpl::ProcessSounds()
{
	while (true)
	{
		std::unique_lock lock(m_Mutex);
		m_WaitForNewSound.wait(lock, [&]() { return m_Head != m_Tail || m_StopProcessing; });

		if (m_StopProcessing)
			break;

		Mix_Volume(-1, static_cast<int>(roundf(m_Queue[m_Head].volume * MIX_MAX_VOLUME)));
		Mix_PlayChannel(-1, m_vSounds[m_Queue[m_Head].id].get(), 0);

		m_Head = ++m_Head % MAX_PENDING;
	}
}

void SDLSoundSystem::SDLSoundSystemImpl::LoadSound(const std::string& fileName, dae::SoundId soundId)
{
	Mix_Chunk* m = NULL;
	std::string fullPath{ std::string("../Data/Sounds/") + fileName };
	m = Mix_LoadWAV(fullPath.c_str());

	if (m == NULL) 
	{
		printf("Failed to load sound. SDL_Mixer error: %s\n", Mix_GetError());
		return;;
	}
	
	m_vSounds.emplace(soundId, std::unique_ptr<Mix_Chunk, DeleteSound>(m, DeleteSound()));
}

SDLSoundSystem::SDLSoundSystem()
	: m_pSDLSoundSystemImpl{ std::make_unique<SDLSoundSystemImpl>() }
{
}

SDLSoundSystem::~SDLSoundSystem() = default;

void SDLSoundSystem::Play(const dae::SoundId id, const float volume)
{
	m_pSDLSoundSystemImpl->Play(id, volume);
}
