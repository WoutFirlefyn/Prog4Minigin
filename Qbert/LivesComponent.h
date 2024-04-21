#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// QbertComponent Class									
//-----------------------------------------------------
class QbertComponent;
class LivesComponent final : public dae::BaseComponent, public dae::Observer<int>
{
public:
	LivesComponent(dae::GameObject* pGameObject);				// Constructor
	virtual ~LivesComponent() override;			// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	LivesComponent(const LivesComponent& other) = delete;
	LivesComponent(LivesComponent&& other) noexcept = delete;
	LivesComponent& operator=(const LivesComponent& other) = delete;
	LivesComponent& operator=(LivesComponent&& other) noexcept = delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Init() override;

	void AddObserver(dae::Subject<int>* pPlayerDiedSubject);

	virtual void Notify(int currentLives) override;
	virtual void SubjectDestroyed(dae::Subject<int>* pSubject) override;

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::Subject<int>* m_pPlayerDiedSubject{ nullptr };
	static int m_CurrentId;
	const int m_HeartId;
};



