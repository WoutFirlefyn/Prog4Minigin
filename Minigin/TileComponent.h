#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"
#include <vector>

//-----------------------------------------------------
// TileComponent Class									
//-----------------------------------------------------
namespace dae
{
	static bool QbertMoving{ false };

	enum class MovementState;
	enum class MovementDirection;
	class QbertComponent;
	class TileComponent final : public BaseComponent, public Observer<MovementState, MovementDirection> , public Observer<bool>
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
		virtual void SubjectDestroyed(Subject<MovementState, MovementDirection>* pSubject) override;

		virtual void Notify(bool roundFinished) override;

		void SetNeighboringTiles(const std::vector<std::vector<GameObject*>>& vTiles, size_t row, size_t col);
		void MoveQbertHere();

		static std::unique_ptr<Subject<bool>> TileChanged;
	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		bool AreAllTilesCovered() { return m_TileCount == m_TilesCovered; }


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		// vector containing all characters on this tile
		std::vector<TileComponent*> m_vNeighboringTiles{};
		QbertComponent* m_pQbertComponent{ nullptr };
		bool m_QbertIsHere{ false };
		int m_TileStage{ 0 };
		int m_MaxTileStage{ 2 };
		int m_CurrentRound{ 0 };
		const int m_TileId;
		static int m_TileCount;
		static int m_TilesCovered;
	};
}

 
