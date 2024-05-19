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

//std::unique_ptr<dae::Subject<bool>> LevelManagerComponent::TileChanged{ std::make_unique<dae::Subject<bool>>() };
std::unique_ptr<dae::Subject<Character, TileType>> LevelManagerComponent::CharacterStartedJumping{ std::make_unique<dae::Subject<Character, TileType>>() };
std::unique_ptr<dae::Subject<Character, Character>> LevelManagerComponent::CharactersCollide{ std::make_unique<dae::Subject<Character, Character>>() };
int LevelManagerComponent::m_CurrentRound{ 0 };
LevelManagerComponent::LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene) : BaseComponent(pGameObject)
{
    TileChanged = std::make_unique<dae::Subject<bool>>();

    // Initializing the tiles
    for (int i{}; i < m_LevelLength; ++i)
    {
        for (int j{}; j < m_LevelLength - i; ++j)
        {
            auto tile = std::make_unique<dae::GameObject>();
            tile->AddComponent<dae::GraphicsComponent>("Qbert Cubes.png");
            tile->AddComponent<dae::SpritesheetComponent>(6, 3);
            tile->AddComponent<TileComponent>();
            m_Tiles[{j, i}] = scene.Add(std::move(tile));
        }
    }

    // Initializing disks
    for (int i{}; i < m_AmountOfDisks; ++i)
    {
        int row, col;
        do
        {
            row = rand() % m_LevelLength;
            col = -1;
            if (rand() % 2 == 1)
                std::swap(row, col);
        } while (m_Tiles.count({ col,row }));

        auto disk = std::make_unique<dae::GameObject>();
        disk->AddComponent<dae::GraphicsComponent>("Disk Spritesheet.png");
        disk->AddComponent<dae::SpritesheetComponent>(4, 6);
        disk->AddComponent<DiskComponent>();

        m_Tiles[{col, row}] = scene.Add(std::move(disk));
    }
}

LevelManagerComponent::~LevelManagerComponent()
{
    if (m_pMoveStateChangedSubject)
        m_pMoveStateChangedSubject->RemoveObserver(this);
    if (m_pCharacterSpawnedSubject)
        m_pCharacterSpawnedSubject->RemoveObserver(this);
    if (m_pDiskReachedTopSubject)
        m_pDiskReachedTopSubject->RemoveObserver(this);
    TileChanged->RemoveObserver(this);
}

void LevelManagerComponent::Init()
{
    m_pMoveStateChangedSubject = CharacterComponent::MoveStateChanged.get();
    m_pMoveStateChangedSubject->AddObserver(this);
    m_pCharacterSpawnedSubject = CharacterComponent::CharacterSpawned.get();
    m_pCharacterSpawnedSubject->AddObserver(this);
    m_pDiskReachedTopSubject = DiskComponent::DiskReachedTop.get();
    m_pDiskReachedTopSubject->AddObserver(this);
    TileChanged->AddObserver(this); 

    const glm::vec3 scale = GetGameObject()->GetWorldScale();

    const glm::ivec2 tileSize = m_Tiles[{0, 0}]->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
    const glm::vec3 tileOffset{ tileSize.x * 0.5f * scale.x, tileSize.y * 0.75f * scale.y, 0.f };

    for (const auto& [index, pTile] : m_Tiles)
    {
        const auto& [col, row] = index;
        pTile->SetParent(GetGameObject());
        glm::vec3 offsetToPos{ tileOffset.x * (col - row), tileOffset.y * (row + col), 0.f };
        if (col >= 0 && row >= 0)
        {
            pTile->SetPosition(offsetToPos);
            continue;
        }

        const glm::ivec2 diskSize = pTile->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
        pTile->SetPosition(glm::vec3{ diskSize.x, diskSize.y, 0.f } * 0.5f * scale + offsetToPos);
    }
}

void LevelManagerComponent::LateUpdate()
{
    if (!m_CharacterMovedDirtyFlag)
        return;

    m_CharacterMovedDirtyFlag = false;

    // check for character collisions
    std::for_each(std::execution::par_unseq, m_Tiles.begin(), m_Tiles.end(), [&](auto& tilePair)
        {
            if (tilePair.second->HasComponent<DiskComponent>())
                return;

            auto charactersOnTile = tilePair.second->GetComponent<TileComponent>()->GetCharacters();

            if (charactersOnTile.size() < 2)
                return;

            std::erase_if(charactersOnTile, [&](const auto& pair)
                {
                    return m_MovingCharacters[pair.first];
                });

            if (charactersOnTile.size() < 2)
                return;

            std::lock_guard lock(m_CharactersCollideMutex);

            for (auto it1 = charactersOnTile.begin(); it1 != charactersOnTile.end(); ++it1)
                for (auto it2 = std::next(it1); it2 != charactersOnTile.end(); ++it2)
                    CharactersCollide->NotifyObservers(it1->first, it2->first);
    });
}

