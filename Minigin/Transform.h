#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		Transform() = default;
		Transform(const glm::vec3& pos);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& pos);
	
		Transform operator+(const Transform& rhs) const
		{
			return Transform(m_Position + rhs.GetPosition());
		}
		Transform operator-(const Transform& rhs) const
		{
			return Transform(m_Position - rhs.GetPosition());
		}
	private:
		glm::vec3 m_Position;
	};
}
