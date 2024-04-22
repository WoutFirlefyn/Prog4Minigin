#pragma once
#include <iostream>
namespace dae
{
	using soundId = unsigned short;
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const soundId id, const float volume) = 0;
	};

	class LoggingSoundSystem final : public SoundSystem 
	{
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : m_pSoundSystem(std::move(ss)) {}
		virtual ~LoggingSoundSystem() = default;
		virtual void Play(const soundId id, const float volume) override 
		{
			m_pSoundSystem->Play(id, volume);
			std::cout << "Playing ID: " << id << " at volume " << volume << std::endl;
		}
	private:
		std::unique_ptr<SoundSystem> m_pSoundSystem;
	};

}

