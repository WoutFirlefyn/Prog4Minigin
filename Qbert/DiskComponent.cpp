#include "DiskComponent.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "TileComponent.h"
#include "CharacterComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "Sounds.h"

std::unique_ptr<dae::Subject<dae::GameObject*, Character>> DiskComponent::DiskReachedTop{ std::make_unique<dae::Subject<dae::GameObject*, Character>>() };

DiskComponent::DiskComponent(dae::GameObject* pGameObject) : BaseComponent(pGameObject)
{
}

DiskComponent::~DiskComponent()
{
	DiskReachedTop->RemoveObserver(this);
}

void DiskComponent::Init()
{
	DiskReachedTop->AddObserver(this);
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

	if (m_pCharacter.second == nullptr)
		return;

	m_PlatformLerpValue = std::min(m_PlatformLerpValue + (deltaTime / m_TimeToReachTop), 1.f);

	GetGameObject()->SetPosition(m_StartPos + (m_EndPos - m_StartPos) * m_PlatformLerpValue);

	if (m_PlatformLerpValue >= 1.f)
		DiskReachedTop->NotifyObservers(GetGameObject(), m_pCharacter.first);
}

void DiskComponent::Notify(dae::GameObject* pDisk, Character)
{
	if (pDisk != GetGameObject())
		return;

	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::DiskLand, 0.2f);
	GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
}

std::pair<Character, dae::GameObject*> DiskComponent::GetCharacter()
{
	auto characterToReturn = m_pCharacter;
	m_pCharacter = { Character::None, nullptr };
	return characterToReturn;
}

void DiskComponent::MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character, dae::GameObject* pTopTile)
{
	m_pCharacter = character;
	m_StartPos = GetGameObject()->GetLocalPosition();

	const glm::vec3 scale = GetGameObject()->GetWorldScale();
	const glm::ivec2 tileSize = pTopTile->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
	const glm::ivec2 diskSize = GetGameObject()->GetComponent<dae::GraphicsComponent>()->GetTextureSize();

	const glm::vec3 offset = (glm::vec3{ tileSize.x * 0.5f, -tileSize.y, 0 } + glm::vec3{ -diskSize.x, diskSize.y, 0 } * 0.5f) * scale;

	m_EndPos = pTopTile->GetLocalPosition() + offset;
	character.second->SetParent(GetGameObject(), true);

	dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::DiskLift, 0.2f);
}



