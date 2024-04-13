#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "QbertComponent.h"
#include "Observer.h"
#include <vector>

//-----------------------------------------------------
// TileComponent Class									
//-----------------------------------------------------
namespace dae
{
	//class QbertComponent;
	//enum class MovementDirection;
	static bool QbertMoving{ false };
	class TileComponent final : public BaseComponent , public Observer<MovementDirection>
	{
	public:
		TileComponent(GameObject* pGameObject);	// Constructor
		TileComponent(GameObject* pGameObject, QbertComponent* pQbertComponent);	// Constructor
		virtual ~TileComponent() override;				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		TileComponent(const TileComponent& other)					= delete;
		TileComponent(TileComponent&& other) noexcept				= delete;
		TileComponent& operator=(const TileComponent& other)		= delete;
		TileComponent& operator=(TileComponent&& other)	noexcept	= delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		virtual void Init() override;
		virtual void Update() override;

		virtual void Notify(MovementDirection movementDirection) override;
		virtual void SubjectDestroyed(Subject<MovementDirection>* pSubject) override;

		void SetNeighboringTiles(const std::vector<std::vector<GameObject*>>& vTiles, size_t row, size_t col);
		void MoveQbertHere();


	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		// vector containing all characters on this tile
		std::vector<TileComponent*> m_vNeighboringTiles{};
		QbertComponent* m_pQbertComponent{ nullptr };
		bool m_QbertIsHere{ false };

	};
}

 
