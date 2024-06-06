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
    , m_pNewRoundStartedSubject{ pLevelManagerComponent->NewRoundStarted.get() }
{
    m_pTileChangedSubject->AddObserver(this);
    m_pNewRoundStartedSubject->AddObserver(this);
}

TileComponent::~TileComponent()
{
    if (m_pTileChangedSubject)
        m_pTileChangedSubject->RemoveObserver(this);
    if (m_pNewRoundStartedSubject)
        m_pNewRoundStartedSubject->RemoveObserver(this);
}

void TileComponent::Init()
{
    m_pSpritesheetComponent = GetGameObject()->GetComponent<dae::SpritesheetComponent>();
    assert(m_pSpritesheetComponent);

    GetGameObject()->SetParent(nullptr, false);
}

void TileComponent::Update()
{
    if (!m_pLevelManagerComponent->IsRoundOver())
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
        m_pSpritesheetComponent->MoveSourceRect(m_pLevelManagerComponent->GetRoundNr() - 1, m_MaxTileStage);
}

void TileComponent::SubjectDestroyed(dae::Subject<Character, bool>* pSubject)
{
    if (pSubject == m_pTileChangedSubject)
        m_pTileChangedSubject = nullptr;
}

void TileComponent::Notify(bool nextLevel)
{
    if (nextLevel)
    {
        m_MaxTileStage = m_pLevelManagerComponent->GetLevelNr() == 2 ? 2 : 1;
        return;
    }
    m_TileStage = 0;
    m_pSpritesheetComponent->MoveSourceRect(m_pLevelManagerComponent->GetRoundNr() - 1, m_TileStage);
}

void TileComponent::SubjectDestroyed(dae::Subject<bool>* pSubject)
{
    if (pSubject == m_pNewRoundStartedSubject)
        m_pNewRoundStartedSubject = nullptr;
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

    m_pSpritesheetComponent->MoveSourceRect(m_pLevelManagerComponent->GetRoundNr() - 1, m_TileStage);
    return stageChange > 0; 
}
