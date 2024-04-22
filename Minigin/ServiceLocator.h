#pragma once
#include <memory>
#include "SoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public SoundSystem
	{
		virtual void Play(const soundId, const float) override {}
	};

	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem() { return *m_pSoundSystemInstance; }
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& pSoundSystem) { m_pSoundSystemInstance = std::move(pSoundSystem); }
	private:
		static std::unique_ptr<SoundSystem> m_pSoundSystemInstance;
	};
}