void LevelManagerComponent::Notify(Character character, MovementInfo movementInfo)
{
    std::pair<std::pair<int, int>, dae::GameObject*> currentTilePair;

    if(!FindCharacter(character, currentTilePair))
    {
        assert(false && "LevelManagerComponent: Character not found");
        return;
    };
    auto pCurrentTileComponent = currentTilePair.second->GetComponent<TileComponent>();

    std::pair<int,int> nextTileIdx = { currentTilePair.first.first + movementInfo.indexOffset.first, currentTilePair.first.second + movementInfo.indexOffset.second };
    auto nextTilePairIt = m_Tiles.find(nextTileIdx);

    switch (movementInfo.state)
    {
    case MovementState::Start:
        TileType nextTileType;

        if (nextTilePairIt == m_Tiles.end())
            nextTileType = TileType::None;
        else if (nextTilePairIt->second->HasComponent<TileComponent>())
            nextTileType = TileType::Tile;
        else
            nextTileType = TileType::Disk;
        CharacterStartedJumping->NotifyObservers(character, nextTileType);
        m_MovingCharacters[character] = true;
        break;
    case MovementState::End:
        if (currentTilePair.second != nextTilePairIt->second)
        {
            auto pNextTileComponent = nextTilePairIt->second->GetComponent<TileComponent>();
            pNextTileComponent->MoveCharacterHere(pCurrentTileComponent->GetCharacter(character));
            pCurrentTileComponent = pNextTileComponent;
        }
        ChangeTile(character, pCurrentTileComponent);

        m_MovingCharacters[character] = false;
        m_CharacterMovedDirtyFlag = true;
        break;
    case MovementState::Fall:
        if (character == Character::Qbert1 || character == Character::Qbert2)
            return;

        m_InactiveCharacters.insert(currentTilePair.second->GetComponent<TileComponent>()->GetCharacter(character));
        break;
    case MovementState::Disk:
        nextTilePairIt->second->GetComponent<DiskComponent>()->MoveCharacterHere(pCurrentTileComponent->GetCharacter(character), m_Tiles[{0,0}]);
        break;
    default:
        return;
    }
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<Character, MovementInfo>* pSubject)
{
    if (pSubject == m_pMoveStateChangedSubject)
        m_pMoveStateChangedSubject = nullptr;
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
    std::pair<int,int> tileIdx{};
    glm::vec3 offset{GetGameObject()->GetLocalScale()};
    switch (character)
    {
    case Character::Qbert1:
        tileIdx = { 0,0 };
        offset *= glm::vec3{ 8, -6, 0 };
        break;
    case Character::Qbert2:
        break;
    case Character::Coily:
        tileIdx = rand() % 2 ? std::make_pair(1,0) : std::make_pair(0,1);
        offset *= glm::vec3{ 8, -20, 0 };
        break;
    case Character::Slick:
    case Character::Sam:
        tileIdx = rand() % 2 ? std::make_pair(1,0) : std::make_pair(0,1);
        offset *= glm::vec3{ 8, -6, 0 };
        break;
    case Character::Ugg:
        break;
    case Character::Wrongway:
        break;
    default:
        return;
    }
    characterObject.mapped()->SetPosition(m_Tiles[tileIdx]->GetWorldPosition() + offset);
    m_Tiles[tileIdx]->GetComponent<TileComponent>()->MoveCharacterHere(std::make_pair(characterObject.key(), characterObject.mapped()));
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<Character>* pSubject)
{
    if (pSubject == m_pCharacterSpawnedSubject)
        m_pCharacterSpawnedSubject = nullptr;
}

void LevelManagerComponent::Notify(bool roundFinished)
{
    if (roundFinished)
    {
        ++m_CurrentRound;
        m_TilesCovered = 0;

        dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::RoundCompleteTune);

        for (const auto& [index, pTile] : m_Tiles)
        {
            auto pTileComponent = pTile->GetComponent<TileComponent>();
            m_InactiveCharacters.merge(pTileComponent->GetCharacters());
            pTileComponent->Reset();
        }
    }
}

void LevelManagerComponent::Notify(dae::GameObject* pDisk, Character)
{
    auto it = std::find_if(m_Tiles.begin(), m_Tiles.end(), [pDisk](const auto& pair) 
        {
            return pair.second == pDisk;
        });

    if (it != m_Tiles.end()) 
    {
        m_Tiles[{0,0}]->GetComponent<TileComponent>()->MoveCharacterHere(it->second->GetComponent<DiskComponent>()->GetCharacter());
        m_vInactiveDisks.push_back(it->second);
        m_Tiles.erase(it);
    }
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<dae::GameObject*, Character>* pSubject)
{
    if (pSubject == m_pDiskReachedTopSubject)
        m_pDiskReachedTopSubject = nullptr;
}

bool LevelManagerComponent::AreAllTilesCovered() const
{ 
    const int amountOfTiles = m_LevelLength * (m_LevelLength + 1) / 2;
    return TileComponent::GetMaxTileStage() * amountOfTiles == m_TilesCovered;
}

void LevelManagerComponent::ChangeTile(Character character, TileComponent* pTileComponent)
{
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

    if (pTileComponent->ChangeTile(m_TilesCovered, tileChange))
        TileChanged->NotifyObservers(AreAllTilesCovered());
}

bool LevelManagerComponent::FindCharacter(Character character, std::pair<std::pair<int, int>, dae::GameObject*>& tile) const
{
    auto it = std::find_if(std::execution::par_unseq, m_Tiles.begin(), m_Tiles.end(), [&character](const auto& tilePair)
        {
            if (auto pTileComponent = tilePair.second->GetComponent<TileComponent>())
                return pTileComponent->IsCharacterHere(character);
            return false;
        });

    if (it == m_Tiles.end())
        return false;

    tile = *it;
    return true;
}
