#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <iostream>
#include "CharacterComponent.h"

class CharacterState
{
public:
	virtual ~CharacterState() = default;
	virtual void HandleInput(MovementInfo) {}
	virtual void Update() {}
	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	CharacterState(CharacterComponent* pCharacter) : m_pCharacter{ pCharacter } {}
	CharacterComponent* m_pCharacter{ nullptr };

	void SetState(std::unique_ptr<CharacterState>&& pNewState);
	dae::GameObject* GetGameObject() const;
	LevelManagerComponent* GetLevelManagerComponent() const;
};

class SpawnState : public CharacterState
{
public:
	SpawnState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual void OnEnter() override;
protected:
	bool Spawn();

	glm::vec3 m_StartPos{};
	glm::vec3 m_TargetPos{};
	float m_HeightOffset{ 400.f };
	float m_FallDuration{ 2.f };
	float m_FallLerpValue{ 0.f };
};

class IdleState : public CharacterState, dae::Observer<Character, Character>
{
public:
	IdleState(CharacterComponent* pCharacter);
	virtual ~IdleState() override;
	virtual void Notify(Character, Character) override {}
	virtual void SubjectDestroyed(dae::Subject<Character, Character>* pSubject) override;
private:
	dae::Subject<Character, Character>* m_pCharactersCollide{ nullptr };
};

class JumpState : public CharacterState
{
public:
	JumpState(CharacterComponent* pCharacter, MovementInfo movementInfo);
	virtual void OnEnter() override;
	virtual void OnExit() override;
protected:
	bool Jump();
	float m_JumpDuration{ 0.4f };
	float m_JumpLerpValue{ 0.f };
	glm::vec3 m_StartPos{};
	MovementInfo m_MovementInfo;
	TileType m_NextTileType{};
};

class DeathState : public CharacterState
{
public:
	DeathState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
};

class ResetState : public CharacterState
{
public:
	ResetState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
protected:
	bool Wait();
private:
	float m_AccumSec{};
	float m_PauseDuration{ 2.5f };
};

