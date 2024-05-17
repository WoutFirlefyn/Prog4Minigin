#include "TileComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "GameObject.h"
#include <iostream>
#include <optional>

int TileComponent::m_MaxTileStage{ 1 };
int TileComponent::m_TileCount{ 0 };
TileComponent::TileComponent(dae::GameObject* pGameObject) : BaseComponent(pGameObject)
    , m_TileId{ m_TileCount++ }
{
}

void TileComponent::Init()
{
    GetGameObject()->SetParent(nullptr, false);
}

bool TileComponent::IsCharacterHere(Character character)
{
    return m_CharactersHere.contains(character);
}

std::pair<Character, dae::GameObject*> TileComponent::GetCharacter(Character character)
{
    auto characterNode = m_CharactersHere.extract(character);
    if (characterNode)
        return std::make_pair(character, characterNode.mapped());
    return std::make_pair(character, nullptr);
}

void TileComponent::MoveCharacterHere(const std::pair<Character, dae::GameObject*>& character)
{
    m_CharactersHere.insert(character);
}

bool TileComponent::ChangeTile(int currentRound, int& tilesCovered, int stageChange)
{
    int newTileStage{ m_TileStage + stageChange };
    if (newTileStage > m_MaxTileStage || newTileStage < 0)
        return false;

    tilesCovered += stageChange;

    m_TileStage = newTileStage;

    GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(currentRound, m_TileStage);
    return stageChange > 0; 
}

void TileComponent::Reset(int currentRound)
{
    m_TileStage = 0;
    GetGameObject()->GetComponent<dae::SpritesheetComponent>()->MoveSourceRect(currentRound, m_TileStage);
    m_CharactersHere.clear();
}
