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

	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void Notify(Character character, dae::GameObject* pCharacterGameObject) override;

	static bool IsEgg() { return m_IsEgg; }
private:
	static bool m_IsEgg;
	int m_AmountOfJumps{ 0 };
};



