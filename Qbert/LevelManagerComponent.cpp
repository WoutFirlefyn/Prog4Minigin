//---------------------------
// Includes
//---------------------------
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "CharacterComponent.h"
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

//std::unique_ptr<dae::Subject<bool>> LevelManagerComponent::TileChanged{ std::make_unique<dae::Subject<bool>>() };
std::unique_ptr<dae::Subject<Character, TileType>> LevelManagerComponent::CharacterStartedJumping{ std::make_unique<dae::Subject<Character, TileType>>() };
std::unique_ptr<dae::Subject<Character, Character>> LevelManagerComponent::CharactersCollide{ std::make_unique<dae::Subject<Character, Character>>() };
LevelManagerComponent::LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene) : BaseComponent(pGameObject)
{
    // Subjects
    TileChanged = std::make_unique<dae::Subject<bool>>();
    //CharacterStartedJumping = std::make_unique<dae::Subject<Character, TileType>>();

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
            tile->SetParent(GetGameObject());
            tile->SetPosition(16.f * (j - i), 24.f * (i + j));
            m_vTiles.emplace_back(scene.Add(std::move(tile)));
            m_Tiles[{i, j}] = m_vTiles.back();
        }
        v2DTiles.emplace_back(vNewTiles);
    }

    // Disks spawning
    for (int i{}; i < m_AmountOfDisks; ++i)
    {
        int row, col;
        do
        {
            row = rand() % m_LevelLength;
            col = -1;
            if (rand() % 2 == 1)
                std::swap(row, col);
        } while (m_Tiles.count({ row, col }));

        auto disk = std::make_unique<dae::GameObject>();
        disk->AddComponent<dae::GraphicsComponent>("Disk Spritesheet.png");
        disk->AddComponent<dae::SpritesheetComponent>(4, 6);
        disk->AddComponent<DiskComponent>(m_Tiles[{0, 0}]);

        glm::vec3 pos{ 8,5,0 };
        if (row < 0)
        {
            disk->SetParent(m_Tiles[{0, col}]);
            pos += glm::vec3{ 16, -24, 0 };
        }
        else
        {
            disk->SetParent(m_Tiles[{row, 0}]);
            pos += glm::vec3{ -16, -24, 0 };
        }

        disk->SetPosition(pos);

        m_Tiles[{row, col}] = scene.Add(std::move(disk));
    }

    // Use the 2D vector to easily set the neighboring tiles
    for (size_t i{}; i < v2DTiles.size(); ++i)
        for (size_t j{}; j < v2DTiles[i].size(); ++j)
            v2DTiles[i][j]->SetNeighboringTiles(v2DTiles, i, j);

    //// Get all edge tiles were a disk can spawn next to
    //std::vector<TileComponent*> vEdgeTiles{};//
    //std::copy(v2DTiles[0].begin(), v2DTiles[0].end(), std::back_inserter(vEdgeTiles));//
    //std::transform(v2DTiles.begin(), v2DTiles.end(), std::back_inserter(vEdgeTiles),///
    //    [](const std::vector<TileComponent*>& row) //
    //    { //
    //        return row[0]; //
    //    });//
    //////

    //// Shuffle to randomise disk locations
    //std::random_device rd;
    //std::mt19937 g(rd());
    //std::shuffle(vEdgeTiles.begin(), vEdgeTiles.end(), g);

    //// Create disks
    //for (int i{}; i < m_AmountOfDisks; ++i)
    //{
    //    auto disk = std::make_unique<dae::GameObject>();
    //    disk->AddComponent<dae::GraphicsComponent>("Disk Spritesheet.png");
    //    disk->AddComponent<dae::SpritesheetComponent>(4, 6); 
    //    disk->AddComponent<DiskComponent>(m_vTiles[0]);
    //    disk->SetPosition(8, 5);
    //   
    //    vEdgeTiles[i]->AddDiskAsNeighbor(scene.Add(std::move(disk)));
    //}
}

