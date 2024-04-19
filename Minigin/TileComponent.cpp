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

std::pair<dae::Character, dae::GameObject*> dae::TileComponent::GetCharacter(Character character)
{
    auto characterNode = m_CharactersHere.extract(character);
    if (characterNode)
        return std::make_pair(characterNode.key(), characterNode.mapped());
    return std::make_pair(character, nullptr);
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

void dae::TileComponent::AddDiskAsNeighbor(GameObject* pDisk)
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




