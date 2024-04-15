//---------------------------
// Includes
//---------------------------
#include "QbertComponent.h"
#include "GameObject.h"
#include "Time.h"
#include "SpritesheetComponent.h"
#include "TileComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::QbertComponent::QbertComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
	PlayerDied = std::make_unique<Subject<>>();
	PlayerMoveStateChanged = std::make_unique<Subject<MovementState, MovementDirection>>();
}

dae::QbertComponent::~QbertComponent()
{
	TileComponent::TileChanged->RemoveObserver(this);
}

void dae::QbertComponent::Init()
{
	PlayerMoveStateChanged->AddObserver(this);
	TileComponent::TileChanged->AddObserver(this);
	GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(static_cast<int>(MovementDirection::Right), 0);
}

void dae::QbertComponent::Update()
{
	if (m_MovementDirection == MovementDirection::None)
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

	glm::vec3 control { m_StartPos };

	if (static_cast<int>(m_MovementDirection) < 2)
		control += (endPos - m_StartPos) * glm::vec3( 0.1f, 1.f, 0.f );
	else
		control += (endPos - m_StartPos) * glm::vec3{ 1.f, 0.1f, 0.f };

	m_AccumSec += Time::GetInstance().GetDeltaTime();
	float t = std::min(m_AccumSec / m_JumpDuration, 1.f);

	glm::vec3 currentPos = (1 - t) * (1 - t) * m_StartPos + 2 * (1 - t) * t * control + t * t * endPos;

	GetGameObject()->SetPosition(currentPos);

	if (t >= 1.f)
	{
		PlayerMoveStateChanged->NotifyObservers((m_IsFalling ? MovementState::Falling : MovementState::End), m_MovementDirection);
		m_MovementDirection = MovementDirection::None;
	}
}

void dae::QbertComponent::Notify(MovementState movementState, MovementDirection movementDirection)
{
	switch (movementState)
	{
	case MovementState::Start:
		m_MovementDirection = movementDirection;
		GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(static_cast<int>(m_MovementDirection), 0);
		m_AccumSec = 0.f;
		m_StartPos = GetGameObject()->GetLocalPosition();
		break;
	case MovementState::Moving:
		break;
	case MovementState::End:
		break;
	case MovementState::Falling:
		Die();
		GetGameObject()->SetPosition(m_StartPos);
		m_IsFalling = false;
		break;
	default:
		break;
	}
}

void dae::QbertComponent::Notify(bool roundFinished)
{
	if (roundFinished)
		GetGameObject()->SetPosition(308, 193);
}

void dae::QbertComponent::Die()
{
	--m_Lives;
	PlayerDied->NotifyObservers();
}
