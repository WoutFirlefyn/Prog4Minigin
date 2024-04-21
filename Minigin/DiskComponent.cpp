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
dae::DiskComponent::DiskComponent(GameObject* pGameObject, GameObject* pTopTile) : BaseComponent(pGameObject)
	, m_pTopTile{ pTopTile }
{
}

dae::DiskComponent::~DiskComponent()
{
}

void dae::DiskComponent::Init()
{
}

void dae::DiskComponent::Update()
{
	if (m_pCharacter.second == nullptr)
		return;

	m_AccumSec += GameTime::GetInstance().GetDeltaTime();

	float t = m_AccumSec / m_TimeToReachTop;

	glm::vec3 endPos = m_pTopTile->GetWorldPosition();

	GetGameObject()->SetPosition(m_StartPos + (endPos - m_StartPos) * t);

	if (t >= 1)
		m_pCharacter.second = nullptr;
}

std::pair<dae::Character, dae::GameObject*> dae::DiskComponent::GetCharacter() const
{
	return m_pCharacter;
}

void dae::DiskComponent::MoveCharacterHere(const std::pair<Character, GameObject*>& character)
{
	m_StartPos = GetGameObject()->GetWorldPosition();
	m_pCharacter = character;
	character.second->SetParent(GetGameObject(), true);
}



