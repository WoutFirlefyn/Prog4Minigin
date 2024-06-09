#pragma once
#include "CharacterComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

class CoilyComponent final : public CharacterComponent, public dae::Observer<Character, MovementInfo>, public dae::Observer<Character, dae::GameObject*>
{
public:
	CoilyComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~CoilyComponent() override;

	CoilyComponent(const CoilyComponent& other) = delete;
	CoilyComponent(CoilyComponent&& other) noexcept = delete;
	CoilyComponent& operator=(const CoilyComponent& other) = delete;
	CoilyComponent& operator=(CoilyComponent&& other) noexcept = delete;

	virtual void Init() override;
	virtual void LateUpdate() override;

	// Move State Changed
	virtual void Notify(Character character, MovementInfo movementInfo) override;

	// Character Spawned
	virtual void Notify(Character character, dae::GameObject* pCharacterGameObject) override;

	bool IsEgg() const { return m_IsEgg; }
private:
	bool m_IsEgg{ true };
	int m_AmountOfJumps{ 0 };
};