LevelManagerComponent::~LevelManagerComponent()
{
    if (m_pMoveStateChangedSubject)
        m_pMoveStateChangedSubject->RemoveObserver(this);
    if (m_pCharacterSpawnedSubject)
        m_pCharacterSpawnedSubject->RemoveObserver(this);
    TileChanged->RemoveObserver(this);
}

void LevelManagerComponent::Init()
{
    m_pMoveStateChangedSubject = CharacterComponent::MoveStateChanged.get();
    m_pMoveStateChangedSubject->AddObserver(this);
    m_pCharacterSpawnedSubject = CharacterComponent::CharacterSpawned.get();
    m_pCharacterSpawnedSubject->AddObserver(this);
    TileChanged->AddObserver(this); 
}

void LevelManagerComponent::LateUpdate()
{
    if (!m_CharacterMovedDirtyFlag)
        return;

    m_CharacterMovedDirtyFlag = false;

    // check for character collisions
    for (auto pTile : m_vTiles)
    {
        auto charactersOnTile = pTile->GetComponent<TileComponent>()->GetCharacters();

        if (charactersOnTile.size() < 2)
            continue;

        std::erase_if(charactersOnTile, [&](const auto& pair) 
            {
                return m_MovingCharacters[pair.first];
            });

        if (charactersOnTile.size() < 2)
            continue;

        for (auto it1 = charactersOnTile.begin(); it1 != charactersOnTile.end(); ++it1) 
            for (auto it2 = std::next(it1); it2 != charactersOnTile.end(); ++it2) 
                CharactersCollide->NotifyObservers(it1->first, it2->first);
    }
}

void LevelManagerComponent::AddObserver(dae::Subject<Character, MovementInfo>*)
{
    //m_pMoveStateChangedSubject = pMoveStateChanged;
}

void LevelManagerComponent::Notify(Character character, MovementInfo movementInfo)
{
    if (movementInfo.direction == MovementDirection::None)
    {
        m_MovingCharacters[character] = false;
        m_CharacterMovedDirtyFlag = true;
        //return;
    }
    std::pair<std::pair<int, int>, dae::GameObject*> currentTile;

    if(!FindCharacter(character, currentTile))
    {
        assert(false && "LevelManagerComponent: Character not found");
        return;
    }

    switch (movementInfo.state)
    {
    case MovementState::Start:
    {
        auto pNextTile = currentTile.second->GetComponent<TileComponent>()->GetNeighboringTile(movementInfo.direction);
        TileType nextTileType;

        if (!pNextTile)
            nextTileType = TileType::None;
        else if (pNextTile->HasComponent<TileComponent>())
            nextTileType = TileType::Tile;
        else
            nextTileType = TileType::Disk;
        CharacterStartedJumping->NotifyObservers(character, nextTileType);
        m_MovingCharacters[character] = true;
        break;
    }
    case MovementState::End:
    {
        auto pCurrentTileComponent = currentTile.second->GetComponent<TileComponent>();
        if (movementInfo.direction != MovementDirection::None)
        {
            auto pNextTile = pCurrentTileComponent->GetNeighboringTile(movementInfo.direction);
            if (!pNextTile || !pNextTile->HasComponent<TileComponent>())
            {
                assert(false);
                return;
            }
            auto pNextTileComponent = pNextTile->GetComponent<TileComponent>();
            pNextTileComponent->MoveCharacterHere(pCurrentTileComponent->GetCharacter(character));
            pCurrentTileComponent = pNextTileComponent;
        }

        m_MovingCharacters[character] = false;
        m_CharacterMovedDirtyFlag = true;

        int tileChange{};
        switch (character)
        {
        case Character::Qbert1:
        case Character::Qbert2:
            tileChange = 1;
            break;
        case Character::Slick:
        case Character::Sam:
            tileChange = -1;
            break;
        default:
            return;
        }

        if (pCurrentTileComponent->ChangeTile(m_CurrentRound, m_TilesCovered, tileChange))
            TileChanged->NotifyObservers(AreAllTilesCovered());
        break;
    }
    case MovementState::Fall:
    {
        if (character == Character::Qbert1 || character == Character::Qbert2)
            return;

        m_InactiveCharacters.insert(currentTile.second->GetComponent<TileComponent>()->GetCharacter(character));
        break;
    }
    case MovementState::Disk:
    {
        auto pCurrentTileComponent = currentTile.second->GetComponent<TileComponent>();
        auto pNextTile = pCurrentTileComponent->GetNeighboringTile(movementInfo.direction);
        if (!pNextTile || !pNextTile->HasComponent<DiskComponent>() || (character != Character::Qbert1 && character != Character::Qbert2))
        {
            assert(false);
            return;
        }
        pNextTile->GetComponent<DiskComponent>()->MoveCharacterHere(pCurrentTileComponent->GetCharacter(character));
        break;
    }
    default:
        return;
    }
}

