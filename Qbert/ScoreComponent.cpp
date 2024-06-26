#include "ScoreComponent.h"
#include "TextComponent.h"
#include "QbertComponent.h"
#include "TileComponent.h"
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "Game.h"

ScoreComponent::ScoreComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent, Character character)
	: BaseComponent(pGameObject)
	, m_pLevelManagerComponent{ pLevelManagerComponent }
	, m_pTileChangedSubject{ pLevelManagerComponent->TileChanged.get() }
	, m_pCharactersCollideSubject{ pLevelManagerComponent->CharactersCollide.get() }
	, m_pMoveStateChangedSubject { CharacterComponent::MoveStateChanged.get() }
	, m_pGameResumedSubject { pLevelManagerComponent->GameResumed.get() }
	, m_CharacterToTrack{ character }
{
}

ScoreComponent::~ScoreComponent()
{
	if (m_pTileChangedSubject)
		m_pTileChangedSubject->RemoveObserver(this);
	if (m_pCharactersCollideSubject)
		m_pCharactersCollideSubject->RemoveObserver(this);
	if (m_pMoveStateChangedSubject)
		m_pMoveStateChangedSubject->RemoveObserver(this);
	if (m_pGameResumedSubject)
		m_pGameResumedSubject->RemoveObserver(this);
}

void ScoreComponent::Init()
{
	m_pTileChangedSubject->AddObserver(this);
	m_pCharactersCollideSubject->AddObserver(this);
	m_pMoveStateChangedSubject->AddObserver(this);
	m_pGameResumedSubject->AddObserver(this);
	m_pTextComponent = GetGameObject()->GetComponent<dae::TextComponent>();
	UpdateScore();
}

void ScoreComponent::Notify(Character character, bool roundFinished)
{
	if (character == m_CharacterToTrack)
		UpdateScore(25);
	if (roundFinished)
		m_AmountOfDisks = m_pLevelManagerComponent->GetAmountOfActiveDisks();
}

void ScoreComponent::SubjectDestroyed(dae::Subject<Character, bool>* pSubject)
{
	if (pSubject == m_pTileChangedSubject)
		m_pTileChangedSubject = nullptr;
}

void ScoreComponent::Notify(Character character, Character otherCharacter)
{
	if (character != m_CharacterToTrack && otherCharacter != m_CharacterToTrack)
		return;

	if (otherCharacter == m_CharacterToTrack)
		std::swap(character, otherCharacter);

	if (otherCharacter == Character::Slick || otherCharacter == Character::Sam)
		UpdateScore(300);
}

void ScoreComponent::SubjectDestroyed(dae::Subject<Character,Character>* pSubject)
{
	if (pSubject == m_pCharactersCollideSubject)
		m_pCharactersCollideSubject = nullptr;
}

void ScoreComponent::Notify(Character character, MovementInfo movementInfo)
{
	if (character == Character::Coily && movementInfo.state == MovementState::Fall)
		UpdateScore(500);
}

void ScoreComponent::SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject)
{
	if (pSubject == m_pMoveStateChangedSubject)
		m_pMoveStateChangedSubject = nullptr;
}

void ScoreComponent::Notify(GameState gameState)
{
	if (gameState == GameState::NextRound)
		UpdateScore(m_AmountOfDisks * 50);
}

void ScoreComponent::SubjectDestroyed(dae::Subject<GameState>* pSubject)
{
	if (pSubject == m_pGameResumedSubject)
		m_pGameResumedSubject = nullptr;
}

void ScoreComponent::UpdateScore(int score)
{
	Game::GetInstance().SaveScore(Game::GetInstance().GetSavedScore() + score);
	m_Score += score;
	if (m_pTextComponent)
		m_pTextComponent->SetText("Score: " + std::to_string(m_Score));
}
