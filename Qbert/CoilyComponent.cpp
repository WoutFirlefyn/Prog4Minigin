#include "CoilyComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "Sounds.h"
#include "CoilyStates.h"

bool CoilyComponent::m_IsEgg{ true };

CoilyComponent::CoilyComponent(dae::GameObject* pGameObject) 
	: CharacterComponent(pGameObject)
{
}

CoilyComponent::~CoilyComponent()
{
	CharacterSpawned->RemoveObserver(this);
}

void CoilyComponent::Init()
{
	CharacterComponent::Init();
	CharacterSpawned->AddObserver(this);
	m_Character = Character::Coily;
	SetState(std::make_unique<CoilySpawnState>(this));
}

void CoilyComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character != m_Character)
		return;

	switch (movementInfo.state)
	{
	case MovementState::End:
		dae::ServiceLocator::GetSoundSystem().Play((m_IsEgg ? dae::Sounds::CoilyEggJump : dae::Sounds::CoilySnakeJump), 0.2f);
		GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRectRelative(0, -1);
		if (++m_AmountOfJumps == 6)
			m_IsEgg = false;
		break;
	default:
		break;
	}
}

void CoilyComponent::Notify(Character character)
{
	if (m_Character != character)
		return;

	m_IsEgg = true;
	m_AmountOfJumps = 0;
}
