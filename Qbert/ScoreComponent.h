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
struct MovementInfo;
class ScoreComponent final : public dae::BaseComponent, public dae::Observer<bool>, public dae::Observer<Character, Character>, public dae::Observer<Character, MovementInfo>, public dae::Observer<>
{
public:
	ScoreComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent);
	virtual ~ScoreComponent() override;

	ScoreComponent(const ScoreComponent& other) = delete;
	ScoreComponent(ScoreComponent&& other) noexcept = delete;
	ScoreComponent& operator=(const ScoreComponent& other) = delete;
	ScoreComponent& operator=(ScoreComponent&& other)	noexcept = delete;

	virtual void Init() override;

	// Tile Changed
	virtual void Notify(bool roundFinished) override;
	virtual void SubjectDestroyed(dae::Subject<bool>* pSubject) override;

	// Characters Collide
	virtual void Notify(Character, Character) override;
	virtual void SubjectDestroyed(dae::Subject<Character, Character>* pSubject) override;

	// Move State Changed
	virtual void Notify(Character, MovementInfo) override;
	virtual void SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject) override;

	virtual void Notify() override;
	virtual void SubjectDestroyed(dae::Subject<>* pSubject)override;

	int GetScore() const { return m_Score; }

private:
	void UpdateScore(int score = 0);

	LevelManagerComponent* m_pLevelManagerComponent{ nullptr };
	dae::TextComponent* m_pTextComponent{ nullptr };

	dae::Subject<bool>* m_pTileChangedSubject{ nullptr };
	dae::Subject<Character,Character>* m_pCharactersCollideSubject{ nullptr };
	dae::Subject<Character, MovementInfo>* m_pMoveStateChangedSubject{ nullptr };
	dae::Subject<>* m_pNewRoundStartedSubject{ nullptr };

	Character m_CharacterToTrack{};
	int m_Score{ 0 };
	int m_AmountOfDisks{ 0 };
};


 
