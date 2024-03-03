//---------------------------
// Includes
//---------------------------
#include <sstream>
#include <iomanip>
#include "FPSComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Time.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::FPSComponent::FPSComponent(GameObject* pGameObject) : BaseComponent(pGameObject)
{
	m_pTextComponent = GetGameObject()->GetComponent<TextComponent>();
}

//---------------------------
// Member functions
//---------------------------


void dae::FPSComponent::Init()
{

}

void dae::FPSComponent::Render() const
{
}

void dae::FPSComponent::Update()
{
	++m_Count;
	m_Delay += Time::GetInstance().GetDeltaTime();

	if (m_Delay >= m_MaxDelay)
	{
		std::stringstream fps;
		fps << std::fixed << std::setprecision(1) << (float(m_Count) / m_Delay);

		if (m_pTextComponent != nullptr)
			m_pTextComponent->SetText(fps.str() + " FPS");

		m_Delay = 0;
		m_Count = 0;
	}
}

void dae::FPSComponent::FixedUpdate()
{

}




