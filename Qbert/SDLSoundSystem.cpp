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
	void Update();
private:
	void LoadSound(const std::string& fileName, Sounds sound);

	struct DeleteSound
	{
		void operator()(Mix_Chunk* ptr) { Mix_FreeChunk(ptr); }
	};
	std::map<dae::SoundId, std::unique_ptr<Mix_Chunk, DeleteSound>> m_vSounds;

	int m_Head{ 0 };
	int m_Tail{ 0 };

	static const int MAX_PENDING{ 16 };
	std::array<SoundMessage, MAX_PENDING> m_Queue{};

	std::future<void> m_SoundThread;
	std::condition_variable m_WaitForNewSound;
	std::mutex m_Mutex{};
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

	LoadSound("Change Selection.wav",		Sounds::ChangeSelection);
	LoadSound("Clear Disks.wav",			Sounds::ClearDisks);
	LoadSound("Coily Egg Jump.wav",			Sounds::CoilyEggJump);
	LoadSound("Coily Fall.wav",				Sounds::CoilyFall);
	LoadSound("Coily Snake Jump.wav",		Sounds::CoilySnakeJump);
	LoadSound("Disk Land.wav",				Sounds::DiskLand);
	LoadSound("Disk Lift.wav",				Sounds::DiskLift);
	LoadSound("Level Screen Tune.wav",		Sounds::LevelScreenTune);
	LoadSound("Other Foes Jump.wav",		Sounds::OtherFoesJump);
	LoadSound("QBert Fall.wav",				Sounds::QbertFall);
	LoadSound("QBert Hit.wav",				Sounds::QbertHit);
	LoadSound("QBert Jump.wav",				Sounds::QbertJump);
	LoadSound("Round Complete Tune.wav",	Sounds::RoundCompleteTune);
	LoadSound("SlickSam Caught.wav",		Sounds::SlickSamCaught);
	LoadSound("Swearing.wav",				Sounds::Swearing);

	m_SoundThread = std::async(&SDLSoundSystem::SDLSoundSystemImpl::Update, this);
}

SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
{
	Play(dae::SoundId(std::numeric_limits<dae::SoundId>().max()), 0);
	m_SoundThread.get();
	Mix_Quit();
}

void SDLSoundSystem::SDLSoundSystemImpl::Play(const dae::SoundId id, const float volume)
{
	assert((m_Tail + 1) % MAX_PENDING != m_Head);
	assert(m_Tail >= 0);

	for (int i{ m_Head }; i != m_Tail; i = ++i % MAX_PENDING)
	{
		if (m_Queue[i].id == id)
		{
			if (volume > m_Queue[i].volume)
			{
				std::lock_guard lock(m_Mutex);
				m_Queue[i].volume = volume;
			}
			return;
		}
	}

	std::lock_guard lock(m_Mutex);
	m_Queue[m_Tail].id = id;
	m_Queue[m_Tail].volume = volume;
	m_Tail = ++m_Tail % MAX_PENDING;
	m_WaitForNewSound.notify_all();
}

void SDLSoundSystem::SDLSoundSystemImpl::Update()
{
	while (true)
	{
		std::unique_lock lock(m_Mutex);
		if (m_Head == m_Tail)
			m_WaitForNewSound.wait(lock);

		if (m_Queue[m_Head].id == std::numeric_limits<dae::SoundId>().max())
			break;

		Mix_Volume(-1, static_cast<int>(roundf(m_Queue[m_Head].volume * MIX_MAX_VOLUME)));
		Mix_PlayChannel(-1, m_vSounds[m_Queue[m_Head].id].get(), 0);

		m_Head = ++m_Head % MAX_PENDING;
	}
}

void SDLSoundSystem::SDLSoundSystemImpl::LoadSound(const std::string& fileName, Sounds sound)
{
	Mix_Chunk* m = NULL;
	std::string fullPath{ std::string("../Data/Sounds/") + fileName };
	m = Mix_LoadWAV(fullPath.c_str());

	if (m == NULL) 
	{
		printf("Failed to load sound. SDL_Mixer error: %s\n", Mix_GetError());
		return;;
	}

	m_vSounds.emplace(static_cast<dae::SoundId>(sound), std::unique_ptr<Mix_Chunk, DeleteSound>(m, DeleteSound()));
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

void SDLSoundSystem::Update()
{
	m_pSDLSoundSystemImpl->Update();
}
