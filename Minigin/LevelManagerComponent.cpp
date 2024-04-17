//---------------------------
// Includes
//---------------------------
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "TileComponent.h"
#include "DiskComponent.h"
#include <algorithm>
#include <iterator>
#include <random>

//---------------------------
// Constructor & Destructor
//---------------------------
dae::LevelManagerComponent::LevelManagerComponent(GameObject* pGameObject, QbertComponent* pQbertComponent, Scene& scene) : BaseComponent(pGameObject)
    , m_pQbertComponent{ pQbertComponent }
{
    TileChanged = std::make_unique<Subject<bool>>();

    glm::vec3 startPos{ 300, 200, 0 };
    std::vector<std::vector<TileComponent*>> v2DTiles{};

    for (int i{}; i < m_LevelLength; ++i)
    {
        std::vector<TileComponent*> vNewTiles{};
        for (int j{}; j < m_LevelLength - i; ++j)
        {
            auto tile = std::make_unique<dae::GameObject>();
            tile->AddComponent<GraphicsComponent>("Qbert Cubes.png");
            tile->AddComponent<SpritesheetComponent>(6, 3);
            tile->AddComponent<TileComponent>();
            vNewTiles.emplace_back(tile->GetComponent<TileComponent>());

            tile->SetPosition(startPos.x + 16.f * (j - i), startPos.y + 24.f * (i + j));
            m_vTiles.emplace_back(scene.Add(std::move(tile)));
        }
        v2DTiles.emplace_back(vNewTiles);
    }

    for (int i{}; i < v2DTiles.size(); ++i)
        for (int j{}; j < v2DTiles[i].size(); ++j)
            v2DTiles[i][j]->SetNeighboringTiles(v2DTiles, i, j);

    std::vector<TileComponent*> vEdgeTiles{};
    std::copy(v2DTiles[0].begin(), v2DTiles[0].end(), std::back_inserter(vEdgeTiles));
    std::transform(v2DTiles.begin(), v2DTiles.end(), std::back_inserter(vEdgeTiles),
        [](const std::vector<TileComponent*>& row) 
        { 
            return row[0]; 
        });

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vEdgeTiles.begin(), vEdgeTiles.end(), g);

    for (int i{}; i < m_AmountOfDisks; ++i)
    {
        auto disk = std::make_unique<GameObject>();
        disk->AddComponent<GraphicsComponent>("Disk Spritesheet.png");
        disk->AddComponent<SpritesheetComponent>(30, 1);
        disk->AddComponent<DiskComponent>(m_pQbertComponent);
        disk->SetPosition(8, 14);
       
        vEdgeTiles[i]->AddDiskAsNeighbor(scene.Add(std::move(disk)));
    }
}

dae::LevelManagerComponent::~LevelManagerComponent()
{
    if (m_pQbertComponent)
        m_pQbertComponent->MoveStateChanged->RemoveObserver(this);
    TileChanged->RemoveObserver(this);
}

void dae::LevelManagerComponent::Init()
{
    m_pQbertComponent->MoveStateChanged->AddObserver(this);
    TileChanged->AddObserver(this);

    m_vTiles[0]->GetComponent<TileComponent>()->MoveCharacterHere(std::make_pair(Character::Qbert1, m_pQbertComponent->GetGameObject()));
}

void dae::LevelManagerComponent::Notify(Character character, MovementState movementState, MovementDirection movementDirection)
{
    auto pCurrentTile = FindCharacter(character);
    assert(pCurrentTile && "LevelManagerComponent: Character not found");

    switch (movementState)
    {
    case MovementState::Start:
    {
        auto pNextTile = pCurrentTile->GetComponent<TileComponent>()->GetNeighboringTile(movementDirection);
        if (pNextTile)
        {
            auto characterObject = pCurrentTile->GetComponent<TileComponent>()->GetCharacter(character);

            if (pNextTile->HasComponent<TileComponent>())
                pNextTile->GetComponent<TileComponent>()->MoveCharacterHere(characterObject);
            else
            {
                // disc code
            }
        }
        else
            m_pQbertComponent->m_IsFalling = true; // replace this with falling obvserver
        break;
    }
    case MovementState::End:
    {
        if (pCurrentTile->GetComponent<TileComponent>()->ChangeTile(m_CurrentRound))
        {
            ++m_TilesCovered;
            TileChanged->NotifyObservers(AreAllTilesCovered());
        }
        break;
    }
    default:
        return;
    }
}

void dae::LevelManagerComponent::SubjectDestroyed(Subject<Character, MovementState, MovementDirection>* pSubject)
{
    if (pSubject == m_pQbertComponent->MoveStateChanged.get())
        m_pQbertComponent = nullptr;
}

void dae::LevelManagerComponent::Notify(bool roundFinished)
{
    if (roundFinished)
    {
        ++m_CurrentRound;
        m_TilesCovered = 0;

        auto pCurrentTile = FindCharacter(Character::Qbert1);

        assert(pCurrentTile && "LevelManagerComponent: Character not found");

        auto characterObject = pCurrentTile->GetComponent<TileComponent>()->GetCharacter(Character::Qbert1);
        
        for (auto pTile : m_vTiles)
            pTile->GetComponent<TileComponent>()->Reset(m_CurrentRound);

        m_vTiles[0]->GetComponent<TileComponent>()->MoveCharacterHere(characterObject);
    }
}

bool dae::LevelManagerComponent::AreAllTilesCovered() const
{ 
    return TileComponent::GetMaxTileStage() * m_vTiles.size() == m_TilesCovered;
}

dae::GameObject* dae::LevelManagerComponent::FindCharacter(Character character) const
{
    auto it = std::find_if(std::execution::par_unseq, m_vTiles.begin(), m_vTiles.end(), [&character](GameObject* pTile)
        {
            return pTile->GetComponent<TileComponent>()->IsCharacterHere(character);
        });

    if (it == m_vTiles.end())
        return nullptr;

    return *it;
}
