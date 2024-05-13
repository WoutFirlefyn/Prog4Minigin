#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <iostream>
#include "CharacterComponent.h"

class CharacterState
{
public:
	virtual ~CharacterState() = default;
	virtual void HandleInput(MovementInfo) {};
	virtual void Update() {}
	virtual void OnEnter() {}
	virtual void OnExit() {}
protected:
	CharacterState(CharacterComponent* pCharacter);
	CharacterComponent* m_pCharacter{ nullptr };
};

class SpawnState : public CharacterState
{
public:
	SpawnState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class IdleState : public CharacterState, dae::Observer<Character, Character>
{
public:
	IdleState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual ~IdleState() override;
	virtual void OnEnter() override;
	virtual void SubjectDestroyed(dae::Subject<Character, Character>* pSubject) override;
private:
	dae::Subject<Character, Character>* m_pCharactersCollide{ nullptr };
};

class JumpState : public CharacterState, dae::Observer<Character, TileType>
{
public:
	JumpState(CharacterComponent* pCharacter, MovementInfo movementInfo);
	virtual ~JumpState() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Notify(Character character, TileType tileType) override;
	virtual void SubjectDestroyed(dae::Subject<Character, TileType>* pSubject) override;
protected:
	bool Jump();
	float m_JumpDuration{ 0.4f };
	float m_JumpLerpValue{ 0.f };
	glm::vec3 m_StartPos{};
	MovementInfo m_MovementInfo;
	TileType m_NextTileType{};
private:
	dae::Subject<Character, TileType>* m_pCharacterStartedJumping{ nullptr };
};

class DeathState : public CharacterState
{
public:
	DeathState(CharacterComponent* pCharacter) : CharacterState(pCharacter) {}
	virtual void OnEnter() override { std::cout << "Died\n"; }
};

