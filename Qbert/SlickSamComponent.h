#pragma once
#include "CharacterComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

class SlickSamComponent final : public CharacterComponent
{
public:
	SlickSamComponent(dae::GameObject* pGameObject, Character character);	// Constructor
	virtual ~SlickSamComponent() override = default;				// Destructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	SlickSamComponent(const SlickSamComponent& other) = delete;
	SlickSamComponent(SlickSamComponent&& other) noexcept = delete;
	SlickSamComponent& operator=(const SlickSamComponent& other) = delete;
	SlickSamComponent& operator=(SlickSamComponent&& other) noexcept = delete;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	virtual void Init() override;

	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void Notify(Character character) override;
};

