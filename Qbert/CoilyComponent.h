#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "CharacterComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

class CoilyComponent final : public CharacterComponent
{
public:
	CoilyComponent(dae::GameObject* pGameObject);	// Constructor
	virtual ~CoilyComponent() override = default;				// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	CoilyComponent(const CoilyComponent& other) = delete;
	CoilyComponent(CoilyComponent&& other) noexcept = delete;
	CoilyComponent& operator=(const CoilyComponent& other) = delete;
	CoilyComponent& operator=(CoilyComponent&& other) noexcept = delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Init() override;

	void AddObserver(dae::Subject<Character, TileType>* pCharacterStartedJumpingSubject);
	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void Notify(Character character) override;

	static bool IsEgg() { return m_IsEgg; }
private:
	static bool m_IsEgg;
	int m_AmountOfJumps{ 0 };
};



