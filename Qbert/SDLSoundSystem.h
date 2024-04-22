#pragma once
#include "SoundSystem.h"

class SDLSoundSystem : public dae::SoundSystem
{
public:
	SDLSoundSystem();
	virtual ~SDLSoundSystem() = default;
	virtual void Play(const dae::soundId id, const float volume) override;

private:
	class SDLSoundSystemImpl;
	std::unique_ptr<SDLSoundSystemImpl> m_pSDLSoundSystemImpl;
};