void LevelManagerComponent::Notify(Character character)
{
    auto characterObject = m_InactiveCharacters.extract(character);

    m_MovingCharacters[character] = true;
    if (characterObject.empty())
    {
        assert(false && "Character doesn't exist or is already spawned");
        return;
    }
    int tileIdx{};
    glm::vec3 offset{};
    switch (character)
    {
    case Character::Qbert1:
        tileIdx = 0;
        offset = glm::vec3{ 8, -6, 0 };
        break;
    case Character::Qbert2:
        break;
    case Character::Coily:
        tileIdx = 1 + (rand() % 2) * 6;
        offset = glm::vec3{ 8, -20, 0 };
        break;
    case Character::Slick:
    case Character::Sam:
        tileIdx = 1 + (rand() % 2) * 6;
        offset = glm::vec3{ 8, -6, 0 };
        break;
    case Character::Ugg:
        break;
    case Character::Wrongway:
        break;
    default:
        tileIdx = -1;
        break;
    }
    characterObject.mapped()->SetPosition(m_vTiles[tileIdx]->GetWorldPosition() + offset);
    m_vTiles[tileIdx]->GetComponent<TileComponent>()->MoveCharacterHere(std::make_pair(characterObject.key(), characterObject.mapped()));
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<Character>* pSubject)
{
    if (pSubject == m_pCharacterSpawnedSubject)
        m_pCharacterSpawnedSubject = nullptr;
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject)
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

        dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::RoundCompleteTune);

        for (auto pTile : m_vTiles)
        {
            auto pTileComponent = pTile->GetComponent<TileComponent>();
            m_InactiveCharacters.merge(pTileComponent->GetCharacters());
            pTileComponent->Reset(m_CurrentRound);
        }
        //auto pCurrentTile = FindCharacter(Character::Qbert1);

        //assert(pCurrentTile && "LevelManagerComponent: Character not found");

        //auto characterObject = pCurrentTile->GetComponent<TileComponent>()->GetCharacter(Character::Qbert1);

        //m_vTiles[0]->GetComponent<TileComponent>()->MoveCharacterHere(characterObject);
    }
}

bool LevelManagerComponent::AreAllTilesCovered() const
{ 
    return TileComponent::GetMaxTileStage() * static_cast<int>(m_vTiles.size()) == m_TilesCovered;
}

bool LevelManagerComponent::FindCharacter(Character character, std::pair<std::pair<int, int>, dae::GameObject*>& tile) const
{
    //auto it = std::find_if(std::execution::par_unseq, m_vTiles.begin(), m_vTiles.end(), [&character](dae::GameObject* pTile)
    //    {
    //        return pTile->GetComponent<TileComponent>()->IsCharacterHere(character);
    //    });
    auto it = std::find_if(std::execution::par_unseq, m_Tiles.begin(), m_Tiles.end(), [&character](const auto& tilePair)
        {
            if (tilePair.second->HasComponent<TileComponent>())
                return tilePair.second->GetComponent<TileComponent>()->IsCharacterHere(character);
            return false;
            //return tilePair.second->GetComponent<TileComponent>()->IsCharacterHere(character);
        });

    if (it == m_Tiles.end())
        return false;

    tile = *it;
    return true;
}
