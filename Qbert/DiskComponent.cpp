#include "DiskComponent.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "TileComponent.h"
#include "CharacterComponent.h"
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "Sounds.h"

std::unique_ptr<dae::Subject<Disk, Character>> DiskComponent::DiskStateChanged{ std::make_unique<dae::Subject<Disk, Character>>() };

DiskComponent::DiskComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent)
	: BaseComponent(pGameObject)
	, m_pLevelManagerComponent{ pLevelManagerComponent }
	, m_Character{ Character::None }
{
	m_pNewRoundStarted = pLevelManagerComponent->NewRoundStarted.get();
	m_pNewRoundStarted->AddObserver(this);
}

DiskComponent::~DiskComponent()
{
	DiskStateChanged->RemoveObserver(this);
	if (m_pNewRoundStarted)
		m_pNewRoundStarted->RemoveObserver(this);
}

void DiskComponent::Init()
{
	DiskStateChanged->AddObserver(this);

	m_pSpritesheetComponent = GetGameObject()->GetComponent<dae::SpritesheetComponent>();
	assert(m_pSpritesheetComponent);
	m_pSpritesheetComponent->MoveSourceRect(rand(), 0);
}

void DiskComponent::Update()
{
	if (LevelManagerComponent::IsRoundOver())
		return;

	float deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
	m_AccumSec += deltaTime;

	float secondsPerFrame{ 1.f / m_Fps };
	if (m_AccumSec > secondsPerFrame)
	{
		m_AccumSec -= secondsPerFrame;
		m_pSpritesheetComponent->MoveSourceRectRelative(1, 0);
	}

	if (m_Character == Character::None)
		return;

	m_PlatformLerpValue = std::min(m_PlatformLerpValue + (deltaTime / m_TimeToReachTop), 1.f);

	GetGameObject()->SetPosition(m_StartPos + (m_EndPos - m_StartPos) * m_PlatformLerpValue);

	if (m_PlatformLerpValue >= 1.f)
		DiskStateChanged->NotifyObservers({ GetGameObject(), DiskState::Stop }, m_Character);
}

void DiskComponent::Notify(Disk disk, Character character)
{
	if (disk.pGameObject != GetGameObject())
		return;

	switch (disk.state)
	{
	case DiskState::Start:
		m_Character = character;

		m_StartPos = GetGameObject()->GetLocalPosition();
		const glm::ivec2 tileSize = m_pLevelManagerComponent->GetTileSize();
		m_EndPos = m_pLevelManagerComponent->GetTilePos({ -1,-1 }) + glm::vec3{ 0, tileSize.y / 2.f, 0 } *GetGameObject()->GetWorldScale();

		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::DiskLift, 0.2f);
		break;
	case DiskState::Stop:
		m_Character = Character::None;

		GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);

		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::DiskLand, 0.2f);
		break;
	}
}

void DiskComponent::Notify()
{
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(true);
	m_pSpritesheetComponent->MoveSourceRect(rand(), LevelManagerComponent::GetRoundNr());
	m_Character = Character::None;
	m_PlatformLerpValue = 0.f;
}

void DiskComponent::SubjectDestroyed(dae::Subject<>* pSubject)
{
	if (pSubject == m_pNewRoundStarted)
		m_pNewRoundStarted = nullptr;
}



