#pragma once
#include "CharacterComponent.h"
#include "Observer.h"
class UggWrongwayComponent : public CharacterComponent, public dae::Observer<Character, MovementInfo>
{
public:
	UggWrongwayComponent(dae::GameObject* pGameObject, Character character);
	virtual ~UggWrongwayComponent() override;

	UggWrongwayComponent(const UggWrongwayComponent& other) = delete;
	UggWrongwayComponent(UggWrongwayComponent&& other) noexcept = delete;
	UggWrongwayComponent& operator=(const UggWrongwayComponent& other) = delete;
	UggWrongwayComponent& operator=(UggWrongwayComponent&& other) noexcept = delete;

	virtual void Init() override;

	virtual void Notify(Character character, MovementInfo movementInfo) override;

	virtual glm::ivec2 GetSpawnPosition() const override;

	bool HasSpawnedLeft() const { return m_SpawnedLeft; }
private:
	bool m_SpawnedLeft{};
};

