#pragma once
#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class TextComponent;
}
class QbertComponent;
class LevelManagerComponent;
enum class Character;
enum class GameState;
struct MovementInfo;
class ScoreComponent final : public dae::BaseComponent, public dae::Observer<Character, bool>, public dae::Observer<Character, Character>, public dae::Observer<Character, MovementInfo>, public dae::Observer<GameState>
{
public:
	ScoreComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent, Character character = static_cast<Character>(0));
	virtual ~ScoreComponent() override;

	ScoreComponent(const ScoreComponent& other) = delete;
	ScoreComponent(ScoreComponent&& other) noexcept = delete;
	ScoreComponent& operator=(const ScoreComponent& other) = delete;
	ScoreComponent& operator=(ScoreComponent&& other)	noexcept = delete;

	virtual void Init() override;

	// Tile Changed
	virtual void Notify(Character character, bool roundFinished) override;
	virtual void SubjectDestroyed(dae::Subject<Character, bool>* pSubject) override;

	// Characters Collide
	virtual void Notify(Character, Character) override;
	virtual void SubjectDestroyed(dae::Subject<Character, Character>* pSubject) override;

	// Move State Changed
	virtual void Notify(Character, MovementInfo) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject) override;

	// New Round Started
	virtual void Notify(GameState gameState) override;
	virtual void SubjectDestroyed(dae::Subject<GameState>* pSubject)override;

	int GetScore() const { return m_Score; }

private:
	void UpdateScore(int score = 0);

	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };
	dae::TextComponent* m_pTextComponent{ nullptr };

	dae::Subject<Character, bool>* m_pTileChangedSubject{ nullptr };
	dae::Subject<Character,Character>* m_pCharactersCollideSubject{ nullptr };
	dae::Subject<Character, MovementInfo>* m_pMoveStateChangedSubject{ nullptr };
	dae::Subject<GameState>* m_pGameResumedSubject{ nullptr };

	Character m_CharacterToTrack{};
	int m_Score{ 0 };
	int m_AmountOfDisks{ 0 };
};


 
