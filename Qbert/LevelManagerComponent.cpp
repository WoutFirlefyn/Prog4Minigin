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
#include "ServiceLocator.h"
#include "Sounds.h"
#include <algorithm>
#include <iterator>
#include <random>

//---------------------------
// Constructor & Destructor
//---------------------------
LevelManagerComponent::LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene) : BaseComponent(pGameObject)
{
    // Subjects
    TileChanged = std::make_unique<dae::Subject<bool>>();
    CharacterFell = std::make_unique<dae::Subject<Character>>();

    glm::vec3 startPos{ 300, 200, 0 };
    std::vector<std::vector<TileComponent*>> v2DTiles{};

    // Creating and placing the tiles
    for (int i{}; i < m_LevelLength; ++i)
    {
        std::vector<TileComponent*> vNewTiles{};
        for (int j{}; j < m_LevelLength - i; ++j)
        {
            auto tile = std::make_unique<dae::GameObject>();
            tile->AddComponent<dae::GraphicsComponent>("Qbert Cubes.png");
            tile->AddComponent<dae::SpritesheetComponent>(6, 3);
            tile->AddComponent<TileComponent>();
            vNewTiles.emplace_back(tile->GetComponent<TileComponent>());

            tile->SetPosition(startPos.x + 16.f * (j - i), startPos.y + 24.f * (i + j));
            m_vTiles.emplace_back(scene.Add(std::move(tile)));
        }
        v2DTiles.emplace_back(vNewTiles);
    }

    // Use the 2D vector to easily set the neighboring tiles
    for (size_t i{}; i < v2DTiles.size(); ++i)
        for (size_t j{}; j < v2DTiles[i].size(); ++j)
            v2DTiles[i][j]->SetNeighboringTiles(v2DTiles, i, j);

    // Get all edge tiles were a disk can spawn next to
    std::vector<TileComponent*> vEdgeTiles{};
    std::copy(v2DTiles[0].begin(), v2DTiles[0].end(), std::back_inserter(vEdgeTiles));
    std::transform(v2DTiles.begin(), v2DTiles.end(), std::back_inserter(vEdgeTiles),
        [](const std::vector<TileComponent*>& row) 
        { 
            return row[0]; 
        });

    // Shuffle to randomise disk locations
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vEdgeTiles.begin(), vEdgeTiles.end(), g);

    // Create disks
    for (int i{}; i < m_AmountOfDisks; ++i)
    {
        auto disk = std::make_unique<dae::GameObject>();
        disk->AddComponent<dae::GraphicsComponent>("Disk Spritesheet.png");
        disk->AddComponent<dae::SpritesheetComponent>(4, 6); 
        disk->AddComponent<DiskComponent>(m_vTiles[0]);
        disk->SetPosition(8, 5);
       
        vEdgeTiles[i]->AddDiskAsNeighbor(scene.Add(std::move(disk)));
    }
}

LevelManagerComponent::~LevelManagerComponent()
{
    if (m_pMoveStateChangedSubject)
        m_pMoveStateChangedSubject->RemoveObserver(this);
    TileChanged->RemoveObserver(this);
}

void LevelManagerComponent::SpawnQbert(dae::GameObject* pGameObject)
{
    m_vTiles[0]->GetComponent<TileComponent>()->MoveCharacterHere(std::make_pair(Character::Qbert1, pGameObject));
}

void LevelManagerComponent::Init()
{
    m_pMoveStateChangedSubject->AddObserver(this);
    TileChanged->AddObserver(this);
}

void LevelManagerComponent::AddObserver(dae::Subject<Character, MovementState, MovementDirection>* pMoveStateChanged)
{
    m_pMoveStateChangedSubject = pMoveStateChanged;
}

void LevelManagerComponent::Notify(Character character, MovementState movementState, MovementDirection movementDirection)
{
    auto pCurrentTile = FindCharacter(character);

    if (!pCurrentTile)
        return;
    //assert(pCurrentTile && "LevelManagerComponent: Character not found");

    switch (movementState)
    {
    case MovementState::Start:
    {
        // maybe in some cases (e.g. enemy ai) check where they're jumping towards when they start jumping
        break;
    }
    case MovementState::End:
    {
        auto pNextTile = pCurrentTile->GetComponent<TileComponent>()->GetNeighboringTile(movementDirection);
        if (pNextTile)
        {
            auto characterObject = pCurrentTile->GetComponent<TileComponent>()->GetCharacter(character);
            if (pNextTile->HasComponent<TileComponent>())
            {
                auto pNextTileComponent = pNextTile->GetComponent<TileComponent>();
                pNextTileComponent->MoveCharacterHere(characterObject);
                if (pNextTileComponent->ChangeTile(m_CurrentRound))
                {
                    ++m_TilesCovered;
                    TileChanged->NotifyObservers(AreAllTilesCovered());
                }
            }
            else if (pNextTile->HasComponent<DiskComponent>())
            {
                pNextTile->GetComponent<DiskComponent>()->MoveCharacterHere(characterObject);
            }
        }
        else
            CharacterFell->NotifyObservers(character);
        break;
    }
    default:
        return;
    }
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<Character, MovementState, MovementDirection>* pSubject)
{
    if (pSubject == m_pMoveStateChangedSubject)
        m_pMoveStateChangedSubject = nullptr;
}

void LevelManagerComponent::Notify(bool roundFinished)
{
    if (roundFinished)
    {
        ++m_CurrentRound;
        m_TilesCovered = 0;

        dae::ServiceLocator::GetSoundSystem().Play(static_cast<dae::SoundId>(Sounds::RoundCompleteTune));

        auto pCurrentTile = FindCharacter(Character::Qbert1);

        assert(pCurrentTile && "LevelManagerComponent: Character not found");

        auto characterObject = pCurrentTile->GetComponent<TileComponent>()->GetCharacter(Character::Qbert1);
        
        for (auto pTile : m_vTiles)
            pTile->GetComponent<TileComponent>()->Reset(m_CurrentRound);

        m_vTiles[0]->GetComponent<TileComponent>()->MoveCharacterHere(characterObject);
    }
}

bool LevelManagerComponent::AreAllTilesCovered() const
{ 
    return TileComponent::GetMaxTileStage() * static_cast<int>(m_vTiles.size()) == m_TilesCovered;
}

dae::GameObject* LevelManagerComponent::FindCharacter(Character character) const
{
    auto it = std::find_if(std::execution::par_unseq, m_vTiles.begin(), m_vTiles.end(), [&character](dae::GameObject* pTile)
        {
            return pTile->GetComponent<TileComponent>()->IsCharacterHere(character);
        });

    if (it == m_vTiles.end())
        return nullptr;

    return *it;
}
