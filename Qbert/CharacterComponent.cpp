#include "CharacterComponent.h"
#include "GameTime.h"
#include "GameObject.h"
#include "QbertStates.h"
#include "LevelManagerComponent.h"
#include "GraphicsComponent.h"

std::unique_ptr<dae::Subject<Character, MovementInfo>> CharacterComponent::MoveStateChanged{ std::make_unique<dae::Subject<Character, MovementInfo>>() };
std::unique_ptr<dae::Subject<Character>> CharacterComponent::CharacterSpawned{ std::make_unique<dae::Subject<Character>>() };

CharacterComponent::CharacterComponent(dae::GameObject* pGameObject) 
	: BaseComponent(pGameObject)
{
}

CharacterComponent::~CharacterComponent()
{
	MoveStateChanged->RemoveObserver(this);
	CharacterSpawned->RemoveObserver(this);
}

void CharacterComponent::Init()
{
	MoveStateChanged->AddObserver(this);
	CharacterSpawned->AddObserver(this);
}

void CharacterComponent::Update()
{
	m_pState->Update();
}

void CharacterComponent::Notify(Character character)
{
	if (character != m_Character)
		return;

	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(true);
}

void CharacterComponent::SetState(std::unique_ptr<CharacterState>&& pNewState)
{
	if (m_pState)
		m_pState->OnExit();
	m_pState = std::move(pNewState);
	if (m_pState)
		m_pState->OnEnter();
}

void CharacterComponent::Move(MovementInfo movementInfo)
{
	m_pState->HandleInput(movementInfo);
}

glm::vec3 CharacterComponent::GetPosition() const
{
	return GetGameObject()->GetLocalPosition();
}

void CharacterComponent::SetPosition(const glm::vec3& pos)
{
	GetGameObject()->SetPosition(pos);
}
