#include <sstream>
#include <iomanip>
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "GameTime.h"

FPSComponent::FPSComponent(dae::GameObject* pGameObject) : BaseComponent(pGameObject)
{
}

void FPSComponent::Init()
{
	m_pTextComponent = GetGameObject()->GetComponent<dae::TextComponent>();
}

void FPSComponent::Update()
{
	++m_Count;
	m_Delay += dae::GameTime::GetInstance().GetDeltaTime();

	if (m_Delay >= m_MaxDelay)
	{
		std::stringstream fps;
		fps << std::fixed << std::setprecision(1) << (static_cast<float>(m_Count) / m_Delay);

		if (m_pTextComponent != nullptr)
			m_pTextComponent->SetText(fps.str() + " FPS");

		m_Delay = 0;
		m_Count = 0;
	}
}