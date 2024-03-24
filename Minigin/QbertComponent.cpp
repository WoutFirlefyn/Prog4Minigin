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
	PlayerDied = std::make_unique<Subject<>>();
	ScoreChanged = std::make_unique<Subject<>>();
}

//---------------------------
// Member functions
//---------------------------
void dae::QbertComponent::Die()
{
	--m_Lives;
	PlayerDied->NotifyObservers();
}

void dae::QbertComponent::GainScore(ScoreType type)
{
	switch (type)
	{
	case ScoreType::TileCovered:
		m_Score += 25;
		break;
	case ScoreType::LevelCleared:
		m_Score += 100;
		break;
	}

	ScoreChanged->NotifyObservers();
}
