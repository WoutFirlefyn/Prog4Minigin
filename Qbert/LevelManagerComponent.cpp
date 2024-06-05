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
#include "GameTime.h"
#include <algorithm>

int LevelManagerComponent::m_CurrentRound{ 0 };
bool LevelManagerComponent::m_RoundOver{ false };
LevelManagerComponent::LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene) : BaseComponent(pGameObject)
    , m_pMoveStateChangedSubject{ CharacterComponent::MoveStateChanged.get() }
    , m_pCharacterSpawnedSubject{ CharacterComponent::CharacterSpawned.get() }
    , m_pDiskStateChanged{ DiskComponent::DiskStateChanged.get() }
{
    TileChanged = std::make_unique<dae::Subject<bool>>();
    NewRoundStarted = std::make_unique<dae::Subject<>>();
    CharactersCollide = std::make_unique<dae::Subject<Character, Character>>();

    // Initializing the tiles
    for (int i{}; i < m_LevelLength; ++i)
    {
        for (int j{}; j < m_LevelLength - i; ++j)
        {
            auto tile = std::make_unique<dae::GameObject>();
            tile->AddComponent<dae::GraphicsComponent>("Qbert Cubes.png");
            tile->AddComponent<dae::SpritesheetComponent>(6, 3);
            tile->AddComponent<TileComponent>(this);
            m_Tiles[{j, i}] = scene.Add(std::move(tile));
        }
    }

    m_TileSize = m_Tiles[{0, 0}]->GetComponent<dae::GraphicsComponent>()->GetTextureSize();
    
    // Initializing disks
    for (int i{}; i < m_AmountOfDisks; ++i)
    {
        auto disk = std::make_unique<dae::GameObject>();
        disk->AddComponent<dae::GraphicsComponent>("Disk Spritesheet.png");
        disk->AddComponent<dae::SpritesheetComponent>(4, 6);
        disk->AddComponent<DiskComponent>(this);

        if (m_DiskSize == glm::ivec2{ 0 })
            m_DiskSize = disk->GetComponent<dae::GraphicsComponent>()->GetTextureSize();

        m_Tiles[GetNewDiskIndex()] = scene.Add(std::move(disk));
    }
}

LevelManagerComponent::~LevelManagerComponent()
{
    if (m_pMoveStateChangedSubject)
        m_pMoveStateChangedSubject->RemoveObserver(this);
    if (m_pCharacterSpawnedSubject)
        m_pCharacterSpawnedSubject->RemoveObserver(this);
    if (m_pDiskStateChanged)
        m_pDiskStateChanged->RemoveObserver(this);
    if (TileChanged.get())
        TileChanged->RemoveObserver(this);
    if (NewRoundStarted.get())
        NewRoundStarted->RemoveObserver(this);
}

void LevelManagerComponent::Init()
{
    m_pMoveStateChangedSubject->AddObserver(this);
    m_pCharacterSpawnedSubject->AddObserver(this);
    m_pDiskStateChanged->AddObserver(this);
    TileChanged->AddObserver(this); 
    NewRoundStarted->AddObserver(this);

    for (const auto& [index, pTile] : m_Tiles)
    {
        pTile->SetParent(GetGameObject());
        pTile->SetPosition(GetTilePos(index));
    }
}

void LevelManagerComponent::Update()
{
    if (!m_RoundOver)
        return;

    m_AccumSec += dae::GameTime::GetInstance().GetDeltaTime();

    if (m_AccumSec >= m_RoundOverDelay)
    {
        m_RoundOver = false;
        m_AccumSec = 0;
        NewRoundStarted->NotifyObservers();
    }
}

void LevelManagerComponent::LateUpdate()
{
    if (!m_CharacterMovedDirtyFlag)
        return;

    m_CharacterMovedDirtyFlag = false;

    for (auto it1 = m_Characters.begin(); it1 != m_Characters.end(); ++it1)
        for (auto it2 = std::next(it1); it2 != m_Characters.end(); ++it2)
            if(it1->second.tileIndex == it2->second.tileIndex && !it1->second.isMoving && !it2->second.isMoving)
                CharactersCollide->NotifyObservers(it1->first, it2->first);
}

