#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;
}

class FPSComponent final : public dae::BaseComponent
{
public:
	FPSComponent(dae::GameObject* pGameObject);	
	virtual ~FPSComponent() override = default;	

	FPSComponent(const FPSComponent& other) = delete;
	FPSComponent(FPSComponent&& other) noexcept = delete;
	FPSComponent& operator=(const FPSComponent& other) = delete;
	FPSComponent& operator=(FPSComponent&& other)	noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;
private:
	dae::TextComponent* m_pTextComponent{};
	int m_Count{};
	float m_MaxDelay{ 0.1f };
	float m_Delay{};
};


 
