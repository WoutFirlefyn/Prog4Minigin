#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "CharacterComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

//-----------------------------------------------------
// QbertComponent Class									
//-----------------------------------------------------
class QbertComponent final : public CharacterComponent, public dae::Observer<bool>
{
public:
	QbertComponent(dae::GameObject* pGameObject);	// Constructor
	virtual ~QbertComponent() override;				// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	QbertComponent(const QbertComponent& other) = delete;
	QbertComponent(QbertComponent&& other) noexcept = delete;
	QbertComponent& operator=(const QbertComponent& other) = delete;
	QbertComponent& operator=(QbertComponent&& other) noexcept = delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Init() override;

	void AddObserver(dae::Subject<bool>* pTileChangedSubject, dae::Subject<Character, TileType>* pCharacterStartedJumpingSubject);
	// MoveStateChanged
	virtual void Notify(Character character, MovementState movementState, MovementDirection movementDirection) override;
	// CharacterSpawned
	virtual void Notify(Character character) override;
	// TileChanged
	virtual void Notify(bool roundFinished) override;
	void SubjectDestroyed(dae::Subject<bool>* pSubject);

	int GetLives() const { return m_Lives; }

	std::unique_ptr<dae::Subject<int>> PlayerDied;
private:
	dae::Subject<bool>* m_pTileChangedSubject{ nullptr };
	int m_Lives{ 3 };
};


 
