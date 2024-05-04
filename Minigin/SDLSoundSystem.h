#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SDLSoundSystem final : public dae::SoundSystem
	{
	public:
		SDLSoundSystem();
		virtual ~SDLSoundSystem() override;
		virtual void Play(const dae::Sounds soundId, const float volume) override;
		virtual void LoadSound(const std::string& fileName, dae::Sounds soundId) override;

	private:
		class SDLSoundSystemImpl;
		std::unique_ptr<SDLSoundSystemImpl> m_pSDLSoundSystemImpl;
	};
}

