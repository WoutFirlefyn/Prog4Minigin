#include "TileComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include <iostream>
#include <optional>

int TileComponent::m_MaxTileStage{ 1 };
TileComponent::TileComponent(dae::GameObject* pGameObject) : BaseComponent(pGameObject)
{
}

void TileComponent::Init()
{
    GetGameObject()->SetParent(nullptr, false);
}

bool TileComponent::ChangeTile(int& tilesCovered, int stageChange)
{
    int newTileStage{ m_TileStage + stageChange };
    if (newTileStage > m_MaxTileStage || newTileStage < 0)
        return false;

    tilesCovered += stageChange;

    m_TileStage = newTileStage;

    GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(LevelManagerComponent::GetRoundNr(), m_TileStage);
    return stageChange > 0; 
}

void TileComponent::Reset()
{
    m_TileStage = 0;
    GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(LevelManagerComponent::GetRoundNr(), m_TileStage);
}
