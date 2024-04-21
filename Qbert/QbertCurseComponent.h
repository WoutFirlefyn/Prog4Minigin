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
class QbertCurseComponent final : public dae::BaseComponent, public dae::Observer<Character>
{
public:
	QbertCurseComponent(dae::GameObject* pGameObject, dae::Subject<Character>* pSubject);				// Constructor
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

	virtual void Notify(Character character) override;
	virtual void SubjectDestroyed(dae::Subject<Character>* pSubject) override;


private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------


	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	dae::Subject<Character>* m_pCharacterFellSubject;
	float m_AccumSec{ 0.f };
	float m_CurseDuration{ 1.f };

};


 
