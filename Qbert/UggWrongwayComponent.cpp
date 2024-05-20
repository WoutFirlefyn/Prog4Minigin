#include "UggWrongwayComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "UggWrongwayStates.h"

UggWrongwayComponent::UggWrongwayComponent(dae::GameObject* pGameObject, Character character) : CharacterComponent(pGameObject)
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
	m_vSpawnPositions.push_back({ 6,0 });
	m_vSpawnPositions.push_back({ 0,6 });

	MoveStateChanged->AddObserver(this);
	SetState(std::make_unique<UggWrongwaySpawnState>(this));
}

// MoveStateChanged
void UggWrongwayComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character != m_Character)
		return;

	if (movementInfo.state == MovementState::End)
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::OtherFoesJump, 0.2f);
}

std::pair<int, int> UggWrongwayComponent::GetSpawnPosition() const
{
	return m_vSpawnPositions[m_SpawnedLeft];
}
