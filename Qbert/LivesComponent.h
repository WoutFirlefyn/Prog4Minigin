#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class QbertComponent;
class LivesComponent final : public dae::BaseComponent, public dae::Observer<>
{
public:
	LivesComponent(dae::GameObject* pGameObject);
	virtual ~LivesComponent() override;	

	LivesComponent(const LivesComponent& other) = delete;
	LivesComponent(LivesComponent&& other) noexcept = delete;
	LivesComponent& operator=(const LivesComponent& other) = delete;
	LivesComponent& operator=(LivesComponent&& other) noexcept = delete;

	virtual void Init() override;

	void AddObserver(dae::Subject<>* pPlayerDiedSubject);

	virtual void Notify() override;
	virtual void SubjectDestroyed(dae::Subject<>* pSubject) override;
private:
	dae::Subject<>* m_pPlayerDiedSubject{ nullptr };
	static int m_CurrentId;
	const int m_HeartId;
};



