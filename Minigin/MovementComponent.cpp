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
		go->SetPosition(go->GetLocalPosition() + m_Velocity);

		m_Velocity = glm::vec3{};
	}
}

void dae::MovementComponent::FixedUpdate()
{

}

void dae::MovementComponent::Move(MovementDir dir)
{
	glm::vec3 velocity{};

	switch (dir)
	{
	case MovementDir::Up:
		velocity = glm::vec3{ 0, -m_MovementSpeed, 0 };
		break;
	case MovementDir::Down:
		velocity = glm::vec3{ 0, m_MovementSpeed, 0 };
		break;
	case MovementDir::Left:
		velocity = glm::vec3{ -m_MovementSpeed, 0, 0 };
		break;
	case MovementDir::Right:
		velocity = glm::vec3{ m_MovementSpeed, 0, 0 };
		break;
	default:
		assert(false && "Invalid movement direction");
		break;
	}

	velocity *= Time::GetInstance().GetDeltaTime();
	m_Velocity += velocity;
}



