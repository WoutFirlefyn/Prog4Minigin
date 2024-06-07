#include "UggWrongwayComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "UggWrongwayStates.h"

UggWrongwayComponent::UggWrongwayComponent(dae::GameObject* pGameObject, Character character, LevelManagerComponent* pLevelManagerComponent) 
	: CharacterComponent(pGameObject, pLevelManagerComponent)
{
	if (character != Character::Ugg && character != Character::Wrongway)
	{
		assert(false);
		return;
	}
	m_Character = character;
}

UggWrongwayComponent::~UggWrongwayComponent()
{
	if (MoveStateChanged.get())
		MoveStateChanged->RemoveObserver(this);
}

void UggWrongwayComponent::Init()
{
	m_SpawnedLeft = rand() % 2;
	m_vSpawnPositions.push_back({ 7,0 });
	m_vSpawnPositions.push_back({ 0,7 });

	MoveStateChanged->AddObserver(this);
	SetState(std::make_unique<UggWrongwayResetState>(this));
}

void UggWrongwayComponent::LateUpdate()
{
	if (m_ResetCharacter)
	{
		m_ResetCharacter = false;
		SetState(std::make_unique<UggWrongwayResetState>(this), false);
	}
}

// MoveStateChanged
void UggWrongwayComponent::Notify(Character character, MovementInfo movementInfo)
{
	if ((character == Character::Qbert1 || character == Character::Qbert2) && movementInfo.state == MovementState::Fall)
		m_ResetCharacter = true;

	if (character != m_Character)
		return;

	switch (movementInfo.state)
	{
	case MovementState::End:
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::OtherFoesJump, 0.2f);	
		break;
	case MovementState::Fall:
		m_SpawnedLeft = rand() % 2;
		break;
	default:
		break;
	}
}

glm::ivec2 UggWrongwayComponent::GetSpawnPosition() const
{
	return m_vSpawnPositions[m_SpawnedLeft];
}
