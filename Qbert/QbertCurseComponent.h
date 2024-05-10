#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"

//-----------------------------------------------------
// QbertCurseComponent Class									
//-----------------------------------------------------
enum class MovementState;
enum class MovementDirection;
enum class Character;
class QbertComponent;
class QbertCurseComponent final : public dae::BaseComponent, public dae::Observer<int>
{
public:
	QbertCurseComponent(dae::GameObject* pGameObject);				// Constructor
	~QbertCurseComponent();				// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	QbertCurseComponent(const QbertCurseComponent& other) = delete;
	QbertCurseComponent(QbertCurseComponent&& other) noexcept = delete;
	QbertCurseComponent& operator=(const QbertCurseComponent& other) = delete;
	QbertCurseComponent& operator=(QbertCurseComponent&& other)	noexcept = delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Init() override;
	virtual void Update() override;

	void AddObserver(dae::Subject<int>* pPlayerDiedSubject);

	virtual void Notify(int) override;
	virtual void SubjectDestroyed(dae::Subject<int>* pSubject) override;


private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::Subject<int>* m_pPlayerDied{ nullptr };
	float m_AccumSec{ 0.f };
	float m_CurseDuration{ 1.f };

};


 
