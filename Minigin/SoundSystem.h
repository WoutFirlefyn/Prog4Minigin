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
		virtual void LoadSound(const std::string& fileName, SoundId soundId) = 0;
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
		virtual void LoadSound(const std::string& fileName, SoundId soundId) override
		{
			m_pSoundSystem->LoadSound(fileName, soundId);
			std::cout << "Loading " << fileName << " with ID: " << soundId << std::endl;
		}
	private:
		std::unique_ptr<SoundSystem> m_pSoundSystem;
	};

	class NullSoundSystem final : public SoundSystem
	{
		virtual void Play(const SoundId, const float) override {}
		virtual void LoadSound(const std::string&, SoundId) override {};
	};
}

