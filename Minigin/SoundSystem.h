#pragma once
#include <iostream>
namespace dae
{
	using SoundId = unsigned short;
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const SoundId id, const float volume = 1.f) = 0;
		virtual void Update() = 0;
	};

	class LoggingSoundSystem final : public SoundSystem 
	{
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : m_pSoundSystem(std::move(ss)) {}
		virtual ~LoggingSoundSystem() = default;
		virtual void Play(const SoundId id, const float volume) override 
		{
			m_pSoundSystem->Play(id, volume);
			std::cout << "Playing ID: " << id << " at volume " << volume << std::endl;
		}
		virtual void Update() {}
	private:
		std::unique_ptr<SoundSystem> m_pSoundSystem;
	};

}

