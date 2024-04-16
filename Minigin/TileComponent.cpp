//---------------------------
// Includes
//---------------------------
#include "TileComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "GameObject.h"
#include <iostream>

std::pair<dae::Characters, dae::GameObject*> dae::TileComponent::tempQbertHolder{};

int dae::TileComponent::m_TileCount{ 0 };
int dae::TileComponent::m_TilesCovered{ 0 };
std::unique_ptr<dae::Subject<bool>> dae::TileComponent::TileChanged{ std::make_unique<Subject<bool>>() };
//---------------------------
// Constructor & Destructor
//---------------------------
dae::TileComponent::TileComponent(GameObject* pGameObject, QbertComponent* pQbertComponent) : BaseComponent(pGameObject)
    , m_pQbertComponent{ pQbertComponent }
    , m_TileId{ m_TileCount++ }
{
}

dae::TileComponent::~TileComponent()
{
    if (m_pQbertComponent)
        m_pQbertComponent->MoveStateChanged->RemoveObserver(this);
    TileChanged->RemoveObserver(this);
}

void dae::TileComponent::Init()
{
    m_pQbertComponent->MoveStateChanged->AddObserver(this);
    auto qbert = m_pQbertComponent->GetGameObject();
    TileChanged->AddObserver(this);
    auto pair = std::make_pair(Characters::Qbert1, qbert);
    tempQbertHolder = pair;
    if (m_TileId == 0)
        MoveCharacterHere(pair);
}

void dae::TileComponent::Notify(Characters character, MovementState movementState, MovementDirection movementDirection)
{
    switch (movementState)
    {
    case MovementState::Start:
        if (m_CharactersHere.contains(character) && !QbertMoving)
        {
            if (auto pTile = m_vNeighboringTiles[static_cast<size_t>(movementDirection)])
            {
                QbertMoving = true;
                auto characterNode = m_CharactersHere.extract(character);
                pTile->GetComponent<TileComponent>()->MoveCharacterHere(std::make_pair(characterNode.key(), characterNode.mapped()));
            }
            else
                m_pQbertComponent->m_IsFalling = true;
        }
        break;
    case MovementState::End:
        QbertMoving = false;
        if (m_CharactersHere.contains(character) && m_TileStage != m_MaxTileStage)
        {
            ++m_TileStage;
            ++m_TilesCovered;
            GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(m_CurrentRound, m_TileStage);
            TileChanged->NotifyObservers(AreAllTilesCovered());
        }
        break;
    default:
        return;
    }
}

void dae::TileComponent::SubjectDestroyed(Subject<Characters, MovementState, MovementDirection>* pSubject)
{
    if (pSubject == m_pQbertComponent->MoveStateChanged.get())
        m_pQbertComponent = nullptr;
}

void dae::TileComponent::Notify(bool roundFinished)
{
    if (roundFinished)
    {
        ++m_CurrentRound;
        m_TilesCovered = m_TileStage = 0;
        GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(m_CurrentRound, m_TileStage);
        m_CharactersHere.clear(); 
        if (m_TileId == 0)
            MoveCharacterHere(tempQbertHolder);
    }
}

void dae::TileComponent::SetNeighboringTiles(const std::vector<std::vector<GameObject*>>& vTiles, size_t row, size_t col)
{
    m_vNeighboringTiles.push_back((row > 0) ? vTiles[row - 1][col] : nullptr);
    m_vNeighboringTiles.push_back((col > 0) ? vTiles[row][col - 1] : nullptr);
    m_vNeighboringTiles.push_back((row < 7 - (col + 1)) ? vTiles[row][col + 1] : nullptr);
    m_vNeighboringTiles.push_back((col < 7 - (row + 1)) ? vTiles[row + 1][col] : nullptr);
}

bool dae::TileComponent::IsEdgeTile() const
{
    return std::any_of(std::execution::par_unseq, m_vNeighboringTiles.begin(), m_vNeighboringTiles.end(), [](GameObject* pGameObject)
        {
            return pGameObject == nullptr;
        });
}

void dae::TileComponent::MoveCharacterHere(const std::pair<Characters, GameObject*>& character)
{
    m_CharactersHere.insert(character);
}




