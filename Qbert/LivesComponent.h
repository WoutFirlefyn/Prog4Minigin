#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class LevelManagerComponent;
enum class GameState;
class LivesComponent final : public dae::BaseComponent, public dae::Observer<>, public dae::Observer<GameState>
{
public:
	LivesComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~LivesComponent() override;	

	LivesComponent(const LivesComponent& other) = delete;
	LivesComponent(LivesComponent&& other) noexcept = delete;
	LivesComponent& operator=(const LivesComponent& other) = delete;
	LivesComponent& operator=(LivesComponent&& other) noexcept = delete;

	virtual void Init() override;

	void AddObserver(dae::Subject<>* pPlayerDiedSubject);

	virtual void Notify() override;
	virtual void SubjectDestroyed(dae::Subject<>* pSubject) override;

	virtual void Notify(GameState gameState) override;
	virtual void SubjectDestroyed(dae::Subject<GameState>* pSubject) override;
private:
	dae::Subject<>* m_pPlayerDiedSubject{ nullptr };
	dae::Subject<GameState>* m_pGameResumedSubject{ nullptr };
	std::vector<dae::GameObject*> m_vHearts;
	int m_Lives{ 3 };
};



