//---------------------------
// Includes
//---------------------------
#define _USE_MATH_DEFINES
#include "RotateComponent.h"
#include "GameObject.h"
#include "Time.h"
#include <cmath>

//---------------------------
// Constructor & Destructor
//---------------------------
dae::RotateComponent::RotateComponent(GameObject* pGameObject) : BaseComponent(pGameObject)
{
}

dae::RotateComponent::RotateComponent(GameObject* pGameObject, float speed, float distance) : BaseComponent(pGameObject)
	, m_Speed{ speed }
	, m_Distance{ distance }
{
}

void dae::RotateComponent::Init()
{
	m_Center = GetGameObject()->GetLocalPosition();
}

void dae::RotateComponent::Update()
{
	m_CurrentAngle += m_Speed * Time::GetInstance().GetDeltaTime();
	
	m_CurrentAngle = std::fmod(m_CurrentAngle, 2 * static_cast<float>(M_PI));

	glm::vec3 newPosition{ cosf(m_CurrentAngle) * m_Distance, sinf(m_CurrentAngle) * m_Distance, 0 };
	newPosition += m_Center;

	GetGameObject()->SetPosition(newPosition);
}