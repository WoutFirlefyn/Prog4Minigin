//---------------------------
// Includes
//---------------------------
#include "DiskComponent.h"
#include "QbertComponent.h"
#include "GameObject.h"
#include "GameTime.h"

//---------------------------
// Constructor & Destructor
//---------------------------
DiskComponent::DiskComponent(dae::GameObject* pGameObject, dae::GameObject* pTopTile) : BaseComponent(pGameObject)
	, m_pTopTile{ pTopTile }
{
}

DiskComponent::~DiskComponent()
{
}

void DiskComponent::Update()
{
	if (m_pCharacter.second == nullptr)
		return;

	m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

	float t = m_AccumSec / m_TimeToReachTop;

	glm::vec3 endPos = m_pTopTile->GetWorldPosition();

	GetGameObject()->SetPosition(m_StartPos + (endPos - m_StartPos) * t);

	if (t >= 1)
		m_pCharacter.second = nullptr;
}

std::pair<Character, dae::GameObject*> DiskComponent::GetCharacter() const
{
	return m_pCharacter;
}

void DiskComponent::MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character)
{
	m_StartPos = GetGameObject()->GetWorldPosition();
	m_pCharacter = character;
	character.second->SetParent(GetGameObject(), true);
}



