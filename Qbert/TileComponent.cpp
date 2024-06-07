#include "TileComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include <iostream>

TileComponent::TileComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent) 
    : BaseComponent(pGameObject)
    , m_pLevelManagerComponent{ pLevelManagerComponent }
    , m_pTileChangedSubject{ pLevelManagerComponent->TileChanged.get() }
    , m_pGameResumedSubject{ pLevelManagerComponent->GameResumed.get() }
{
    m_pTileChangedSubject->AddObserver(this);
    m_pGameResumedSubject->AddObserver(this);
}

TileComponent::~TileComponent()
{
    if (m_pTileChangedSubject)
        m_pTileChangedSubject->RemoveObserver(this);
    if (m_pGameResumedSubject)
        m_pGameResumedSubject->RemoveObserver(this);
}

void TileComponent::Init()
{
    m_pSpritesheetComponent = GetGameObject()->GetComponent<dae::SpritesheetComponent>();
    assert(m_pSpritesheetComponent);

    GetGameObject()->SetParent(nullptr, false);
}

void TileComponent::Update()
{
    if (!m_RoundOverAnimationEnabled || !m_pLevelManagerComponent->IsGamePaused())
        return;

    m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

    float secondsPerFrame{ 1.f / m_Fps };
    if (m_AccumSec > secondsPerFrame)
    {
        m_AccumSec -= secondsPerFrame;
        m_pSpritesheetComponent->MoveSourceRectRelative(0, 1);
    }
}

void TileComponent::Notify(Character, bool roundFinished)
{
    if (roundFinished)
    {
        m_pSpritesheetComponent->MoveSourceRect(m_pLevelManagerComponent->GetRoundNr() + m_pLevelManagerComponent->GetLevelNr() - 2, m_MaxTileStage);
        m_RoundOverAnimationEnabled = true;
    }
}

void TileComponent::SubjectDestroyed(dae::Subject<Character, bool>* pSubject)
{
    if (pSubject == m_pTileChangedSubject)
        m_pTileChangedSubject = nullptr;
}

void TileComponent::Notify(GameState gameState)
{
    switch (gameState)
    {
    case GameState::NextRound:
        m_TileStage = 0;
        m_pSpritesheetComponent->MoveSourceRect(m_pLevelManagerComponent->GetRoundNr() + m_pLevelManagerComponent->GetLevelNr() - 2, m_TileStage);
        break;
    case GameState::NextLevel:
        m_MaxTileStage = m_pLevelManagerComponent->GetLevelNr() == 2 ? 2 : 1;
        break;
    default:
        break;
    }
    m_RoundOverAnimationEnabled = false;
}

void TileComponent::SubjectDestroyed(dae::Subject<GameState>* pSubject)
{
    if (pSubject == m_pGameResumedSubject)
        m_pGameResumedSubject = nullptr;
}

bool TileComponent::ChangeTile(int& tilesCovered, int stageChange)
{
    if (m_pLevelManagerComponent->GetLevelNr() == 3 && m_TileStage + stageChange > m_MaxTileStage)
        stageChange *= -1;

    int newTileStage = m_TileStage + stageChange;

    if (newTileStage > m_MaxTileStage || newTileStage < 0)
        return false;

    tilesCovered += stageChange;

    m_TileStage = newTileStage;

    m_pSpritesheetComponent->MoveSourceRect(m_pLevelManagerComponent->GetRoundNr() + m_pLevelManagerComponent->GetLevelNr() - 2, m_TileStage);
    return stageChange > 0; 
}
