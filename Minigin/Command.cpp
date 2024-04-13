#include "Command.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include "ScoreComponent.h"
#include "Time.h"

void dae::MoveCommand::Execute()
{
	auto go = GetGameObject();
	if (go)
		go->GetComponent<QbertComponent>()->Jump(m_Direction);
}

void dae::KillCommand::Execute()
{
	auto go = GetGameObject();
	if (go)
		go->GetComponent<dae::QbertComponent>()->Die();
}

void dae::ScoreCommand::Execute()
{
	//auto go = GetGameObject();
	//if (go)
	//	go->GetComponent<dae::QbertComponent>()->GainScore(m_ScoreType);
}
