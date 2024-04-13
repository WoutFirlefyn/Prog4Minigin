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
	static bool QbertMoving{ false };
	class TileComponent final : public BaseComponent, public Observer<MovementState, MovementDirection>
	{
	public:
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

		virtual void Notify(MovementState movementState, MovementDirection movementDirection) override;
		//virtual void Notify() override;
		virtual void SubjectDestroyed(Subject<MovementState, MovementDirection>* pSubject) override;
		//virtual void SubjectDestroyed(Subject<>* pSubject) override;

		void SetNeighboringTiles(const std::vector<std::vector<GameObject*>>& vTiles, size_t row, size_t col);
		void MoveQbertHere();

		static std::unique_ptr<Subject<>> TileChanged;
		static int m_TileCount;
		static int m_TilesCovered;
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
		int m_TileStage{ 0 };
		int m_MaxTileStage{ 1 };
		const int m_TileId;
	};
}

 
