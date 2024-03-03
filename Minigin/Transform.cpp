#include "Transform.h"

dae::Transform::Transform(const glm::vec3& pos) : Transform(pos, glm::vec3(3), glm::vec3(0))
{
}

dae::Transform::Transform(const glm::vec3& pos, const glm::vec3& rot) : Transform(pos, rot, glm::vec3(0))
{
}

dae::Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
	: m_Position{ pos }
	, m_Rotation{ rot }
	, m_Scale{ scale }
{
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::Transform::SetRotation(const float x, const float y, const float z)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

void dae::Transform::SetScale(const float x, const float y, const float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}
