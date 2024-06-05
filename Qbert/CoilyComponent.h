#pragma once
#include "CharacterComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

class CoilyComponent final : public CharacterComponent, public dae::Observer<Character, MovementInfo>, public dae::Observer<Character, dae::GameObject*>
{
public:
	CoilyComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent, bool versusModeEnabled = false);
	virtual ~CoilyComponent() override;

	CoilyComponent(const CoilyComponent& other) = delete;
	CoilyComponent(CoilyComponent&& other) noexcept = delete;
	CoilyComponent& operator=(const CoilyComponent& other) = delete;
	CoilyComponent& operator=(CoilyComponent&& other) noexcept = delete;

	virtual void Init() override;
	virtual void LateUpdate() override;

	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void Notify(Character character, dae::GameObject* pCharacterGameObject) override;

	bool IsEgg() const { return m_IsEgg; }
	bool VersusModeEnabled() const { return m_VersusMode; }
private:
	bool m_IsEgg{ true };
	int m_AmountOfJumps{ 0 };
	bool m_VersusMode{ false };
};



