//---------------------------
// Includes
//---------------------------
#include "TileComponent.h"
#include "QbertComponent.h"
#include "GameObject.h"

//---------------------------
// Constructor & Destructor
//---------------------------
dae::TileComponent::TileComponent(GameObject* pGameObject) : BaseComponent(pGameObject)
{
}

dae::TileComponent::TileComponent(GameObject* pGameObject, QbertComponent* pQbertComponent) : BaseComponent(pGameObject)
    , m_pQbertComponent{ pQbertComponent }
{
}

dae::TileComponent::~TileComponent()
{
    if (m_pQbertComponent)
        m_pQbertComponent->PlayerMoved->RemoveObserver(this);
}

void dae::TileComponent::Init()
{
    m_pQbertComponent->PlayerMoved->AddObserver(this);
}

void dae::TileComponent::Notify(MovementDirection movementDirection)
{
    (void)movementDirection;
    //m_vNeighboringTiles[static_cast<size_t>(movementDirection)];
}

void dae::TileComponent::SubjectDestroyed(Subject<MovementDirection>* pSubject)
{
    if (pSubject == m_pQbertComponent->PlayerMoved.get())
        m_pQbertComponent = nullptr;
}

void dae::TileComponent::SetNeighboringTiles(const std::vector<std::vector<GameObject*>>& vTiles, int row, int col)
{
    m_vNeighboringTiles.push_back((row > 0) ? vTiles[row - 1][col]->GetComponent<TileComponent>() : nullptr);
    m_vNeighboringTiles.push_back((col < 7 - (row + 1)) ? vTiles[row + 1][col]->GetComponent<TileComponent>() : nullptr);
    m_vNeighboringTiles.push_back((col > 0) ? vTiles[row][col - 1]->GetComponent<TileComponent>() : nullptr);
    m_vNeighboringTiles.push_back((row < 7 - (col + 1)) ? vTiles[row][col + 1]->GetComponent<TileComponent>() : nullptr);
}

//---------------------------
// Member functions
//---------------------------

// write member functions here




