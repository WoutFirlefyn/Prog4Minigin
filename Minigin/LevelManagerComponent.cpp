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
#include <algorithm>

//---------------------------
// Constructor & Destructor
//---------------------------
dae::LevelManagerComponent::LevelManagerComponent(GameObject* pGameObject, QbertComponent* pQbertComponent, Scene& scene) : BaseComponent(pGameObject)
    , m_pQbertComponent{ pQbertComponent }
{
    TileChanged = std::make_unique<Subject<bool>>();

    glm::vec3 startPos{ 300, 200, 0 };
    std::vector<std::vector<TileComponent*>> vTiles{};

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
            scene.Add(std::move(tile));
        }
        vTiles.emplace_back(vNewTiles);
    }

    for (int i{}; i < vTiles.size(); ++i)
        for (int j{}; j < vTiles[i].size(); ++j)
            vTiles[i][j]->SetNeighboringTiles(vTiles, i, j);

    for (const auto& row : vTiles)
        m_vTiles.insert(m_vTiles.end(), row.begin(), row.end());
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

    m_vTiles[0]->MoveCharacterHere(std::make_pair(Character::Qbert1, m_pQbertComponent->GetGameObject()));
}

void dae::LevelManagerComponent::Notify(Character character, MovementState movementState, MovementDirection movementDirection)
{
    auto pCurrentTile = FindCharacter(character);
    assert(pCurrentTile && "LevelManagerComponent: Character not found");

    switch (movementState)
    {
    case MovementState::Start:
    {
        auto pNextTile = pCurrentTile->GetNeighboringTile(movementDirection);
        if (pNextTile)
        {
            std::pair<Character, GameObject*> characterObject = std::make_pair(character, nullptr);
            pCurrentTile->GetCharacter(characterObject);

            if (auto pTileComponent = pNextTile->GetComponent<TileComponent>())
                pTileComponent->MoveCharacterHere(characterObject);
            else
            {
                // disc code
            }
        }
        else
            m_pQbertComponent->m_IsFalling = true;
        break;
    }
    case MovementState::End:
    {
        if (pCurrentTile->ChangeTile(m_CurrentRound))
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

        std::pair<Character, GameObject*> characterObject = std::make_pair(Character::Qbert1, nullptr);

        pCurrentTile->GetCharacter(characterObject);
        
        for (auto pTile : m_vTiles)
            pTile->Reset(m_CurrentRound);

        m_vTiles[0]->MoveCharacterHere(characterObject);
    }
}

bool dae::LevelManagerComponent::AreAllTilesCovered() const
{ 
    return TileComponent::GetMaxTileStage() * m_vTiles.size() == m_TilesCovered;
}

dae::TileComponent* dae::LevelManagerComponent::FindCharacter(Character character) const
{
    auto it = std::find_if(std::execution::par_unseq, m_vTiles.begin(), m_vTiles.end(), [&character](TileComponent* pTile)
        {
            return pTile->IsCharacterHere(character);
        });

    if (it == m_vTiles.end())
        return nullptr;

    return *it;
}
