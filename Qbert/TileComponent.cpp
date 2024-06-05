#include "TileComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include <iostream>

int TileComponent::m_MaxTileStage{ 1 };
TileComponent::TileComponent(dae::GameObject* pGameObject, LevelManagerComponent* pLevelManagerComponent) : BaseComponent(pGameObject)
{
    m_pTileChanged = pLevelManagerComponent->TileChanged.get();
    m_pTileChanged->AddObserver(this);
    m_pNewRoundStarted = pLevelManagerComponent->NewRoundStarted.get();
    m_pNewRoundStarted->AddObserver(this);
}

TileComponent::~TileComponent()
{
    if (m_pTileChanged)
        m_pTileChanged->RemoveObserver(this);
    if (m_pNewRoundStarted)
        m_pNewRoundStarted->RemoveObserver(this);
}

void TileComponent::Init()
{
    m_pSpritesheetComponent = GetGameObject()->GetComponent<dae::SpritesheetComponent>();
    assert(m_pSpritesheetComponent);

    GetGameObject()->SetParent(nullptr, false);
}

void TileComponent::Update()
{
    if (!LevelManagerComponent::IsRoundOver())
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
        m_pSpritesheetComponent->MoveSourceRect(LevelManagerComponent::GetRoundNr(), m_MaxTileStage);
}

void TileComponent::SubjectDestroyed(dae::Subject<Character, bool>* pSubject)
{
    if (pSubject == m_pTileChanged)
        m_pTileChanged = nullptr;
}

void TileComponent::Notify()
{
    m_TileStage = 0;
    m_pSpritesheetComponent->MoveSourceRect(LevelManagerComponent::GetRoundNr(), m_TileStage);
}

void TileComponent::SubjectDestroyed(dae::Subject<>* pSubject)
{
    if (pSubject == m_pNewRoundStarted)
        m_pNewRoundStarted = nullptr;
}

bool TileComponent::ChangeTile(int& tilesCovered, int stageChange)
{
    int newTileStage{ m_TileStage + stageChange };
    if (newTileStage > m_MaxTileStage || newTileStage < 0)
        return false;

    tilesCovered += stageChange;

    m_TileStage = newTileStage;

    m_pSpritesheetComponent->MoveSourceRect(LevelManagerComponent::GetRoundNr(), m_TileStage);
    return stageChange > 0; 
}