void LevelManagerComponent::Notify(Character character, MovementInfo movementInfo)
{
    auto characterPairIt = m_Characters.find(character);
    if (characterPairIt == m_Characters.end())
    {
        assert(false);
        return;
    }

    switch (movementInfo.state)
    {
    case MovementState::Start:
        characterPairIt->second.isMoving = true;
        characterPairIt->second.previousTileIndex = characterPairIt->second.tileIndex;
        characterPairIt->second.tileIndex += movementInfo.indexOffset;
        break;
    case MovementState::End:
    {
        characterPairIt->second.isMoving = false;
        m_CharacterMovedDirtyFlag = true;

        auto nextTilePairIt = m_Tiles.find(characterPairIt->second.tileIndex);
        if (nextTilePairIt != m_Tiles.end())
            LandOnTile(character, nextTilePairIt->second->GetComponent<TileComponent>());
        break;
    }
    case MovementState::Fall:
        if (character == Character::Qbert1 || character == Character::Qbert2)
            characterPairIt->second.tileIndex -= movementInfo.indexOffset; // replace with characterPairIt->second.tileIndex = movementInfo.previousTileIndex; when possible
        else
            characterPairIt->second.tileIndex = { -1,-1 };
        break;
    case MovementState::Disk:
        DiskComponent::DiskStateChanged->NotifyObservers({ m_Tiles[characterPairIt->second.tileIndex], DiskState::Start}, character);
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

void LevelManagerComponent::Notify(Character character, dae::GameObject* pCharacterGameObject)
{    
    if (character != Character::Qbert1 && character != Character::Qbert2)
        m_Characters[character].isMoving = true;

    glm::ivec2 tileIdx{ pCharacterGameObject->GetComponent<CharacterComponent>()->GetSpawnPosition() };
    glm::vec3 offset{};

    if (character == Character::Coily)
        offset = glm::vec3{ 8, -20, 0 };
    else
        offset = glm::vec3{ 8, -6, 0 };

    offset *= GetGameObject()->GetLocalScale();

    pCharacterGameObject->SetPosition(GetTilePos(tileIdx) + GetGameObject()->GetLocalPosition() + offset);
    m_Characters[character].tileIndex = tileIdx;
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<Character, dae::GameObject*>* pSubject)
{
    if (pSubject == m_pCharacterSpawnedSubject)
        m_pCharacterSpawnedSubject = nullptr;
}

void LevelManagerComponent::Notify(bool roundFinished)
{
    if (roundFinished)
    {
        m_RoundOver = true;
        ++m_CurrentRound;
        m_TilesCovered = 0;

        dae::ServiceLocator::GetSoundSystem().Play(dae::Sounds::RoundCompleteTune);
    }
}

void LevelManagerComponent::Notify(Disk disk, Character character)
{
    switch (disk.state)
    {
    case DiskState::Start:
        m_vInactiveDisks.push_back(disk.pGameObject);
        m_Tiles.erase(std::find_if(m_Tiles.begin(), m_Tiles.end(), [&disk](const auto& pair)
            {
                return pair.second == disk.pGameObject;
            }));
        break;
    case DiskState::Stop:
        m_Characters[character].tileIndex = { 0,0 };
        break;
    }
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<Disk, Character>* pSubject)
{
    if (pSubject == m_pDiskStateChanged)
        m_pDiskStateChanged = nullptr;
}

void LevelManagerComponent::Notify()
{
    std::erase_if(m_Tiles, [&](auto& tilePair)
        {
            if (tilePair.second->HasComponent<DiskComponent>())
            {
                m_vInactiveDisks.push_back(tilePair.second);
                return true;
            }
            return false;
        });

    for (dae::GameObject* pDisk : m_vInactiveDisks)
    {
        glm::ivec2 newIdx = GetNewDiskIndex();
        m_Tiles[newIdx] = pDisk;
        pDisk->SetPosition(GetTilePos(newIdx));
    }
    m_vInactiveDisks.clear();
}

TileType LevelManagerComponent::GetNextTileType(Character character, MovementInfo movementInfo) const
{
    auto currentTilePairIt = m_Characters.find(character);
    if (currentTilePairIt == m_Characters.end())
    {
        assert(false);
        return TileType::None;
    }

    glm::ivec2 nextTileIdx = currentTilePairIt->second.tileIndex + movementInfo.indexOffset;
    auto nextTilePairIt = m_Tiles.find(nextTileIdx);

    if (nextTilePairIt == m_Tiles.end())
        return TileType::None;
    if (nextTilePairIt->second->HasComponent<TileComponent>())
        return TileType::Tile;
    return TileType::Disk;
}

int LevelManagerComponent::GetAmountOfActiveDisks() const
{
    return static_cast<int>(std::count_if(m_Tiles.begin(), m_Tiles.end(), [](auto& tilePair)
        {
            return tilePair.second->HasComponent<DiskComponent>();
        }));
}

std::pair<Character, CharacterInfo> LevelManagerComponent::GetCharacter(Character character) const
{
    auto it = m_Characters.find(character);

    if (it == m_Characters.end())
        return { Character::None, CharacterInfo{} };

    return *it;
}

glm::ivec2 LevelManagerComponent::GetNewDiskIndex() const
{
    if (GetAmountOfActiveDisks() >= m_LevelLength * 2)
        return { -1,-1 };

    glm::ivec2 idx{};
    do
    {
        idx.x = rand() % m_LevelLength;
        idx.y = -1;
        if (rand() % 2 == 1)
            std::swap(idx.y, idx.x);
    } while (m_Tiles.contains({ idx.x,idx.y }));
    return idx;
}

bool LevelManagerComponent::AreAllTilesCovered() const
{ 
    const int amountOfTiles = m_LevelLength * (m_LevelLength + 1) / 2;
    return TileComponent::GetMaxTileStage() * amountOfTiles == m_TilesCovered;
}

void LevelManagerComponent::LandOnTile(Character character, TileComponent* pTileComponent)
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

glm::vec3 LevelManagerComponent::GetTilePos(glm::ivec2 tileIdx) const
{
    const glm::vec3 scale = GetGameObject()->GetWorldScale();
    const glm::vec3 tileOffset{ m_TileSize.x * 0.5f * scale.x, m_TileSize.y * 0.75f * scale.y, 0.f };

    glm::vec3 offsetToPos{ tileOffset.x * (tileIdx.x - tileIdx.y), tileOffset.y * (tileIdx.y + tileIdx.x), 0.f };

    if (tileIdx.x < 0 || tileIdx.y < 0)
        offsetToPos += glm::vec3{ m_DiskSize.x, m_DiskSize.y, 0.f } * 0.5f * scale;

    return offsetToPos;
}