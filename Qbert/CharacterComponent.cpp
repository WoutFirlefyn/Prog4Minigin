#include "CharacterComponent.h"
#include "GraphicsComponent.h"
#include "LevelManagerComponent.h"
#include "CharacterStates.h"
#include "GameObject.h"

std::unique_ptr<dae::Subject<Character, MovementInfo>> CharacterComponent::MoveStateChanged{ std::make_unique<dae::Subject<Character, MovementInfo>>() };
std::unique_ptr<dae::Subject<Character, dae::GameObject*>> CharacterComponent::CharacterSpawned{ std::make_unique<dae::Subject<Character, dae::GameObject*>>() };

CharacterComponent::CharacterComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pLevelManagerComponent{ pLevelManagerComponent }
{
}

void CharacterComponent::Update()
{
	m_pState->Update();
}

void CharacterComponent::Move(MovementInfo movementInfo)
{
	m_pState->HandleInput(movementInfo);
} 

void CharacterComponent::SetState(std::unique_ptr<CharacterState>&& pNewState)
{
	if (m_pState)
		m_pState->OnExit();
	m_pState = std::move(pNewState);
	if (m_pState)
		m_pState->OnEnter();
}
