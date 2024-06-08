#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class LevelManagerComponent;
enum class GameState;
class RoundCounterComponent final : public dae::BaseComponent, public dae::Observer<GameState>
{
public:
	RoundCounterComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~RoundCounterComponent() override;

	RoundCounterComponent(const RoundCounterComponent& other) = delete;
	RoundCounterComponent(RoundCounterComponent&& other) noexcept = delete;
	RoundCounterComponent& operator=(const RoundCounterComponent& other) = delete;
	RoundCounterComponent& operator=(RoundCounterComponent&& other)	noexcept = delete;

	virtual void Init() override;
	//virtual void Update() override;

	virtual void Notify(GameState gameState) override;
	virtual void SubjectDestroyed(dae::Subject<GameState>* pSubject) override;
private:
	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };
	dae::Subject<GameState>* m_pGameResumedSubject{ nullptr };
};

