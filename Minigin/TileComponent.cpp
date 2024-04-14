//---------------------------
// Includes
//---------------------------
#include "TileComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "SpritesheetComponent.h"
#include "GameObject.h"
#include <iostream>

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
        m_pQbertComponent->PlayerMoveStateChanged->RemoveObserver(this);
    TileChanged->RemoveObserver(this);
}

void dae::TileComponent::Init()
{
    m_pQbertComponent->PlayerMoveStateChanged->AddObserver(this);
    TileChanged->AddObserver(this);
    if (m_TileId == 0)
        MoveQbertHere();
}

void dae::TileComponent::Notify(MovementState movementState, MovementDirection movementDirection)
{
    switch (movementState)
    {
    case MovementState::Start:
        if (m_QbertIsHere && !QbertMoving)
        {
            if (auto pTile = m_vNeighboringTiles[static_cast<size_t>(movementDirection)])
            {
                m_QbertIsHere = false;
                QbertMoving = true;
                pTile->MoveQbertHere();
            }
            else
                m_pQbertComponent->m_IsFalling = true;
        }
        break;
    case MovementState::End:
        QbertMoving = false;
        if (m_QbertIsHere && m_TileStage != m_MaxTileStage)
        {
            ++m_TileStage;
            ++m_TilesCovered;
            GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(m_CurrentRound, m_TileStage);
            TileChanged->NotifyObservers(m_TileCount * m_MaxTileStage == m_TilesCovered);
        }
        break;
    default:
        return;
    }
}

void dae::TileComponent::SubjectDestroyed(Subject<MovementState, MovementDirection>* pSubject)
{
    if (pSubject == m_pQbertComponent->PlayerMoveStateChanged.get())
        m_pQbertComponent = nullptr;
}

void dae::TileComponent::Notify(bool roundFinished)
{
    if (roundFinished)
    {
        ++m_CurrentRound;
        m_TilesCovered = m_TileStage = 0;

        GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(m_CurrentRound, m_TileStage);

        m_QbertIsHere = false;
        if (m_TileId == 0)
            MoveQbertHere();
    }
}

void dae::TileComponent::SetNeighboringTiles(const std::vector<std::vector<GameObject*>>& vTiles, size_t row, size_t col)
{
    m_vNeighboringTiles.push_back((row > 0) ? vTiles[row - 1][col]->GetComponent<TileComponent>() : nullptr);
    m_vNeighboringTiles.push_back((col > 0) ? vTiles[row][col - 1]->GetComponent<TileComponent>() : nullptr);
    m_vNeighboringTiles.push_back((row < 7 - (col + 1)) ? vTiles[row][col + 1]->GetComponent<TileComponent>() : nullptr);
    m_vNeighboringTiles.push_back((col < 7 - (row + 1)) ? vTiles[row + 1][col]->GetComponent<TileComponent>() : nullptr);
}

void dae::TileComponent::MoveQbertHere()
{
    m_QbertIsHere = true;
}




