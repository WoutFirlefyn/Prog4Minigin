#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SDLSoundSystem final : public dae::SoundSystem
	{
	public:
		SDLSoundSystem();
		virtual ~SDLSoundSystem() override;
		virtual void Play(const dae::SoundId id, const float volume) override;
		virtual void LoadSound(const std::string& fileName, dae::SoundId soundId) override;

	private:
		class SDLSoundSystemImpl;
		std::unique_ptr<SDLSoundSystemImpl> m_pSDLSoundSystemImpl;
	};
}

