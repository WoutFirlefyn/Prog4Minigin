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
	m_pTileChangedSubject = m_pLevelManagerComponent->TileChanged.get();
	m_pTileChangedSubject->AddObserver(this);
}

CharacterComponent::~CharacterComponent()
{
	if(m_pTileChangedSubject)
		m_pTileChangedSubject->RemoveObserver(this);
}

void CharacterComponent::Update()
{
	m_pState->Update();
}

void CharacterComponent::Notify(Character, bool roundFinished)
{
	if (roundFinished)
		m_ResetCharacter = true;
}

void CharacterComponent::SubjectDestroyed(dae::Subject<Character, bool>* pSubject)
{
	if (pSubject == m_pTileChangedSubject)
		m_pTileChangedSubject = nullptr;
}

void CharacterComponent::Move(MovementInfo movementInfo)
{
	m_pState->HandleInput(movementInfo);
} 

void CharacterComponent::SetState(std::unique_ptr<CharacterState>&& pNewState, bool callOnExit)
{
	if (m_pState && callOnExit)
		m_pState->OnExit();
	m_pState = std::move(pNewState);
	if (m_pState)
		m_pState->OnEnter();
}
