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

int TileComponent::m_MaxTileStage{ 1 };
int TileComponent::m_TileCount{ 0 };
//---------------------------
// Constructor & Destructor
//---------------------------
TileComponent::TileComponent(dae::GameObject* pGameObject) : BaseComponent(pGameObject)
    , m_TileId{ m_TileCount++ }
{
}

void TileComponent::SetNeighboringTiles(const std::vector<std::vector<TileComponent*>>& vTiles, size_t row, size_t col)
{
    const size_t levelLength = vTiles.size();

    m_vNeighboringTiles.push_back((row > 0) ? vTiles[row - 1][col]->GetGameObject() : nullptr);
    m_vNeighboringTiles.push_back((col > 0) ? vTiles[row][col - 1]->GetGameObject() : nullptr);
    m_vNeighboringTiles.push_back((row < levelLength - (col + 1)) ? vTiles[row][col + 1]->GetGameObject() : nullptr);
    m_vNeighboringTiles.push_back((col < levelLength - (row + 1)) ? vTiles[row + 1][col]->GetGameObject() : nullptr);
}

bool TileComponent::IsEdgeTile() const
{
    return std::any_of(std::execution::par_unseq, m_vNeighboringTiles.begin(), m_vNeighboringTiles.end(), [](auto pTile)
        {
            return pTile == nullptr;
        });
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
    if (newTileStage > m_MaxTileStage || newTileStage < 0 || stageChange == 0)
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

void TileComponent::AddDiskAsNeighbor(dae::GameObject* pDisk)
{
    MovementDirection direction{};
    if (std::count(std::execution::par_unseq, m_vNeighboringTiles.begin(), m_vNeighboringTiles.end(), nullptr) == 2)
        direction = static_cast<MovementDirection>(rand() % 2);
    else
    {
        auto it = std::find(std::execution::par_unseq, m_vNeighboringTiles.begin(), m_vNeighboringTiles.end(), nullptr);
        direction = static_cast<MovementDirection>(std::distance(m_vNeighboringTiles.begin(), it));
    }

    glm::vec3 offset{};
    switch (direction)
    {
    case MovementDirection::Up:
        offset += glm::vec3{ 16, -24, 0 };
        break;
    case MovementDirection::Left:
        offset += glm::vec3{ -16, -24, 0 };
        break;
    default:
        return;
    }
    pDisk->SetParent(GetGameObject());
    pDisk->SetPosition(pDisk->GetLocalPosition() + offset);
    pDisk->SetParent(nullptr, true);

    m_vNeighboringTiles[static_cast<size_t>(direction)] = pDisk;
}




