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

std::unique_ptr<dae::Subject<Character, Character>> LevelManagerComponent::CharactersCollide{ std::make_unique<dae::Subject<Character, Character>>() };
int LevelManagerComponent::m_CurrentRound{ 0 };
bool LevelManagerComponent::m_RoundOver{ false };
LevelManagerComponent::LevelManagerComponent(dae::GameObject* pGameObject, dae::Scene& scene) : BaseComponent(pGameObject)
    , m_pMoveStateChangedSubject{ CharacterComponent::MoveStateChanged.get() }
    , m_pCharacterSpawnedSubject{ CharacterComponent::CharacterSpawned.get() }
    , m_pDiskStateChanged{ DiskComponent::DiskStateChanged.get() }
{
    TileChanged = std::make_unique<dae::Subject<bool>>();
    NewRoundStarted = std::make_unique<dae::Subject<>>();

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

    // Initializing disks
    for (int i{}; i < m_AmountOfDisks; ++i)
    {
        auto disk = std::make_unique<dae::GameObject>();
        disk->AddComponent<dae::GraphicsComponent>("Disk Spritesheet.png");
        disk->AddComponent<dae::SpritesheetComponent>(4, 6);
        disk->AddComponent<DiskComponent>(this);

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
}

void LevelManagerComponent::Init()
{
    m_pMoveStateChangedSubject->AddObserver(this);
    m_pCharacterSpawnedSubject->AddObserver(this);
    m_pDiskStateChanged->AddObserver(this);
    TileChanged->AddObserver(this); 

    m_TileSize = m_Tiles[{0, 0}]->GetComponent<dae::GraphicsComponent>()->GetTextureSize();

    for (const auto& [index, pTile] : m_Tiles)
    {
        if (m_DiskSize == glm::ivec2{ 0 } && pTile->HasComponent<DiskComponent>())
            m_DiskSize = pTile->GetComponent<dae::GraphicsComponent>()->GetTextureSize();

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
    auto currentTilePairIt = m_Characters.find(character);
    if (currentTilePairIt == m_Characters.end())
    {
        assert(false);
        return;
    }
    glm::ivec2 nextTileIdx = currentTilePairIt->second.tileIndex + movementInfo.indexOffset;

    switch (movementInfo.state)
    {
    case MovementState::Start:
        m_Characters[character].isMoving = true;
        break;
    case MovementState::End:
    {
        m_Characters[character].tileIndex = nextTileIdx;
        m_Characters[character].isMoving = false;
        m_CharacterMovedDirtyFlag = true;

        auto nextTilePairIt = m_Tiles.find(nextTileIdx);
        if (nextTilePairIt != m_Tiles.end())
            LandOnTile(character, nextTilePairIt->second->GetComponent<TileComponent>());
        break;
    }
    case MovementState::Fall:
        if (character == Character::Qbert1 || character == Character::Qbert2)
            break;
        m_Characters[character].tileIndex = { -1,-1 };
        break;
    case MovementState::Disk:
        m_Characters[character].tileIndex = nextTileIdx;
        DiskComponent::DiskStateChanged->NotifyObservers({ m_Tiles[nextTileIdx], DiskState::Start}, character);
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
    if (disk.state == DiskState::Start)
        return;

    m_vInactiveDisks.push_back(disk.pGameObject);
    auto it = std::find_if(m_Tiles.begin(), m_Tiles.end(), [&disk](const auto& pair) 
        {
            return pair.second == disk.pGameObject;
        });
    m_Tiles.erase(it);

    m_Characters[character].tileIndex = { 0,0 };
}

void LevelManagerComponent::SubjectDestroyed(dae::Subject<Disk, Character>* pSubject)
{
    if (pSubject == m_pDiskStateChanged)
        m_pDiskStateChanged = nullptr;
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

MovementInfo LevelManagerComponent::GetDirectionToNearestQbert() const
{
    auto coilyTilePairIt = m_Characters.find(Character::Coily);
    auto qbert1TilePairIt = m_Characters.find(Character::Qbert1);
    auto qbert2TilePairIt = m_Characters.find(Character::Qbert2);

    if (coilyTilePairIt == m_Characters.end() || qbert1TilePairIt == m_Characters.end())
    {
        assert(false);
        return MovementInfo{};
    }

    glm::ivec2 deltaTileIdx = coilyTilePairIt->second.tileIndex - qbert1TilePairIt->second.tileIndex;

    if (qbert2TilePairIt != m_Characters.end())
    {
        glm::ivec2 deltaTileIdx2 = coilyTilePairIt->second.tileIndex - qbert2TilePairIt->second.tileIndex;
        if (CalculateManhattanDistance(deltaTileIdx) > CalculateManhattanDistance(deltaTileIdx2))
            deltaTileIdx = deltaTileIdx2;
    }

    glm::ivec2 option1 = { (deltaTileIdx.x < 0) ? 1 : -1, 0 };
    glm::ivec2 option2 = { 0, (deltaTileIdx.y < 0) ? 1 : -1 };

    if (std::abs(deltaTileIdx.x) > std::abs(deltaTileIdx.y))
        deltaTileIdx = option1;
    else if (std::abs(deltaTileIdx.x) < std::abs(deltaTileIdx.y))
        deltaTileIdx = option2;
    else
    {
        bool chooseOption1First = rand() % 2 == 0;
        glm::ivec2 firstChoice = chooseOption1First ? option1 : option2;
        glm::ivec2 secondChoice = chooseOption1First ? option2 : option1;

        if (m_Tiles.find(coilyTilePairIt->second.tileIndex + firstChoice) != m_Tiles.end())
            deltaTileIdx = firstChoice;
        else
            deltaTileIdx = secondChoice;
    }

    return MovementInfo::GetMovementInfo(deltaTileIdx);
}

glm::ivec2 LevelManagerComponent::GetNewDiskIndex() const
{
    glm::ivec2 idx;
    do
    {
        idx.x = rand() % m_LevelLength;
        idx.y = -1;
        if (rand() % 2 == 1)
            std::swap(idx.y, idx.x);
    } while (m_Tiles.count({ idx.x,idx.y }));
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
    {
        offsetToPos += glm::vec3{ m_DiskSize.x, m_DiskSize.y, 0.f } * 0.5f * scale;
    }

    return offsetToPos;
}

int LevelManagerComponent::CalculateManhattanDistance(const glm::ivec2& deltaPos) const
{
    return std::abs(deltaPos.x) + std::abs(deltaPos.y);
}
