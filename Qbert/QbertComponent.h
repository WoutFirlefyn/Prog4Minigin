#pragma once
#include "CharacterComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

class QbertComponent final : public CharacterComponent, public dae::Observer<Character, MovementInfo>
{
public:
	QbertComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent, Character character = Character::Qbert1);
	virtual ~QbertComponent() override;				

	QbertComponent(const QbertComponent& other) = delete;
	QbertComponent(QbertComponent&& other) noexcept = delete;
	QbertComponent& operator=(const QbertComponent& other) = delete;
	QbertComponent& operator=(QbertComponent&& other) noexcept = delete;

	virtual void Init() override;
	virtual void LateUpdate() override;
	
	// MoveStateChanged
	virtual void Notify(Character character, MovementInfo movementInfo) override;

	int GetLives() const { return m_Lives; }

	std::unique_ptr<dae::Subject<>> PlayerDied;
private:
	int m_Lives{ 3 };
	bool m_OtherQbertDied{ false };
};


 
