#include "DiskComponent.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "TileComponent.h"
#include "CharacterComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "Sounds.h"

std::unique_ptr<dae::Subject<Disk, Character>> DiskComponent::DiskStateChanged{ std::make_unique<dae::Subject<Disk, Character>>() };

DiskComponent::DiskComponent(dae::GameObject* pGameObject, dae::GameObject* pTopTile)
	: BaseComponent(pGameObject)
	, m_pTopTile{ pTopTile }
	, m_Character{ Character::None }
{
}

DiskComponent::~DiskComponent()
{
	DiskStateChanged->RemoveObserver(this);
}

void DiskComponent::Init()
{
	DiskStateChanged->AddObserver(this);

	m_pSpritesheetComponent = GetGameObject()->GetComponent<dae::SpritesheetComponent>();
	assert(m_pSpritesheetComponent);
	m_pSpritesheetComponent->MoveSourceRect(rand() % 4, 0);
}

void DiskComponent::Update()
{
	float deltaTime = dae::GameTime::GetInstance().GetDeltaTime();
	m_AccumSec += deltaTime;

	float secondsPerFrame{ 1.f / m_Fps };
	if (m_AccumSec > secondsPerFrame)
	{
		m_AccumSec -= secondsPerFrame;
		m_pSpritesheetComponent->MoveSourceRect((m_pSpritesheetComponent->GetCurrCol() + 1) % 4, 0);
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

		const glm::ivec2 tileSize = m_pTopTile->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
		const glm::ivec2 diskSize = GetGameObject()->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
		const glm::vec3 offset = (glm::vec3{ tileSize.x * 0.5f, -tileSize.y, 0 } + glm::vec3{ -diskSize.x, diskSize.y, 0 } *0.5f) * GetGameObject()->GetWorldScale();
		m_EndPos = m_pTopTile->GetLocalPosition() + offset;

		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::DiskLift, 0.2f);
		break;
	case DiskState::Stop:
		m_Character = Character::None;
		GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::DiskLand, 0.2f);
		break;
	}

}



