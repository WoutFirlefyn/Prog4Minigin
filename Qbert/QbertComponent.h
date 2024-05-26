#pragma once
#include "CharacterComponent.h"
#include "Observer.h"
#include <memory>
#include <glm/glm.hpp>

class QbertComponent final : public CharacterComponent, public dae::Observer<Character, MovementInfo>/*, public dae::Observer<bool>*/
{
public:
	QbertComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~QbertComponent() override;				

	QbertComponent(const QbertComponent& other) = delete;
	QbertComponent(QbertComponent&& other) noexcept = delete;
	QbertComponent& operator=(const QbertComponent& other) = delete;
	QbertComponent& operator=(QbertComponent&& other) noexcept = delete;

	virtual void Init() override;

	void AddObserver(dae::Subject<bool>* pTileChangedSubject);
	// MoveStateChanged
	virtual void Notify(Character character, MovementInfo movementInfo) override;
	// TileChanged
	virtual void Notify(bool roundFinished) override;
	//virtual void Notify(bool roundFinished) override;
	//void SubjectDestroyed(dae::Subject<bool>* pSubject);

	int GetLives() const { return m_Lives; }

	std::unique_ptr<dae::Subject<>> PlayerDied;
private:
	//dae::Subject<bool>* m_pTileChangedSubject{ nullptr };
	int m_Lives{ 3 };
};


 
