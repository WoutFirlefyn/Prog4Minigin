#pragma once
#include "CharacterStates.h"

class QbertIdleState : public IdleState
{
public:
	QbertIdleState(CharacterComponent* pCharacter) : IdleState(pCharacter) {}
	virtual void HandleInput(MovementInfo) override;
	virtual void Notify(Character, Character) override;
};

class QbertJumpState : public JumpState
{
public:
	QbertJumpState(CharacterComponent* pCharacter, MovementInfo movementInfo) : JumpState(pCharacter, movementInfo) {}
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class QbertDeathState : public DeathState
{
public:
	QbertDeathState(CharacterComponent* pCharacter, const glm::vec3& startPos = glm::vec3(0))
		: DeathState(pCharacter) 
		, m_StartPos{ startPos }
	{
	}
	virtual void Update() override;
	virtual void OnExit() override;
private:
	glm::vec3 m_StartPos{};
	float m_AccumSec{ 0.f };
	float m_RespawnDelay{ 2.5f };
};

struct Disk;
class QbertDiskState : public CharacterState, public dae::Observer<Disk, Character>
{
public:
	QbertDiskState(CharacterComponent* pCharacter);
	virtual ~QbertDiskState() override;
	virtual void Update() override;
	virtual void OnExit() override;
	virtual void Notify(Disk disk, Character character) override;
	virtual void SubjectDestroyed(dae::Subject<Disk, Character>* pSubject) override;
private:
	dae::Subject<Disk, Character>* m_pDiskStateChangedSubject{ nullptr };

	bool m_DiskReachedTop{ false };
	glm::vec3 m_StartPos{};
	glm::vec3 m_TargetPos{};
	float m_FallLerpValue{};
	float m_FallDuration{ 0.5f };
};

class QbertSpawnState : public SpawnState
{
public:
	QbertSpawnState(CharacterComponent* pCharacter) : SpawnState(pCharacter) {}
	virtual void Update() override;
	virtual void OnEnter() override;
};

class QbertResetState : public ResetState
{
public:
	QbertResetState(CharacterComponent* pCharacter) : ResetState(pCharacter) {}
	virtual void Update() override;
};