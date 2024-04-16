//---------------------------
// Includes
//---------------------------
#include "TileComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "GameObject.h"
#include <iostream>
#include <optional>

int dae::TileComponent::m_MaxTileStage{ 1 };
int dae::TileComponent::m_TileCount{ 0 };
//---------------------------
// Constructor & Destructor
//---------------------------
dae::TileComponent::TileComponent(GameObject* pGameObject) : BaseComponent(pGameObject)
    , m_TileId{ m_TileCount++ }
{
}

void dae::TileComponent::SetNeighboringTiles(const std::vector<std::vector<TileComponent*>>& vTiles, size_t row, size_t col)
{
    m_vNeighboringTiles.push_back((row > 0) ? vTiles[row - 1][col]->GetGameObject() : nullptr);
    m_vNeighboringTiles.push_back((col > 0) ? vTiles[row][col - 1]->GetGameObject() : nullptr);
    m_vNeighboringTiles.push_back((row < 7 - (col + 1)) ? vTiles[row][col + 1]->GetGameObject() : nullptr);
    m_vNeighboringTiles.push_back((col < 7 - (row + 1)) ? vTiles[row + 1][col]->GetGameObject() : nullptr);
}

bool dae::TileComponent::IsEdgeTile() const
{
    return std::any_of(std::execution::par_unseq, m_vNeighboringTiles.begin(), m_vNeighboringTiles.end(), [](auto pTile)
        {
            return pTile == nullptr;
        });
}

bool dae::TileComponent::IsCharacterHere(Character character)
{
    return m_CharactersHere.contains(character);
}

void dae::TileComponent::GetCharacter(std::pair<dae::Character, dae::GameObject*>& character)
{
    auto characterNode = m_CharactersHere.extract(character.first);
    if (characterNode)
        character.second = characterNode.mapped();
}

void dae::TileComponent::MoveCharacterHere(const std::pair<Character, GameObject*>& character)
{
    m_CharactersHere.insert(character);
}

bool dae::TileComponent::ChangeTile(int currentRound)
{
    if (m_TileStage == m_MaxTileStage)
        return false;

    GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(currentRound, ++m_TileStage);
    return true; 
}

void dae::TileComponent::Reset(int currentRound)
{
    m_TileStage = 0;
    GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(currentRound, m_TileStage);
    m_CharactersHere.clear();
}




