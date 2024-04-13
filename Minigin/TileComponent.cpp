//---------------------------
// Includes
//---------------------------
#include "TileComponent.h"
#include "QbertComponent.h"
#include "GraphicsComponent.h"
#include "TileSpritesheetComponent.h"
#include "GameObject.h"
#include <iostream>

int dae::TileComponent::m_TileCount{ 0 };
int dae::TileComponent::m_TilesCovered{ 0 };
std::unique_ptr<dae::Subject<>> dae::TileComponent::TileChanged{ std::make_unique<Subject<>>() };
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
    if (m_pQbertComponent->PlayerMoved)
        m_pQbertComponent->PlayerMoved->RemoveObserver(this);
    if (m_pQbertComponent->PlayerFinishedMoving)
        m_pQbertComponent->PlayerFinishedMoving->RemoveObserver(this);
}

void dae::TileComponent::Init()
{
    m_pQbertComponent->PlayerMoved->AddObserver(this);
    m_pQbertComponent->PlayerFinishedMoving->AddObserver(this);
    if (m_TileId == 0)
        MoveQbertHere();
}

void dae::TileComponent::Notify(MovementDirection movementDirection)
{
    if (m_QbertIsHere && !QbertMoving)
    {
        auto pTile = m_vNeighboringTiles[static_cast<size_t>(movementDirection)];
        if (pTile)
        {
            m_QbertIsHere = false;
            QbertMoving = true;
            pTile->MoveQbertHere();
        }
    }
}

void dae::TileComponent::Notify()
{
    QbertMoving = false;
    if (m_QbertIsHere && m_TileStage != m_MaxTileStage)
    {
        ++m_TileStage;
        ++m_TilesCovered;
        GetGameObject()->GetComponent<SpritesheetComponent>()->MoveSourceRect(0, m_TileStage);
        TileChanged->NotifyObservers();
    }
}

void dae::TileComponent::SubjectDestroyed(Subject<MovementDirection>* pSubject)
{
    if (pSubject == m_pQbertComponent->PlayerMoved.get())
        m_pQbertComponent->PlayerMoved = nullptr;
}

void dae::TileComponent::SubjectDestroyed(Subject<>* pSubject)
{
    if (pSubject == m_pQbertComponent->PlayerFinishedMoving.get())
        m_pQbertComponent->PlayerFinishedMoving = nullptr;
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

//---------------------------
// Member functions
//---------------------------

// write member functions here




