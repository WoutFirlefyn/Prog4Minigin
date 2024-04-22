#include "SDLSoundSystem.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <vector>

class SDLSoundSystem::SDLSoundSystemImpl
{
public:
	SDLSoundSystemImpl();
	~SDLSoundSystemImpl();

	SDLSoundSystemImpl(const SDLSoundSystemImpl& other) = delete;
	SDLSoundSystemImpl(SDLSoundSystemImpl&& other) noexcept = delete;
	SDLSoundSystemImpl& operator=(const SDLSoundSystemImpl& other) = delete;
	SDLSoundSystemImpl& operator=(SDLSoundSystemImpl&& other) noexcept = delete;

	void Play(const dae::soundId id, const float volume);
private:
	int LoadMusic(const std::string& fileName);
	int LoadSound(const std::string& fileName);
	std::vector<std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>> m_vMusic;
	std::vector<std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>> m_vSounds;
};

SDLSoundSystem::SDLSoundSystemImpl::SDLSoundSystemImpl()
{
	Mix_Init(MIX_INIT_WAVPACK);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
	{
		printf("SDL_Mixer couldnt init. Err: %s\n", Mix_GetError());
		return;
	}

	LoadSound("Swearing.wav");
}

SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
{
	Mix_Quit();
}

void SDLSoundSystem::SDLSoundSystemImpl::Play(const dae::soundId id, const float)
{
	Mix_Volume(-1, 10);
	Mix_PlayChannel(-1, m_vSounds[id].get(), 0);
}

int SDLSoundSystem::SDLSoundSystemImpl::LoadMusic(const std::string& fileName)
{
	Mix_Music* m = NULL;
	m = Mix_LoadMUS(fileName.c_str());
	if (m == NULL) {
		printf("Failed to load music. SDL_Mixer error: %s\n", Mix_GetError());
		return -1;
	}
	m_vMusic.push_back(std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>(m, Mix_FreeMusic));
	return static_cast<int>(m_vMusic.size() - 1);
}

int SDLSoundSystem::SDLSoundSystemImpl::LoadSound(const std::string& fileName)
{
	Mix_Chunk* m = NULL;
	std::string fullPath{ std::string("../Data/Sounds/") + fileName };
	m = Mix_LoadWAV(fullPath.c_str());
	if (m == NULL) {
		printf("Failed to load sound. SDL_Mixer error: %s\n", Mix_GetError());
		return -1;
	}
	m_vSounds.push_back(std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>(m, Mix_FreeChunk));
	return static_cast<int>(m_vSounds.size() - 1);
}

SDLSoundSystem::SDLSoundSystem()
	: m_pSDLSoundSystemImpl{ std::make_unique<SDLSoundSystemImpl>() }
{
}

void SDLSoundSystem::Play(const dae::soundId id, const float volume)
{
	m_pSDLSoundSystemImpl->Play(id, volume);
}
