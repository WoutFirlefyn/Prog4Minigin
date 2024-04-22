#pragma once
#include "SoundSystem.h"

enum class Sounds : dae::SoundId
{
	ChangeSelection,
	ClearDisks,
	CoilyEggJump,
	CoilyFall,
	CoilySnakeJump,
	DiskLand,
	DiskLift,
	LevelScreenTune,
	OtherFoesJump,
	QbertFall,
	QbertHit,
	QbertJump,
	RoundCompleteTune,
	SlickSamCaught,
	Swearing,

	MaxAmountOfSounds
};

class SDLSoundSystem final : public dae::SoundSystem
{
public:
	SDLSoundSystem();
	virtual ~SDLSoundSystem() override;
	virtual void Play(const dae::SoundId id, const float volume) override;
	virtual void Update() override;

private:
	class SDLSoundSystemImpl;
	std::unique_ptr<SDLSoundSystemImpl> m_pSDLSoundSystemImpl;
};

