//---------------------------
// Includes
//---------------------------
#include "QbertComponent.h"
#include "GameObject.h"
#include "Time.h"
#include "SpritesheetComponent.h"
#include "TileComponent.h"
#include "LevelManagerComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::QbertComponent::QbertComponent(GameObject* pGameObject) 
	: BaseComponent(pGameObject)
{
	PlayerDied = std::make_unique<Subject<>>();
	MoveStateChanged = std::make_unique<Subject<Character, MovementState, MovementDirection>>();
}

dae::QbertComponent::~QbertComponent()
{
	if (m_pLevelManagerComponent)
		m_pLevelManagerComponent->TileChanged->RemoveObserver(this);
}

void dae::QbertComponent::Init()
{
	MoveStateChanged->AddObserver(this);
	m_pLevelManagerComponent->TileChanged->AddObserver(this);
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
		MoveStateChanged->NotifyObservers(Character::Qbert1, (m_IsFalling ? MovementState::Falling : MovementState::End), m_MovementDirection);
		m_MovementDirection = MovementDirection::None;
	}
}

void dae::QbertComponent::AddObserver(BaseComponent* pBaseComponent)
{
	if (auto pComponent = dynamic_cast<LevelManagerComponent*>(pBaseComponent))
		m_pLevelManagerComponent = pComponent;
}

void dae::QbertComponent::Notify(Character, MovementState movementState, MovementDirection movementDirection)
{
	switch (movementState)
	{
	case MovementState::Start:
		m_MovementDirection = movementDirection;
		GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(static_cast<int>(m_MovementDirection), 0);
		m_AccumSec = 0.f;
		m_StartPos = GetGameObject()->GetLocalPosition();
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

void dae::QbertComponent::SubjectDestroyed(Subject<bool>* pSubject)
{
	if (pSubject == m_pLevelManagerComponent->TileChanged.get())
		m_pLevelManagerComponent = nullptr;
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
