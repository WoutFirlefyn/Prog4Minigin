//---------------------------
// Includes
//---------------------------
#include "QbertComponent.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::QbertComponent::QbertComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
	// nothing to create
}

//---------------------------
// Member functions
//---------------------------
void dae::QbertComponent::Die()
{
	--m_Lives;
	PlayerDied.NotifyObservers();
}

void dae::QbertComponent::GainScore(ScoreType type)
{
	ScoreChanged.NotifyObservers(type);
}
