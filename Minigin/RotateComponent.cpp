//---------------------------
// Includes
//---------------------------
#include "RotateComponent.h"
#include "GameObject.h"
#include "Time.h"

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
	m_Center = GetGameObject()->GetWorldTransform().GetPosition();
}

void dae::RotateComponent::Render() const
{

}

void dae::RotateComponent::Update()
{
	m_CurrentAngle += m_Speed * Time::GetInstance().GetDeltaTime();

	glm::vec3 newPosition{ cosf(m_CurrentAngle) * m_Distance, sinf(m_CurrentAngle) * m_Distance, 0 };
	if (GetGameObject()->GetParent() == nullptr)
		newPosition += m_Center;
	GetGameObject()->SetPosition(newPosition);
}

void dae::RotateComponent::FixedUpdate()
{
}

//---------------------------
// Member functions
//---------------------------

// write member functions here




