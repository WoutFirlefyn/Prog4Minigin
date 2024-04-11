#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "BaseComponent.h"
#include <vector>

//-----------------------------------------------------
// TileComponent Class									
//-----------------------------------------------------
namespace dae
{
	class TileComponent final : public BaseComponent
	{
	public:
		TileComponent(GameObject* pGameObject);	// Constructor
		virtual ~TileComponent() override = default;				// Destructor

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
		void SetNeighboringTiles(const std::vector<TileComponent*>& vNeighboringTiles) { m_vNeighboringTiles = vNeighboringTiles; }


	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------


		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------
		// vector containing all characters on this tile
		std::vector<TileComponent*> m_vNeighboringTiles{};

	};
}

 