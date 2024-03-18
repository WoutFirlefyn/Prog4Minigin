//---------------------------
// Includes
//---------------------------
#include "MovementComponent.h"
#include "GameObject.h"
#include "Time.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::MovementComponent::MovementComponent(GameObject* pGameObject, float movementSpeed)
	: BaseComponent(pGameObject)
	, m_MovementSpeed(movementSpeed)
{
}

//---------------------------
// Member functions
//---------------------------
void dae::MovementComponent::Init()
{

}

void dae::MovementComponent::Render() const
{

}

void dae::MovementComponent::Update()
{
	if (glm::length(m_Velocity) > 0)
	{
		auto go = GetGameObject();
		go->SetPosition(go->GetLocalPosition() + glm::normalize(m_Velocity) * m_MovementSpeed * Time::GetInstance().GetDeltaTime());

		m_Velocity = glm::vec3{};
	}
}

void dae::MovementComponent::FixedUpdate()
{
}

void dae::MovementComponent::RenderGUI()
{
}

void dae::MovementComponent::Move(MovementDir dir)
{
	glm::vec3 velocity{};

	switch (dir)
	{
	case MovementDir::Up:
		velocity = glm::vec3{ 0, -1, 0 };
		break;
	case MovementDir::Down:
		velocity = glm::vec3{ 0, 1, 0 };
		break;
	case MovementDir::Left:
		velocity = glm::vec3{ -1, 0, 0 };
		break;
	case MovementDir::Right:
		velocity = glm::vec3{ 1, 0, 0 };
		break;
	default:
		assert(false && "Invalid movement direction");
		break;
	}

	m_Velocity += velocity;
}



