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
		virtual void Play(const Sounds soundId, const float volume = 1.f) = 0;
		virtual void LoadSound(const std::string& fileName, Sounds soundId) = 0;
		virtual void ClearSounds() = 0;
		virtual void ToggleMute() = 0;
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
		virtual void ClearSounds() override
		{
			m_pSoundSystem->ClearSounds();
			std::cout << "Unloading all loaded sounds\n";
		}
		virtual void ToggleMute()
		{ 
			std::cout << "Toggled sound mute\n";
			m_pSoundSystem->ToggleMute();
		}
	private:
		std::unique_ptr<SoundSystem> m_pSoundSystem;
	};

	class NullSoundSystem final : public SoundSystem
	{
		virtual void Play(const Sounds, const float) override {}
		virtual void LoadSound(const std::string&, Sounds) override {}
		virtual void ClearSounds() override {}
		virtual void ToggleMute() {}
	};
}

