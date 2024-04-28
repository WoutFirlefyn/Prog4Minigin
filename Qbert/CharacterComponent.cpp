#include "CharacterComponent.h"
#include "GameTime.h"
#include "GameObject.h"

CharacterComponent::CharacterComponent(dae::GameObject* pGameObject) 
	: BaseComponent(pGameObject)
{
	MoveStateChanged = std::make_unique<dae::Subject<Character, MovementState, MovementDirection>>();
}

CharacterComponent::~CharacterComponent()
{
}

void CharacterComponent::Init()
{
	MoveStateChanged->AddObserver(this);
}

void CharacterComponent::Update()
{
	if (!IsMoving())
		return;

	glm::vec3 endPos{ m_StartPos };
	switch (m_MovementDirection)
	{
	case MovementDirection::Up:
		endPos += glm::vec3{ 16, -24, 0 };
		break;
	case MovementDirection::Down:
		endPos += glm::vec3{ -16, 24, 0 };
		break;
	case MovementDirection::Left:
		endPos += glm::vec3{ -16, -24, 0 };
		break;
	case MovementDirection::Right:
		endPos += glm::vec3{ 16, 24, 0 };
		break;
	default:
		return;
	}

	glm::vec3 control{ m_StartPos };

	if (static_cast<int>(m_MovementDirection) < 2)
		control += (endPos - m_StartPos) * glm::vec3(0.2f, 1.f, 0.f);
	else
		control += (endPos - m_StartPos) * glm::vec3{ 1.f, 0.2f, 0.f };

	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();
	float t = std::min(m_AccumSec / m_JumpDuration, 1.f);

	glm::vec3 currentPos = (1 - t) * (1 - t) * m_StartPos + 2 * (1 - t) * t * control + t * t * endPos;

	GetGameObject()->SetPosition(currentPos);

	if (t >= 1.f)
		MoveStateChanged->NotifyObservers(Character::Qbert1, MovementState::End, m_MovementDirection);
}

void CharacterComponent::Notify(Character , MovementState , MovementDirection )
{
}
