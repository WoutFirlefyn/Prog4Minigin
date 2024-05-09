#include "CharacterComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "QbertStates.h"
#include <iostream>

CharacterComponent::CharacterComponent(dae::GameObject* pGameObject) 
	: BaseComponent(pGameObject)
{
	MoveStateChanged = std::make_unique<dae::Subject<Character, MovementState, MovementDirection>>();
}

void CharacterComponent::Init()
{
	MoveStateChanged->AddObserver(this);
}

void CharacterComponent::Update()
{
	if (auto pNewState = m_pState->Update())
	{
		m_pState->OnExit();
		m_pState = std::move(pNewState);
		m_pState->OnEnter();
	}
}

void CharacterComponent::Notify(Character character)
{
	if (character == m_Character)
		m_IsGoingToFall = true;
}

void CharacterComponent::SubjectDestroyed(dae::Subject<Character>* pSubject)
{
	if (pSubject == m_pCharacterGoingToFallSubject)
		m_pCharacterGoingToFallSubject = nullptr;
}

void CharacterComponent::Move(MovementDirection movementDirection)
{
	if (auto pNewState = m_pState->HandleInput(movementDirection))
	{
		m_pState->OnExit();
		m_pState = std::move(pNewState);
		m_pState->OnEnter();
	}
}

glm::vec3 CharacterComponent::GetPosition() const
{
	return GetGameObject()->GetLocalPosition();
}

void CharacterComponent::SetPosition(const glm::vec3& pos)
{
	GetGameObject()->SetPosition(pos);
}
