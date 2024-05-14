#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

class RotateComponent final : public dae::BaseComponent
{
public:
	RotateComponent(dae::GameObject* pGameObject);
	RotateComponent(dae::GameObject* pGameObject, float speed, float distance);
	virtual ~RotateComponent() = default;

	RotateComponent(const RotateComponent& other)					= delete;
	RotateComponent(RotateComponent&& other) noexcept				= delete;
	RotateComponent& operator=(const RotateComponent& other)		= delete;
	RotateComponent& operator=(RotateComponent&& other)	noexcept	= delete;

	virtual void Init() override;
	virtual void Update() override;
private:
	glm::vec3 m_Center{};
	float m_StartAngle{};
	float m_CurrentAngle{};
	float m_Speed{ 5.f };
	float m_Distance{ 50.f };
};


 
