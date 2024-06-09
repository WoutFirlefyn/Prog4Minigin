#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class LevelManagerComponent;
class CharacterComponent;
enum class GameState;
enum class Character;
struct MovementInfo;
class LivesComponent final : public dae::BaseComponent, public dae::Observer<GameState>, public dae::Observer<Character, MovementInfo>
{
public:
	LivesComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent, CharacterComponent* pCharacterComponent);
	virtual ~LivesComponent() override;	

	LivesComponent(const LivesComponent& other) = delete;
	LivesComponent(LivesComponent&& other) noexcept = delete;
	LivesComponent& operator=(const LivesComponent& other) = delete;
	LivesComponent& operator=(LivesComponent&& other) noexcept = delete;

	virtual void Init() override;

	virtual void Notify(Character character, MovementInfo movementInfo) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject) override;

	virtual void Notify(GameState gameState) override;
	virtual void SubjectDestroyed(dae::Subject<GameState>* pSubject) override;
private:
	dae::Subject<Character, MovementInfo>* m_pMoveStateChanged{ nullptr };
	dae::Subject<GameState>* m_pGameResumedSubject{ nullptr };
	std::vector<dae::GameObject*> m_vHearts;
	Character m_Character{};
	int m_Lives{ 3 };
};



