#include "Observer.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include <algorithm>

void dae::Subject::AddObserver(std::unique_ptr<Observer>&& pObserver)
{ 
	if (pObserver)
		m_vObservers.push_back(std::move(pObserver));
}

void dae::Subject::RemoveObserver(std::unique_ptr<Observer>&& pObserver)
{
	if (pObserver)
		m_vObservers.erase(std::remove(m_vObservers.begin(), m_vObservers.end(), pObserver), m_vObservers.end());
}

void dae::Subject::NotifyObservers(GameObject* pGameObject)
{
	for (auto& pObserver : m_vObservers)
		pObserver->Notify(pGameObject);
}


dae::HealthObserver::HealthObserver(TextComponent* pTextComponent)
	: m_pTextComponent{ pTextComponent }
{
}

void dae::HealthObserver::Notify(GameObject* pGameObject)
{
	if (!m_pTextComponent)
		return;

	int lives{ pGameObject->GetComponent<HealthComponent>()->GetLives() };

	m_pTextComponent->SetText("Lives: " + std::to_string(lives));
}

dae::ScoreObserver::ScoreObserver(TextComponent* pTextComponent)
	: m_pTextComponent{ pTextComponent }
{
}

void dae::ScoreObserver::Notify(GameObject* pGameObject)
{
	if (!m_pTextComponent)
		return;

	int score{ pGameObject->GetComponent<ScoreComponent>()->GetScore() };

	m_pTextComponent->SetText("Score: " + std::to_string(score));
}
