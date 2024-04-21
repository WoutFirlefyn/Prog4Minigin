//---------------------------
// Includes
//---------------------------
#define _USE_MATH_DEFINES
#include "RotateComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include <cmath>

//---------------------------
// Constructor & Destructor
//---------------------------
RotateComponent::RotateComponent(dae::GameObject* pGameObject) : BaseComponent(pGameObject)
{
}

RotateComponent::RotateComponent(dae::GameObject* pGameObject, float speed, float distance) : BaseComponent(pGameObject)
	, m_Speed{ speed }
	, m_Distance{ distance }
{
}

void RotateComponent::Init()
{
	m_Center = GetGameObject()->GetLocalPosition();
}

void RotateComponent::Update()
{
	m_CurrentAngle += m_Speed * dae::GameTime::GetInstance().GetDeltaTime();
	
	m_CurrentAngle = std::fmod(m_CurrentAngle, 2 * static_cast<float>(M_PI));

	glm::vec3 newPosition{ cosf(m_CurrentAngle) * m_Distance, sinf(m_CurrentAngle) * m_Distance, 0 };
	newPosition += m_Center;

	GetGameObject()->SetPosition(newPosition);
}