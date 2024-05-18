#include "DiskComponent.h"
#include "SpritesheetComponent.h"
#include "GraphicsComponent.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "Sounds.h"

DiskComponent::DiskComponent(dae::GameObject* pGameObject, dae::GameObject* pTopTile) : BaseComponent(pGameObject)
	, m_pTopTile{ pTopTile }
	, m_pSpritesheetComponent{ nullptr }
{
}

void DiskComponent::Init()
{
	m_pSpritesheetComponent = GetGameObject()->GetComponent<dae::SpritesheetComponent>();
	assert(m_pSpritesheetComponent);
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

	glm::vec3 endPos = m_pTopTile->GetLocalPosition();

	GetGameObject()->SetPosition(m_StartPos + (endPos - m_StartPos) * m_PlatformLerpValue);

	if (m_PlatformLerpValue >= 1.f)
	{
		dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::DiskLand, 0.2f);

		m_pCharacter.second->SetParent(nullptr);
		m_pCharacter.second->SetPosition(308, 193);
		m_pTopTile->GetComponent<TileComponent>()->MoveCharacterHere(m_pCharacter);
		m_pCharacter.second = nullptr;
		GetGameObject()->GetComponent<dae::GraphicsComponent>()->ToggleRendering(false);
	}
}

void DiskComponent::MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character)
{
	m_pCharacter = character;
	m_StartPos = GetGameObject()->GetLocalPosition();
	character.second->SetParent(GetGameObject(), true);
}



