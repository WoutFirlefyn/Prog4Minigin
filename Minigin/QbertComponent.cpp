//---------------------------
// Includes
//---------------------------
#include "QbertComponent.h"
#include "GameObject.h"
#include "Time.h"
#include "SpritesheetComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::QbertComponent::QbertComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
	PlayerDied = std::make_unique<Subject<>>();
	PlayerMoveStateChanged = std::make_unique<Subject<MovementState, MovementDirection>>();
	//PlayerStopMoving = std::make_unique<Subject<>>();
}

void dae::QbertComponent::Init()
{
	GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(static_cast<int>(MovementDirection::Right), 0);
}

void dae::QbertComponent::Update()
{
	if (m_MovementDirection == MovementDirection::None)
		return;

	m_AccumSec += Time::GetInstance().GetDeltaTime();

	float t = std::min(m_AccumSec / m_JumpDuration, 1.f);

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

	glm::vec3 control = m_StartPos;

	if (static_cast<int>(m_MovementDirection) < 2)
		control += (endPos - m_StartPos) * glm::vec3( 0.1f, 1.f, 0.f );
	else
		control += (endPos - m_StartPos) * glm::vec3{ 1.0f, 0.1f, 0.f };

	glm::vec3 currentPos{};
	currentPos.x = (1 - t) * (1 - t) * m_StartPos.x + 2 * (1 - t) * t * control.x + t * t * endPos.x;
	currentPos.y = (1 - t) * (1 - t) * m_StartPos.y + 2 * (1 - t) * t * control.y + t * t * endPos.y;

	GetGameObject()->SetPosition(currentPos);

	if (t >= 1.f)
	{
		PlayerMoveStateChanged->NotifyObservers(MovementState::End, m_MovementDirection);
		m_MovementDirection = MovementDirection::None;
	}
}

void dae::QbertComponent::Jump(MovementDirection direction)
{
	if (IsMoving())
		return;
	PlayerMoveStateChanged->NotifyObservers(MovementState::Start, direction);
	m_MovementDirection = direction;
	GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(static_cast<int>(direction), 0);
	m_AccumSec = 0.f;
	m_StartPos = GetGameObject()->GetLocalPosition();
}

//---------------------------
// Member functions
//---------------------------
void dae::QbertComponent::Die()
{
	--m_Lives;
	PlayerDied->NotifyObservers();
}
