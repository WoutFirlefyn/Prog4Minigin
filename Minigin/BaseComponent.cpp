//---------------------------
// Includes
//---------------------------
#include "BaseComponent.h"

dae::BaseComponent::BaseComponent(GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
}

dae::GameObject* dae::BaseComponent::GetParent() const
{
	return m_pGameObject;
}
