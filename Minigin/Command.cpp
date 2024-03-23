#include "Command.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "Time.h"

void dae::MoveCommand::Execute()
{
	auto go = GetGameObject();
	if (go)
		go->SetPosition(go->GetLocalPosition() + m_Direction * Time::GetInstance().GetDeltaTime());
}

void dae::KillCommand::Execute()
{
	auto go = GetGameObject();
	if (go)
		go->GetComponent<dae::HealthComponent>()->LoseALife();
}

void dae::ScoreCommand::Execute()
{
	auto go = GetGameObject();
	if (go)
		go->GetComponent<dae::ScoreComponent>()->IncreaseScore(m_ScoreIncrease);
}
