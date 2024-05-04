#pragma once
#include <iostream>
namespace dae
{
	using SoundId = unsigned short;
	enum class Sounds : SoundId;
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const Sounds id, const float volume = 1.f) = 0;
		virtual void LoadSound(const std::string& fileName, Sounds soundId) = 0;
	};

	class LoggingSoundSystem final : public SoundSystem 
	{
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : m_pSoundSystem(std::move(ss)) {}
		virtual ~LoggingSoundSystem() = default;
		virtual void Play(const Sounds id, const float volume) override
		{
			m_pSoundSystem->Play(id, volume);
			std::cout << "Playing ID: " << static_cast<SoundId>(id) << " at volume " << volume << std::endl;
		}
		virtual void LoadSound(const std::string& fileName, Sounds soundId) override
		{
			m_pSoundSystem->LoadSound(fileName, soundId);
			std::cout << "Loading " << fileName << " with ID: " << static_cast<SoundId>(soundId) << std::endl;
		}
	private:
		std::unique_ptr<SoundSystem> m_pSoundSystem;
	};

	class NullSoundSystem final : public SoundSystem
	{
		virtual void Play(const Sounds, const float) override {}
		virtual void LoadSound(const std::string&, Sounds) override {};
	};
}

