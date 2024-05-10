#include "CharacterComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "QbertStates.h"
#include "LevelManagerComponent.h"

std::unique_ptr<dae::Subject<Character, MovementState, MovementDirection>> CharacterComponent::MoveStateChanged{ std::make_unique<dae::Subject<Character, MovementState, MovementDirection>>() };

CharacterComponent::CharacterComponent(dae::GameObject* pGameObject) 
	: BaseComponent(pGameObject)
{
}

CharacterComponent::~CharacterComponent()
{
	MoveStateChanged->RemoveObserver(this);
	m_pCharacterStartedJumping->RemoveObserver(this);
}

void CharacterComponent::Init()
{
	m_pCharacterStartedJumping = LevelManagerComponent::CharacterStartedJumping.get();
	m_pCharacterStartedJumping->AddObserver(this);
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

void CharacterComponent::Notify(Character character, TileType tileType)
{
	if (character == m_Character)
		m_NextTileType = tileType;
}

void CharacterComponent::SubjectDestroyed(dae::Subject<Character, TileType>* pSubject)
{
	if (pSubject == m_pCharacterStartedJumping)
		m_pCharacterStartedJumping = nullptr;
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
