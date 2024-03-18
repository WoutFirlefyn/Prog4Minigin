#include "Command.h"
#include "GameObject.h"
#include "Time.h"

void dae::MoveCommand::Execute()
{
	auto go = GetGameObject();
	if (go)
		go->SetPosition(go->GetLocalPosition() + m_Direction * Time::GetInstance().GetDeltaTime());
}