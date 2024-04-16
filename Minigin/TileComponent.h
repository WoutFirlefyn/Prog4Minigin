#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include "Observer.h"
#include <vector>
#include <unordered_map>

//-----------------------------------------------------
// TileComponent Class									
//-----------------------------------------------------
namespace dae
{

	static bool QbertMoving{ false };

	enum class Characters;
	enum class MovementState;
	enum class MovementDirection;
	class QbertComponent;
	class TileComponent final : public BaseComponent, public Observer<Characters, MovementState, MovementDirection>, public Observer<bool>
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

		virtual void Notify(Characters character, MovementState movementState, MovementDirection movementDirection) override;
		virtual void SubjectDestroyed(Subject<Characters, MovementState, MovementDirection>* pSubject) override;

		virtual void Notify(bool roundFinished) override;

		void SetNeighboringTiles(const std::vector<std::vector<GameObject*>>& vTiles, size_t row, size_t col);
		bool IsEdgeTile() const;
		void MoveCharacterHere(const std::pair<Characters, GameObject*>& character);

		static std::unique_ptr<Subject<bool>> TileChanged;
	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		bool AreAllTilesCovered() { return m_MaxTileStage * m_TileCount == m_TilesCovered; }

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		std::vector<GameObject*> m_vNeighboringTiles{};
		std::unordered_map<Characters, GameObject*> m_CharactersHere;
		QbertComponent* m_pQbertComponent{ nullptr };
		//bool m_QbertIsHere{ false };
		int m_TileStage{ 0 };
		int m_MaxTileStage{ 1 };
		int m_CurrentRound{ 0 };
		const int m_TileId;
		static int m_TileCount;
		static int m_TilesCovered;

		static std::pair<Characters, GameObject*> tempQbertHolder;
	};
}

 
