//---------------------------
// Includes
//---------------------------
#include "DiskComponent.h"
#include "SpritesheetComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ServiceLocator.h"
#include "SDLSoundSystem.h"

//---------------------------
// Constructor & Destructor
//---------------------------
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
	auto& time = dae::GameTime::GetInstance();
	m_AccumSec += time.GetDeltaTime();

	float secondsPerFrame{ 1.f / m_Fps };
	if (m_AccumSec > secondsPerFrame)
	{
		m_AccumSec -= secondsPerFrame;
		m_pSpritesheetComponent->MoveSourceRect((m_pSpritesheetComponent->GetCurrCol() + 1) % 4, 0);
	}

	if (m_pCharacter.second == nullptr)
		return;

	m_PlatformLerpValue = std::min(m_PlatformLerpValue + (time.GetDeltaTime() / m_TimeToReachTop), 1.f);

	glm::vec3 endPos = m_pTopTile->GetWorldPosition();

	GetGameObject()->SetPosition(m_StartPos + (endPos - m_StartPos) * m_PlatformLerpValue);

	if (1.f - m_PlatformLerpValue < FLT_EPSILON)
	{
		dae::ServiceLocator::GetSoundSystem().Play(static_cast<dae::SoundId>(Sounds::DiskLand));
		m_pCharacter.second = nullptr;
	}
}

std::pair<Character, dae::GameObject*> DiskComponent::GetCharacter() const
{
	return m_pCharacter;
}

void DiskComponent::MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character)
{
	dae::ServiceLocator::GetSoundSystem().Play(static_cast<dae::SoundId>(Sounds::DiskLift));
	m_AccumSec = 0.f;
	m_StartPos = GetGameObject()->GetWorldPosition();
	m_pCharacter = character;
	character.second->SetParent(GetGameObject(), true);
}



