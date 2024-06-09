#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <glm/glm.hpp>

struct MovementInfo;
enum class Character;
class CharacterComponent;
class QbertCurseComponent final : public dae::BaseComponent, public dae::Observer<Character, MovementInfo>
{
public:
	QbertCurseComponent(dae::GameObject* pGameObject, CharacterComponent* pCharacterComponent);
	virtual ~QbertCurseComponent() override;

	QbertCurseComponent(const QbertCurseComponent& other) = delete;
	QbertCurseComponent(QbertCurseComponent&& other) noexcept = delete;
	QbertCurseComponent& operator=(const QbertCurseComponent& other) = delete;
	QbertCurseComponent& operator=(QbertCurseComponent&& other)	noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;

	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject) override;
private:
	dae::Subject<Character, MovementInfo>* m_pMoveStateChangedSubject{ nullptr };
	Character m_Character{};
	float m_AccumSec{ 0.f };
	float m_CurseDuration{ 2.5f };
	const glm::vec3 m_PosOffset{ -18.f, -28.f, 0.f };
};


 
