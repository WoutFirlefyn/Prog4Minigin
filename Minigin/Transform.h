#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		Transform() = default;
		Transform(const glm::vec3& pos, const glm::vec3& rot = glm::vec3(0), const glm::vec3& scale = glm::vec3(1));

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& pos) { m_Position = pos; }

		const glm::vec3& GetRotation() const { return m_Rotation; }
		void SetRotation(float x, float y, float z);
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }

		const glm::vec3& GetScale() const { return m_Scale; }
		void SetScale(float x, float y, float z);
		void SetScale(float scale) { m_Scale = glm::vec3{ scale }; }
		void SetScale(const glm::vec3& scale) { m_Scale = scale; }

		Transform operator+(const Transform& rhs) const
		{
			return Transform(m_Position + rhs.GetPosition(), m_Rotation + rhs.GetRotation(), m_Scale * rhs.GetScale());
		}
		Transform operator-(const Transform& rhs) const
		{
			return Transform(m_Position - rhs.GetPosition(), m_Rotation - rhs.GetRotation(), m_Scale / rhs.GetScale());
		}
	private:
		glm::vec3 m_Position{ glm::vec3(0) };
		glm::vec3 m_Rotation{ glm::vec3(0) };
		glm::vec3 m_Scale{ glm::vec3(1) };
	};
}
