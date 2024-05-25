#pragma once
#include "CharacterComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

class SlickSamComponent final : public CharacterComponent, public dae::Observer<Character, MovementInfo>
{
public:
	SlickSamComponent(dae::GameObject* pGameObject, Character character, LevelManagerComponent* pLevelManagerComponent);
	virtual ~SlickSamComponent() override;

	SlickSamComponent(const SlickSamComponent& other) = delete;
	SlickSamComponent(SlickSamComponent&& other) noexcept = delete;
	SlickSamComponent& operator=(const SlickSamComponent& other) = delete;
	SlickSamComponent& operator=(SlickSamComponent&& other) noexcept = delete;

	virtual void Init() override;

	virtual void Notify(Character character, MovementInfo movementInfo) override;
};

