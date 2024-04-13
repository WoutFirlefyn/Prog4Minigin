#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "SpritesheetComponent.h"
#include "Observer.h"
#include <memory>

//-----------------------------------------------------
// TileSpritesheetComponent Class									
//-----------------------------------------------------
namespace dae
{
	class TileSpritesheetComponent final : public SpritesheetComponent
	{
	public:
		TileSpritesheetComponent(GameObject* pGameObject, int spriteCols, int spriteRows);				// Constructor
		~TileSpritesheetComponent() = default;				// Destructor

		// -------------------------
		// Copy/move constructors and assignment operators
		// -------------------------    
		TileSpritesheetComponent(const TileSpritesheetComponent& other)					 = delete;
		TileSpritesheetComponent(TileSpritesheetComponent&& other) noexcept				 = delete;
		TileSpritesheetComponent& operator=(const TileSpritesheetComponent& other)		 = delete;
		TileSpritesheetComponent& operator=(TileSpritesheetComponent&& other) noexcept   = delete;

		//-------------------------------------------------
		// Member functions						
		//-------------------------------------------------
		void MoveSourceRect(int cols, int rows);


	private:
		//-------------------------------------------------
		// Private member functions								
		//-------------------------------------------------
		

		//-------------------------------------------------
		// Datamembers								
		//-------------------------------------------------


	};
}

 
