#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <glm/glm.hpp>

enum class MovementState;
enum class MovementDirection;
enum class Character;
class QbertComponent;
class QbertCurseComponent final : public dae::BaseComponent, public dae::Observer<>
{
public:
	QbertCurseComponent(dae::GameObject* pGameObject);
	virtual ~QbertCurseComponent() override;

	QbertCurseComponent(const QbertCurseComponent& other) = delete;
	QbertCurseComponent(QbertCurseComponent&& other) noexcept = delete;
	QbertCurseComponent& operator=(const QbertCurseComponent& other) = delete;
	QbertCurseComponent& operator=(QbertCurseComponent&& other)	noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;

	void AddObserver(dae::Subject<>* pPlayerDiedSubject);

	virtual void Notify() override;
	virtual void SubjectDestroyed(dae::Subject<>* pSubject) override;
private:
	void QbertDied();

	dae::Subject<>* m_pPlayerDied{ nullptr };
	Character m_Character{};
	float m_AccumSec{ 0.f };
	float m_CurseDuration{ 2.5f };
	const glm::vec3 m_PosOffset{ -18.f, -28.f, 0.f };
};


 
