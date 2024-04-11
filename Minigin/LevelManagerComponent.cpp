//---------------------------
// Includes
//---------------------------
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "GraphicsComponent.h"
#include "TileSpritesheetComponent.h"
#include "TileComponent.h"
#include <algorithm>

//---------------------------
// Constructor & Destructor
//---------------------------
dae::LevelManagerComponent::LevelManagerComponent(GameObject* pGameObject, Scene& scene) : BaseComponent(pGameObject)
{
	//m_vTiles.resize(static_cast<size_t>(m_LevelLength * (m_LevelLength + 1) / 2));
    std::vector<std::vector<GameObject*>> vTiles{};
    glm::vec3 startPos{ 300, 200, 0 };

    for (int i{}; i < m_LevelLength; ++i)
    {
        std::vector<GameObject*> vNewTiles{};
        for (int j{}; j < m_LevelLength - i; ++j)
        {
            auto tile = std::make_unique<dae::GameObject>();
            tile->AddComponent<dae::GraphicsComponent>("Qbert Cubes.png");
            tile->AddComponent<dae::TileSpritesheetComponent>(6, 3);
            tile->AddComponent<dae::TileComponent>();

            tile->SetPosition(startPos.x + 16.f * (j - i), startPos.y + 24.f * (i + j));
            vNewTiles.emplace_back(scene.Add(std::move(tile)));
        }
        vTiles.emplace_back(vNewTiles);
    }

    for (int i{}; i < vTiles.size(); ++i)
        for (int j{}; j < vTiles[i].size(); ++j)
            vTiles[i][j]->GetComponent<TileComponent>()->SetNeighboringTiles(GetAdjacentTiles(vTiles, i, j));
}

void dae::LevelManagerComponent::Init()
{
}

void dae::LevelManagerComponent::Update()
{
}

std::vector<dae::TileComponent*> dae::LevelManagerComponent::GetAdjacentTiles(const std::vector<std::vector<GameObject*>>& vTiles, size_t row, size_t col) const
{
    std::vector<TileComponent*> vAdjacentTiles{};

    vAdjacentTiles.push_back((row > 0)                                            ? vTiles[row - 1][col]->GetComponent<TileComponent>() : nullptr);
    vAdjacentTiles.push_back((row < m_LevelLength - 1 && col < m_LevelLength - (row + 1)) ? vTiles[row + 1][col]->GetComponent<TileComponent>() : nullptr);
    vAdjacentTiles.push_back((col > 0)                                            ? vTiles[row][col - 1]->GetComponent<TileComponent>() : nullptr);
    vAdjacentTiles.push_back((col < m_LevelLength - 1 && row < m_LevelLength - (col + 1)) ? vTiles[row][col + 1]->GetComponent<TileComponent>() : nullptr);

    return vAdjacentTiles;
}

//---------------------------
// Member functions
//---------------------------

// write member functions here




